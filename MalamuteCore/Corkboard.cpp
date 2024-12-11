#include "Corkboard.h"

#include "CorkboardBackend.h"
#include "Commands/AddWireCommand.h"
#include "Commands/AddCasingCommand.h"
#include "Commands/AddSelectionCommand.h"
#include "Commands/ChangeParamsCommand.h"
#include "Commands/MoveSelectionCommand.h"
#include "Commands/DeleteWireCommand.h"
#include "Commands/DeleteCasingCommand.h"
#include "Commands/DeleteSelectionCommand.h"
#include "Commands/ShiftWireCommand.h"
#include "Commands/ResizeCasingCommand.h"
#include "Commands/MinimizeCasingCommand.h"
#include "MalamuteCore/IdMaker.h"
#include "MalamuteCore/Styling/LabelStyle.h"
#include "WireBackend.h"

#include <QQmlApplicationEngine>

Corkboard::Corkboard(QQuickItem *parent) : QQuickItem(parent),
    m_undoStack(new UndoStack()),
    m_selectionOrPanStartPoint(QPointF()),
    m_selecting(false),
    m_panning(false),
    m_drawGrid(true),
    m_contextMenuActive(false),
    m_visibleWindowWidth(1280),
    m_visibleWindowHeight(720),
    m_contextMenuPt(QPointF()),
    m_movingItems(),
    m_initialPositions(),
    m_headerHeight(48)
{
    setFocus(true);
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);
    zoomFactor = 1.1;
}

CorkboardBackend *Corkboard::backend() const
{
    return m_backend;
}

void Corkboard::createBackend(std::shared_ptr<IdeaRegistry> ideaRegistry)
{
    m_backend = new CorkboardBackend(ideaRegistry, this, this);
}

void Corkboard::mousePressEvent(QMouseEvent *event)
{   
    if(mapToScene(event->pos()).y() < 40)
    {
        // I don't want people to access the corkboard by clicking on unused areas of the header.
        // This prevents that.
        return;
    }
    QQuickItem::forceActiveFocus();

    if(event->buttons() == Qt::RightButton)
    {
        //Show the Context Menu
        setContextMenuPt(mapToScene(event->pos()));
        setContextMenuActive(true);

        event->accept();
    }
    else if (event->buttons() == Qt::LeftButton && (event->modifiers() & Qt::ShiftModifier))
    {
        //Starting a selection box
        m_selectionOrPanStartPoint = event->pos();
        setSelecting(true);
        event->accept();
    }
    else if (event->buttons() == Qt::LeftButton)
    {
        // Click to clear selection, maybe start drag.
        deselectAll();
        setContextMenuActive(false);
        m_selectionOrPanStartPoint = event->globalPosition();
        m_startingPos = position();
        m_panning = true;
        event->accept();
    }
}

void Corkboard::mouseMoveEvent(QMouseEvent *event)
{
    if(m_panning)
    {
        QPointF panningCurrentPoint = event->globalPosition();
        QPointF displacement = m_selectionOrPanStartPoint - panningCurrentPoint;
        setPosition(m_startingPos - displacement);
        event->accept();
    }
    else if(m_selecting)
    {
        QPointF selectionCurrentPoint = event->pos();
        setSelectingBox(QRectF(m_selectionOrPanStartPoint,selectionCurrentPoint).normalized());
        event->accept();
    }
}

void Corkboard::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_panning)
    {
        m_panning = false;
        QPointF panningCurrentPoint = event->globalPosition();
        QPointF displacement = m_selectionOrPanStartPoint - panningCurrentPoint;
        setPosition(m_startingPos - displacement);
        event->accept();
    }
    if(m_selecting == true)
    {
        setSelecting(false);
        selectInBox(m_selectingBox);
        setSelectingBox(QRectF());
        event->accept();
    }
}

QQmlEngine *Corkboard::qmlEngine() const
{
    return m_qmlEngine;
}

void Corkboard::setQmlEngine(QQmlEngine *qmlEngine)
{
    m_qmlEngine = qmlEngine;
    m_rootContext = m_qmlEngine->rootContext();
}

void Corkboard::undo()
{
    m_undoStack->undo();
}

void Corkboard::redo()
{
    m_undoStack->redo();
}

void Corkboard::cut()
{
    addInternalWiresToSelection();
    QJsonObject obj = m_backend->saveSelection();
    QByteArray byteArray = QJsonDocument(obj).toJson(QJsonDocument::Compact);

    QMimeData* mimeData = new QMimeData();
    mimeData->setData(corkboardOperationsMimeType,byteArray);
    QClipboard* clipboard = QGuiApplication::clipboard();
    clipboard->setMimeData(mimeData);

    QQuickItem::forceActiveFocus();

    DeleteSelectionCommand* command = new DeleteSelectionCommand(obj, m_backend);
    m_undoStack->push(command);
}

void Corkboard::copy()
{
    addInternalWiresToSelection();
    QJsonObject obj = m_backend->saveSelection();
    QByteArray byteArray = QJsonDocument(obj).toJson(QJsonDocument::Compact);

    QQuickItem::forceActiveFocus();

    QMimeData* mimeData = new QMimeData();
    mimeData->setData(corkboardOperationsMimeType,byteArray);
    QClipboard* clipboard = QGuiApplication::clipboard();
    clipboard->setMimeData(mimeData);
}

void Corkboard::paste()
{
    deselectAll();
    QPointF corkboardPos = mapFromGlobal(QCursor::pos());
    QQuickItem::forceActiveFocus();

    QClipboard * clipboard = QGuiApplication::clipboard();
    const QMimeData* mimeData = clipboard->mimeData();

    if(!mimeData)
    {
        //Invalid paste operation
        return;
    }

    if(mimeData->hasFormat(corkboardOperationsMimeType))
    {
        QByteArray byteArray = mimeData->data(corkboardOperationsMimeType);
        QJsonObject const obj = QJsonDocument::fromJson(byteArray).object();
        AddSelectionCommand* command = new AddSelectionCommand(obj, m_backend, corkboardPos, true);
        m_undoStack->push(command);
    }
}

void Corkboard::duplicate()
{
    addInternalWiresToSelection();
    QJsonObject obj = m_backend->saveSelection();
    QQuickItem::forceActiveFocus();

    deselectAll();

    if(!obj.empty())
    {
        QPointF corkboardPos = mapFromGlobal(QCursor::pos());
        AddSelectionCommand* command = new AddSelectionCommand(obj, m_backend, corkboardPos, true);
        m_undoStack->push(command);
    }
}

void Corkboard::deleteSelection()
{
    QJsonObject obj = m_backend->saveSelection();
    DeleteSelectionCommand* command = new DeleteSelectionCommand(obj, m_backend);
    m_undoStack->push(command);
    QQuickItem::forceActiveFocus();
}

void Corkboard::deleteAll()
{
    selectAll();
    deleteSelection();
    QQuickItem::forceActiveFocus();
}

void Corkboard::setVisibleWindowParams(int width, int height)
{
    m_visibleWindowWidth = width;
    m_visibleWindowHeight = height;
}

void Corkboard::centerView()
{
    setX(-1 * (width() - m_visibleWindowWidth) / 2);
    setY(-1 * (height() - m_visibleWindowHeight) / 2);
}

void Corkboard::resetScale()
{
    setScale(1.0);
}

void Corkboard::zoomIn()
{
    setScale(scale() * zoomFactor);
}

void Corkboard::zoomOut()
{
    setScale(scale() / zoomFactor);
}

void Corkboard::minimizeFromCasingMenu()
{
    int id = casingWhichCreatedMenu->casingBackend()->id();
    bool oldState = casingWhichCreatedMenu->minimized();

    MinimizeCasingCommand* command = new MinimizeCasingCommand(id, this, oldState, true);
    m_undoStack->push(command);
}

void Corkboard::showControlsFromCasingMenu()
{
    casingWhichCreatedMenu->extraButtonClicked();
}

void Corkboard::changeNameFromCasingMenu(const QString &newIdeaName)
{
    casingWhichCreatedMenu->setIdeaName(newIdeaName);
}

void Corkboard::deleteFromCasingMenu()
{
    DeleteCasingCommand* command = new DeleteCasingCommand(casingWhichCreatedMenu->casingBackend()->save(), m_backend);
    m_undoStack->push(command);

    setFocus(true);
}

void Corkboard::deleteFromCasing(Casing *casing)
{
    DeleteCasingCommand* command = new DeleteCasingCommand(casing->casingBackend()->save(), m_backend);
    m_undoStack->push(command);

    setFocus(true);
}

void Corkboard::zoomIn(QPointF pos)
{
    QPointF windowPos = mapToScene(pos);

    setScale(scale() * zoomFactor);
    double p_x = (pos.x() - width()/2.0) / width();
    double p_y = (pos.y() - height()/2.0) / height();

    setX(-width()/2.0 - p_x * width() * scale() + windowPos.x());
    setY(-height()/2.0 - p_y * height() * scale() + windowPos.y() - m_headerHeight);
}

void Corkboard::zoomOut(QPointF pos)
{
    QPointF windowPos = mapToScene(pos);

    setScale(scale() / zoomFactor);
    double p_x = (pos.x() - width()/2.0) / width();
    double p_y = (pos.y() - height()/2.0) / height();

    setX(-width()/2.0 - p_x * width() * scale() + windowPos.x());
    setY(-height()/2.0 - p_y * height() * scale() + windowPos.y() - m_headerHeight);
}

bool Corkboard::selecting() const
{
    return m_selecting;
}

bool Corkboard::contextMenuActive() const
{
    return m_contextMenuActive;
}

QPointF Corkboard::contextMenuPt() const
{
    return m_contextMenuPt;
}

void Corkboard::setSelecting(bool selecting)
{
    if (m_selecting == selecting)
        return;

    m_selecting = selecting;
    emit selectingChanged(m_selecting);
}
void Corkboard::setContextMenuActive(bool contextMenuActive)
{
    if (m_contextMenuActive == contextMenuActive)
        return;

    m_contextMenuActive = contextMenuActive;
    emit contextMenuActiveChanged(m_contextMenuActive);
}

void Corkboard::setContextMenuPt(QPointF contextMenuPt)
{
    if (m_contextMenuPt == contextMenuPt)
        return;

    m_contextMenuPt = contextMenuPt;
    emit contextMenuPtChanged(m_contextMenuPt);
}

void Corkboard::setCasingMenuActive(bool casingMenuActive)
{
    if (m_casingMenuActive == casingMenuActive)
        return;

    m_casingMenuActive = casingMenuActive;
    emit casingMenuActiveChanged(m_casingMenuActive);
}

void Corkboard::setCasingMenuPt(QPointF casingMenuPt)
{
    if (m_casingMenuPt == casingMenuPt)
        return;

    m_casingMenuPt = casingMenuPt;
    emit casingMenuPtChanged(m_casingMenuPt);
}

void Corkboard::setHeaderHeight(int headerHeight)
{
    if (m_headerHeight == headerHeight)
        return;

    m_headerHeight = headerHeight;
    emit headerHeightChanged(m_headerHeight);
}

UndoStack* Corkboard::undoStack() const
{
    return m_undoStack;
}

int Corkboard::headerHeight() const
{
    return m_headerHeight;
}

void Corkboard::addInternalWiresToSelection()
{
    std::vector<Casing*> casings = selectedCasings();
    std::vector<int> inIds;
    std::vector<int> outIds;

    //Loop over selected casings. Add all wire in plugs to them.
    for(ulong i = 0; i < casings.size(); i++)
    {
        std::vector<int> a = casings[i]->casingBackend()->wireIds(PlugType::IN);
        inIds.insert(inIds.end(), a.begin(), a.end());
    }

    //Loop over selected casings. Add all wire out plugs to them.
    for(ulong i = 0; i < casings.size(); i++)
    {
        std::vector<int> a = casings[i]->casingBackend()->wireIds(PlugType::OUT);
        outIds.insert(outIds.end(), a.begin(), a.end());
    }

    //Compare loops set selected to true on duplicates.
    for(size_t i = 0; i < inIds.size(); i++)
    {
        for(size_t j = 0; j < outIds.size(); j++)
        {
            if(inIds[i] == outIds[j])
            {
                m_backend->getWireBackend(inIds[i])->getWire()->setSelected(true);
            }
        }
    }
}

void Corkboard::setSelectingBox(QRectF selectingBox)
{
    if (m_selectingBox == selectingBox)
        return;

    m_selectingBox = selectingBox;
    emit selectingBoxChanged(m_selectingBox);
}

std::shared_ptr<DataTypeRegistry> Corkboard::dataTypeRegistry() const
{
    return m_dataTypeRegistry;
}

void Corkboard::setDataTypeRegistry(const std::shared_ptr<DataTypeRegistry> &dataTypeRegistry)
{
    m_dataTypeRegistry = dataTypeRegistry;
}

bool Corkboard::casingMenuActive() const
{
    return m_casingMenuActive;
}

QPointF Corkboard::casingMenuPt() const
{
    return m_casingMenuPt;
}

void Corkboard::readyCasingMenu(Casing *casing, QPointF point)
{
    casingWhichCreatedMenu = casing;
    setCasingMenuPt(point);
    setCasingMenuActive(true);
}

QRectF Corkboard::selectingBox() const
{
    return m_selectingBox;
}

void Corkboard::setWirePlugSwitch(const QJsonObject &wirePlugSwitch)
{
    m_wirePlugSwitch = wirePlugSwitch;
}

void Corkboard::createIdea(const QJsonObject &jsonObj)
{
    AddCasingCommand* command = new AddCasingCommand(jsonObj, m_backend);
    m_undoStack->push(command);
}

void Corkboard::createIdea(QString name, QPointF point)
{
    deselectAll();
    QPointF corkboardPos =  mapFromScene(point);

    QJsonObject obj;
    obj["x"] = corkboardPos.x();
    obj["y"] = corkboardPos.y();

    //How to get the colors. This might be worth improving later.
    std::unique_ptr<Idea> idea = m_backend->registry().createIdea(name);
    int outColorsCount = idea->attributes().dataTypesOut().size();

    QJsonArray colorArray;
    for(int i = 0; i < outColorsCount; i++)
    {
        QColor color = Magic<LabelStyle>::cast().getColor();
        colorArray.append(color.name());
    }
    QJsonObject ideaJson;
    ideaJson["c"] = colorArray;

    QJsonObject casingJson;
    casingJson["ia"] = ideaJson;
    casingJson["id"] = Magic<IdMaker>::cast().getID();
    casingJson["nm"] = name;
    casingJson["cp"] = obj;

    AddCasingCommand* command = new AddCasingCommand(casingJson, m_backend);
    m_undoStack->push(command);
}

Casing *Corkboard::createCasing(const QJsonObject &json)
{
    QString ideaName = json["nm"].toString();
    std::unique_ptr<Idea> idea = m_backend->registry().createIdea(ideaName);


    QQmlComponent qmlComponent(qmlEngine(),QUrl(QStringLiteral("qrc:/QML/Casing.qml")));
    QObject* obj = qmlComponent.create(m_rootContext);
    Casing* casing = qobject_cast<Casing*>(obj);

    auto casingBackend = m_backend->createCasingBackend(std::move(idea), casing, json);
    casingBackend->prepareContext(new QQmlContext(m_rootContext,casingBackend));

    casing->setBackend(casingBackend);
    casing->setCorkboard(this);
    casingBackend->finishSetup(json["ia"].toObject());
    casing->finishSetup();

    setFocus(true);
    return casing;
}

Wire *Corkboard::createWire(WireBackend* wireBackend)
{
    deselectAll();

    QQmlComponent component(qmlEngine(),"qrc:/QML/Wire.qml");
    QObject* preWire = component.create(m_rootContext);

    Wire* wire = qobject_cast<Wire*>(preWire);
    wire->setParentItem(this);
    wire->setParent(this);
    wire->setCorkboard(this);
    wire->setBackend(wireBackend);
    wireBackend->setWire(wire);

    return wire;
}

Wire *Corkboard::createWireFront(const QJsonObject &jsonObj)
{
    auto wireBackend = backend()->createWireBackend(jsonObj);

    QQmlComponent qmlComponent(qmlEngine(),"qrc:/QML/Wire.qml");
    QObject* preWire = qmlComponent.create(m_rootContext);

    Wire* wire = qobject_cast<Wire*>(preWire);
    wire->setParentItem(this);
    wire->setParent(this);
    wire->setCorkboard(this);
    wire->setBackend(wireBackend.get());
    wireBackend->setWire(wire);

    wireBackend->getCasing(PlugType::OUT)->sendDataNewWire(wireBackend->id());

    int outPlug = wireBackend->getPlug(PlugType::OUT);
    QColor color = wireBackend.get()->getCasing(PlugType::OUT)->casing().outPlugLabelColors().at(outPlug);
    wire->setColor(color);
    if(wireBackend->plugedIn()) // Creating a completed wire and need to color things.
        wireBackend->getCasing(PlugType::IN)->casing().setInPlugColor(wireBackend->getPlug(PlugType::OUT), color);


    return wire;
}

void Corkboard::shiftWire(const QJsonObject &oldWire, const QJsonObject &newWire)
{
    ShiftWireCommand* command = new ShiftWireCommand(oldWire, newWire, m_backend, false);
    m_undoStack->push(command);

    setFocus(true);
}

void Corkboard::changeParams(int id, QJsonObject oldParams, QJsonObject newParams, bool actionNeeded)
{
    ChangeParamsCommand* command = new ChangeParamsCommand(id,oldParams, newParams, m_backend, actionNeeded);
    m_undoStack->push(command);
}

void Corkboard::changeSize(int id, QPointF oldSize, QPointF newSize)
{
    ResizeCasingCommand* command = new ResizeCasingCommand(id, this, oldSize, newSize);
    m_undoStack->push(command);
}

void Corkboard::createOrShiftWire(const QJsonObject &jsonObj)
{
    if(m_wirePlugSwitch.isEmpty())
    {
        //this looks weird, because it is. The final parameter false just means don't do this the first time.
        // it actually works, just it's odd that it works.
        AddWireCommand* command = new AddWireCommand(jsonObj, m_backend, false);
        m_undoStack->push(command);
    }
    else
    {
        if(jsonObj["iid"] == m_wirePlugSwitch["iid"] &&
                jsonObj["oid"] == m_wirePlugSwitch["oid"] &&
                jsonObj["ip"] == m_wirePlugSwitch["ip"] &&
                jsonObj["op"] == m_wirePlugSwitch["op"])
        {
            // Case when a wire was disconnected and then reconnected
            // to the same plug.
            // Since nothing happened, nothing needs to happen.
        }
        else
        {
            //this looks weird, because it is. The final parameter false just means don't do this the first time.
            // it actually works, just it's odd that it works.
            ShiftWireCommand* command = new ShiftWireCommand(m_wirePlugSwitch, jsonObj, m_backend, false);
            m_undoStack->push(command);
        }
    }
}

bool Corkboard::destroyLooseWire()
{
    if(m_wirePlugSwitch.isEmpty())
    {
        //No wire existed before this. So nothing needs to be done.
        return false;
    }
    else
    {
        //Case when a wire is destroyed by dragging off into the distance.
        DeleteWireCommand* command = new DeleteWireCommand(m_wirePlugSwitch, m_backend);
        m_undoStack->push(command);
        return true;
    }
}

void Corkboard::moveItemFromCommand(int id, QPointF pos)
{
    CasingBackend* casingBackend = backend()->getCasing(id);
    if(casingBackend)
    {
        casingBackend->casing().setX(pos.x());
        casingBackend->casing().setY(pos.y());
        casingBackend->moveWires();
        return;
    }
    qFatal("Move called on invalid id");
}

void Corkboard::reportParamsChanged(int id, QJsonObject oldParams, QJsonObject newParams)
{
    ChangeParamsCommand* command = new ChangeParamsCommand(id, oldParams, newParams, m_backend, false);
    m_undoStack->push(command);
}

std::vector<CasingBackend *> Corkboard::locateCasingNear(QPointF pos, int padding)
{
    std::vector<CasingBackend*> casingBackends;



    QObjectList children = this->children();

    for(int i = 0; i < children.count(); i++)
    {
        Casing* casing = qobject_cast<Casing*>(children[i]);

        if(casing && casing->x() < pos.x() + padding && casing->y() < pos.y() + padding &&
                casing->x()+casing->width() + padding > pos.x() &&
                casing->y() + casing->height() + padding > pos.y())
        {
            casingBackends.push_back(casing->casingBackend());
        }
    }
    return casingBackends;
}

void Corkboard::deselectAll()
{
    foreach(QObject* obj, children())
    {
        Casing* casing = qobject_cast<Casing*>(obj);
        if(casing)
        {
            casing->setSelected(false);
            continue;
        }

        Wire* wire = qobject_cast<Wire*>(obj);
        if(wire)
        {
            wire->setSelected(false);
            continue;
        }
    }
}

void Corkboard::selectAll()
{
    foreach(QObject* obj, children())
    {
        Casing* casing = qobject_cast<Casing*>(obj);
        if(casing)
        {
            casing->setSelected(true);
            continue;
        }

        Wire* wire = qobject_cast<Wire*>(obj);
        if(wire)
        {
            wire->setSelected(true);
            continue;
        }
    }
}

QByteArray Corkboard::save()
{
    QJsonObject sceneJson = m_backend->save();
    QJsonDocument doc(sceneJson);
    return doc.toJson(QJsonDocument::Compact);
}

void Corkboard::load(QByteArray data)
{
    m_undoStack->clear();
    m_backend->load(data);
}

void Corkboard::selectInBox(QRectF selectionBox)
{
    foreach (QObject* obj, children())
    {
        Casing* casing = qobject_cast<Casing*>(obj);
        if(casing)
        {
            double casingCenterX = casing->x() + casing->width() / 2;
            double casingCenterY = casing->y() + casing->height() / 2;

            if(casingCenterX > selectionBox.left() && casingCenterX < selectionBox.right() && casingCenterY > selectionBox.top() && casingCenterY < selectionBox.bottom())
                casing->setSelected(true);
            continue;
        }
        Wire* wire = qobject_cast<Wire*>(obj);
        if(wire)
        {
            double wireCenterX = wire->x() + wire->width() / 2;
            double wireCenterY = wire->y() + wire->height() / 2;

            if(wireCenterX > selectionBox.left() && wireCenterX < selectionBox.right() && wireCenterY > selectionBox.top() && wireCenterY < selectionBox.bottom())
                wire->setSelected(true);
            continue;
        }
    }
}

QObjectList Corkboard::selected()
{
    QObjectList list;

    foreach (QObject* obj, children())
    {
        Casing* casing = qobject_cast<Casing*>(obj);
        if(casing && casing->selected())
        {
            list.append(casing);
            continue;
        }
        Wire* wire = qobject_cast<Wire*>(obj);
        if(wire && wire->selected())
        {
            list.append(wire);
            continue;
        }
    }
    return list;
}

std::vector<Casing *> Corkboard::selectedCasings()
{
    std::vector<Casing*> casings;
    foreach (QObject* obj, children())
    {
        Casing* casing = qobject_cast<Casing*>(obj);
        if(casing && casing->selected())
        {
            casings.push_back(casing);
        }

    }
    return casings;
}

std::vector<QQuickItem *> Corkboard::selectedNonWires()
{
    std::vector<QQuickItem*> selectedItems;
    foreach (QObject* obj, children())
    {
        Casing* casing = qobject_cast<Casing*>(obj);
        if(casing && casing->selected())
        {
            selectedItems.push_back(casing);
            continue;
        }
    }
    return selectedItems;
}

std::vector<Wire *> Corkboard::selectedWires()
{
    std::vector<Wire*> wires;
    foreach (QObject* obj, children())
    {
        Wire* wire = qobject_cast<Wire*>(obj);
        if(wire && wire->selected())
        {
            wires.push_back(wire);
        }
    }
    return wires;
}

bool Corkboard::canUndo()
{
    return m_undoStack->canUndo();
}

void Corkboard::prepareMoveSelection()
{
    m_movingItems = selectedNonWires();
    m_initialPositions.reserve(m_movingItems.size());

    for(uint i = 0; i < m_movingItems.size(); i++)
    {
        m_initialPositions.push_back(m_movingItems[i]->position());
    }
}

void Corkboard::moveSelection(QPointF displacement)
{
    QPointF scaledDisplacement(displacement.x() / scale(),
                               displacement.y() / scale());


    for(uint i = 0; i < m_movingItems.size(); i++)
    {
        m_movingItems[i]->setPosition(m_initialPositions[i] + scaledDisplacement);
        Casing* casing = qobject_cast<Casing*>(m_movingItems[i]);
        if(casing)
            casing->casingBackend()->moveWires();
    }
}

void Corkboard::finishMoveSelection()
{
    //This sets up the undo/redo stack to do the rigth things.
    std::vector<QPointF> finalPositions;
    std::vector<int> idList;

    uint size = static_cast<uint>(m_movingItems.size());
    finalPositions.reserve(size);
    idList.reserve(size);

    for(uint i = 0; i < size; i++)
    {
        Casing* casing = qobject_cast<Casing*>(m_movingItems[i]);
        if(casing)
        {
            casing->casingBackend()->moveWires();
            idList.push_back(casing->casingBackend()->id());
            finalPositions.push_back(m_movingItems[i]->position());
            continue;
        }
    }

    //Ensure a move actually happened.
    if(qAbs(m_initialPositions[0].x() - finalPositions[0].x()) > 0.00001
            || qAbs(m_initialPositions[0].y() - finalPositions[0].y()) > 0.00001)
    {
        MoveSelectionCommand* command = new MoveSelectionCommand(idList, m_initialPositions, finalPositions, this, false);
        m_undoStack->push(command);
    }

    m_initialPositions.clear();
}

void Corkboard::resizeCasingFromCommand(int id, QPointF size)
{
    CasingBackend* wrap = backend()->getCasing(id);

    if(!wrap->casing().embeddedQML())
        return;

    QQuickItem* embedded = wrap->getEmbeddedQML();
    embedded->setWidth(size.x());
    embedded->setHeight(size.y());
}

void Corkboard::reportSizeChanged(int id, QPointF oldSize, QPointF newSize)
{
    //Ensure a resize actually happened.
    if(qAbs(oldSize.x() - newSize.x()) > 0.00001
            || qAbs(oldSize.y() - newSize.y()) > 0.00001)
    {
        ResizeCasingCommand* command = new ResizeCasingCommand(id, this, oldSize, newSize);
        m_undoStack->push(command);
    }
}

void Corkboard::reportIdeaMinized(int id, bool oldState)
{
    MinimizeCasingCommand* command = new MinimizeCasingCommand(id, this, oldState);
    m_undoStack->push(command);
}

void Corkboard::minimizeCasingFromCommand(int id, bool state)
{
    CasingBackend* wrap = backend()->getCasing(id);
    wrap->casing().setMinimized(state);
}

void Corkboard::wheelEvent(QWheelEvent *event)
{
    if(event->angleDelta().y() > 0)
        zoomIn(event->position());
    else
        zoomOut(event->position());
}

void Corkboard::keyPressEvent(QKeyEvent *event)
{
    if(event->matches(QKeySequence::Copy))
        copy();
    else if(event->matches(QKeySequence::Cut))
        cut();
    else if(event->matches(QKeySequence::Paste))
        paste();
    else if(event->matches(QKeySequence::Undo))
        undo();
    else if(event->matches(QKeySequence::Redo))
        redo();
    else if(event->modifiers() & Qt::ControlModifier && event->key() == Qt::Key_D)
    {
        duplicate();
    }
    else if(event->matches(QKeySequence::SelectAll))
        selectAll();

    if(event->key() ==  Qt::Key_Delete)
        deleteSelection();
    else if(event->key() == Qt::Key_Left)
        setX(x() + 100);
    else if(event->key() == Qt::Key_Right)
        setX(x() - 100);
    else if(event->key() == Qt::Key_Up)
        setY(y() + 100);
    else if(event->key() == Qt::Key_Down)
        setY(y() - 100);
}
