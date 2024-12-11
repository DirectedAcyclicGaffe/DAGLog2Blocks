#include "Casing.h"

#include <QJsonValue>

#include "Wire.h"
#include "CorkboardBackend.h"
#include "Corkboard.h"
#include "CasingBackend.h"
#include "PlugEvents.h"
#include "Styling/LabelStyle.h"
#include "Styling/CasingStyle.h"
#include "WireBackend.h"

Casing::Casing(QQuickItem *parent) : QQuickItem(parent),
    m_moving(false),
    m_resizing(false),

    m_QMLBoxX(0),
    m_QMLBoxY(0),
    m_QMLBoxWidth(0),
    m_QMLBoxHeight(0),

    m_selected(true),
    m_minimized(false),
    m_minimizedSetDuringLoad(false),
    m_changeNameOnMinimized(false),
    m_nameChanged(false)
{
    setFlag(ItemHasContents);
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);
    setZ(3.0);
}

Casing::~Casing()
{

}

QJsonObject Casing::save() const
{
    QJsonObject saveJson;
    saveJson["x"] = x();
    saveJson["y"] = y();
    if(m_nameChanged)
        saveJson["n"] = m_ideaName;
    if(m_minimizable)
        saveJson["m"] = m_minimized;
    return saveJson;
}

void Casing::load(const QJsonObject &json)
{
    QJsonValue x = json["x"];
    if(!x.isUndefined())
        setX(x.toDouble());

    QJsonValue y = json["y"];
    if(!y.isUndefined())
        setY(y.toDouble());


    QJsonValue m = json["m"];
    if(!m.isUndefined())
    {
        m_minimized = m.toBool();
        m_minimizedSetDuringLoad = true;
    }

    QJsonValue n = json["n"];
    if(!n.isUndefined())
    {
        m_nameChanged = true;
        setIdeaName(n.toString());
    }
}
void Casing::finishSetup()
{
    IdeaAttributes attributes = m_backend->idea()->attributes();
    m_hasEmbeddedQML = attributes.embeddedQMLPath() == "" ? false : true;
    m_hasMinimizedQML = attributes.minimizedQMLPath() == "" ? false : true;
    m_changeNameOnMinimized = attributes.differentNameOnMinimized();
    setHasExtraQML(attributes.externalQMLPaths().size() == 0 ? false : true);

    m_resizeable = attributes.resizeable();
    emit resizeableChanged(m_resizeable);
    m_minimizable = attributes.minimizable();
    emit minimizableChanged(m_minimizable);

    if(m_minimizable && !m_minimizedSetDuringLoad)
        m_minimized = attributes.startMinimized();

    m_backend->idea()->setMinimized(m_minimized);

    setupInPlugs();
    setupOutPlugs();
    setupCaption();

    m_backend->clearStopLights();

    if(m_minimizable == false && m_hasEmbeddedQML == true)
        m_backend->loadEmbeddedQML();
    else if(m_minimizable == true && m_minimized == false && m_hasEmbeddedQML == true)
        m_backend->loadEmbeddedQML();

    else if(m_minimizable == true && m_minimized == true && m_hasMinimizedQML == true)
        m_backend->loadMinimizedQML();

    setupFeedbackBox();
}

void Casing::setupCaption()
{
    if(m_nameChanged)
        return;
    IdeaAttributes attributes = m_backend->idea()->attributes();

    if(attributes.displayName() == "")
    {
        m_nameSize = QSize();
        emit nameSizeChanged(m_nameSize);
        m_ideaName = "";
        emit ideaNameChanged(m_ideaName);
    }
    else if(!m_minimized || !m_changeNameOnMinimized)
    {
        m_ideaName = attributes.displayName();
        emit ideaNameChanged(m_ideaName);

        QFont nameFont("verdana",20, QFont::Bold);
        nameFont.setBold(true);
        QFontMetrics nameFontMetrics(nameFont);
        m_nameSize = nameFontMetrics.size(Qt::TextShowMnemonic,ideaName());
        emit nameSizeChanged(m_nameSize);
    }
    else /// Minimized and name different on minimized
    {
        m_ideaName = attributes.minimizedName();
        emit ideaNameChanged(m_ideaName);

        QFont nameFont("verdana",20, QFont::ExtraBold);
        nameFont.setBold(true);
        QFontMetrics nameFontMetrics(nameFont);
        m_nameSize = nameFontMetrics.size(Qt::TextShowMnemonic,ideaName());
        emit nameSizeChanged(m_nameSize);
    }
}

void Casing::setupInPlugs()
{
    IdeaAttributes attributes = m_backend->idea()->attributes();

    //Setup the sink plugs
    m_inPlugs = static_cast<int> (attributes.dataTypesIn().size());
    m_inPlugLabels.reserve(m_inPlugs);
    int labelWidth = 0;
    for(int i = 0; i < m_inPlugs; i++)
    {
        QString label;
        if(attributes.customLabels())
            label = attributes.labelsIn().at(i);
        else
            label = m_corkboard->dataTypeRegistry()->dataTypeAbbreviation(attributes.dataTypesIn()[i]);

        QFont labelFont("verdana",11, QFont::Bold);
        QFontMetrics fm(labelFont);

        if(fm.horizontalAdvance(label) >= labelWidth)
            labelWidth = fm.horizontalAdvance(label);

        m_inPlugLabels.append(label);
    }

    m_inPlugLabelColors.reserve(m_inPlugs);

    if(m_backend->idea()->inPlugLabelColors().size() != 0)
    {
        m_inPlugLabelColors = m_backend->idea()->inPlugLabelColors();
    }
    else
    {
        m_backend->idea()->prepareInPlugLabelColors(m_inPlugs,  LabelStyle::defaultColor());
        for(int i = 0; i < m_inPlugs; i++)
        {
            m_inPlugLabelColors.push_back(LabelStyle::defaultColor());
            m_backend->idea()->setInPlugLabelColor(i, LabelStyle::defaultColor());
        }
    }
    setInLabelWidth(labelWidth);
    emit inPlugLabelsChanged(m_inPlugLabels);
}

void Casing::setupOutPlugs()
{
    IdeaAttributes attributes = m_backend->idea()->attributes();

    //Setup the source plugs
    m_outPlugs = static_cast<int> (attributes.dataTypesOut().size());
    int labelWidth = 0;
    for(int i = 0; i < m_outPlugs; i++)
    {
        QString label;
        if(attributes.customLabels())
            label = attributes.labelsOut().at(i);
        else
            label = m_corkboard->dataTypeRegistry()->dataTypeAbbreviation(attributes.dataTypesOut()[i]);

        QFont labelFont("verdana",11, QFont::Bold);
        QFontMetrics fm(labelFont);

        if(fm.horizontalAdvance(label) >= labelWidth)
            labelWidth = fm.horizontalAdvance(label);

        m_outPlugLabels.append(label);
    }

    m_outPlugLabelColors.reserve(m_outPlugs);

    if(m_backend->idea()->outPlugLabelColors().size() != 0)
    {
        m_outPlugLabelColors = m_backend->idea()->outPlugLabelColors();
    }
    else
    {
        QList<QColor> colorList;
        for(int i = 0; i < m_outPlugs; i++)
        {
            QColor color = Magic<LabelStyle>::cast().getColor();
            colorList.append(color);
            m_outPlugLabelColors.push_back(color);
        }
        m_backend->idea()->prepareOutPlugLabelColors(colorList);
    }

    setOutLabelWidth(labelWidth);
    emit outPlugLabelsChanged(m_outPlugLabels);
}

void Casing::setupFeedbackBox()
{
    QString feedbackMessage = m_backend->idea()->statusMessage();
    setFeedbackMessage(feedbackMessage);
    if(feedbackMessage == "")
    {
        setFeedbackBoxSize(QSize(0,0));
        recalculateSize();
        return;
    }

    QFont statusFont("verdana",14, QFont::Bold);
    statusFont.setBold(true);
    QFontMetrics statusFontMetrics(statusFont);
    QSize statusSize = statusFontMetrics.size(Qt::TextShowMnemonic, feedbackMessage);
    setFeedbackBoxSize(statusSize);

    //Setup the validation area.
    IdeaStatus feedbackStatus = m_backend->idea()->status();

    switch (feedbackStatus)
    {
    case IdeaStatus::Working:
        setFeedbackColor(QColor(0x14af14));
        break;
    case IdeaStatus::StandBy:
        setFeedbackColor(QColor(0x62aeef));
        break;
    case IdeaStatus::Warning:
        setFeedbackColor(QColor(0x78ae51));
        break;
    case IdeaStatus::DisplayWarning:
        setFeedbackColor(QColor(0xae9451));
        break;
    case IdeaStatus::Error:
        setFeedbackColor(QColor(0xf64f4f));
        break;
    case IdeaStatus::Fatal:
        setFeedbackColor(QColor(0x010101));
        break;
    }
    recalculateSize();
}

Corkboard *Casing::corkboard() const
{
    return m_corkboard;
}

CasingBackend* Casing::casingBackend()
{
    return m_backend;
}

void Casing::setCorkboard(Corkboard *corkboard)
{
    m_corkboard = corkboard;
    setParentItem(corkboard);
    setParent(corkboard);
}

void Casing::setBackend(CasingBackend *backend)
{
    m_backend = backend;
}

void Casing::mousePressEvent(QMouseEvent *event)
{
    setFocus(true);
    m_backend->onDeleteExternalQML();

    if(event->buttons() == Qt::RightButton)
    {
        if(!(event->modifiers() & Qt::ControlModifier))
        {
            m_corkboard->readyCasingMenu(this, mapToScene(event->pos()));
            event->accept();
            return;
        }
        else
        {
            int const plug = mouseOverPlug(PlugType::OUT, event->pos());

            if(plug != -1)
            {
                QColor newColor = Magic<LabelStyle>::cast().getColor();
                setOutPlugColor(plug, newColor);
                
                std::list<WireBackend*> wires = m_backend->getWiresAtPlug(PlugType::OUT,plug);

                if(!wires.empty())
                {
                    for(auto& wire : wires)
                    {
                        int plugNum = wire->getPlug(PlugType::IN);
                        wire->getCasing(PlugType::IN)->casing().setInPlugColor(plugNum, newColor);
                    }
                }
                return;
            }

            int const plug2 = mouseOverPlug(PlugType::IN, event->pos());
            if(plug2 != -1)
            {
                std::list<WireBackend*> wires = m_backend->getWiresAtPlug(PlugType::IN,plug2);

                if(wires.size() == 0) // Don't change color no input.
                    return;

                QColor newColor = Magic<LabelStyle>::cast().getColor();
                setInPlugColor(plug2, newColor);

                CasingBackend* inputCasing = wires.front()->getCasing(PlugType::OUT);
                //CasingBackend * inputCasing = wires[0]->getCasing(PlugType::OUT);
                //int const inputplug = wires[0]->getPlug(PlugType::OUT);
                int const inputPlut = wires.front()->getPlug(PlugType::OUT);

                inputCasing->casing().setOutPlugColor(inputPlut, newColor);

                std::list<WireBackend*> wires2 = inputCasing->getWiresAtPlug(PlugType::OUT,inputPlut);
                if(!wires2.empty())
                {
                    for(auto& wire : wires2)
                    {
                        int plugNum = wire->getPlug(PlugType::IN);
                        wire->getCasing(PlugType::IN)->casing().setInPlugColor(plugNum, newColor);
                    }
                }
            }

            return;
        }
    }

    //Check if creating/editing a wire.
    for (PlugType plugToCheck: {PlugType::IN, PlugType::OUT})
    {
        int const plug = mouseOverPlug(plugToCheck,event->pos());

        if(plug != -1)
        {
            //Clicked on Valid plug. If there is an existing wire drag it. If no wire create one.
            std::list<WireBackend*> wires = m_backend->getWiresAtPlug(plugToCheck,plug);

            //Drag or delete existing wire.
            if(!wires.empty() && plugToCheck == PlugType::IN)
            {
                setInPlugToDefaultColor(plug);
                auto wire = wires.front();
                wire->sendNullData();

                //This stores the previous state for the undo/redo command framework.
                m_corkboard->setWirePlugSwitch(wire->save());
                
                PlugEvents interaction(*m_backend,*wire);
                interaction.unPlug(plugToCheck);

                if(event->modifiers() & Qt::AltModifier)
                    m_corkboard->destroyLooseWire();
                else
                {
                    wire->getWire()->simulateMousePressEvent(event);
                    wire->getWire()->grabMouse();
                }

                event->accept();
                return;
            }

            else //Create new wire
            {
                //This lets the undo/redo framework know that there wasn't a previous wire.
                m_corkboard->setWirePlugSwitch(QJsonObject());

                auto wireBackend = m_corkboard->backend()->createWireBackend(plugToCheck,m_backend,plug);
                Wire* wire = m_corkboard->createWire(wireBackend.get());

                QColor wireColor;
                if(plugToCheck == PlugType::IN)
                    wireColor = QColor(0xa6b0cb);
                else
                    wireColor = outPlugLabelColors().at(plug);

                wire->setColor(wireColor);
                
                m_backend->addWire(plugToCheck,plug,*wireBackend);

                event->accept();
                wireBackend->getWire()->simulateMousePressEvent(event);
                wireBackend->getWire()->grabMouse();
                event->accept();
                return;
            }
        }
    }

    if (!(event->modifiers() & Qt::ShiftModifier))
    {
        if(selected() == false)
            m_corkboard->deselectAll();

        setSelected(true);
        event->accept();
    }
    else
    {
        setSelected(!selected());
        event->accept();
        return;
    }

    //Check if mouse is over the resizer.
    if(resizeable() && checkHitResizer(event->pos()))
    {
        QQuickItem* embed = m_backend->getEmbeddedQML();
        initialClickPos = event->scenePosition();
        initialSize = QPointF(embed->width(), embed->height());
        m_resizing = true;
    }
    else
    {
        //Assume the begining of a drag event.
        initialClickPos = event->scenePosition();
        m_moving = true;
        m_backend->setEmbeddedQMLEnabled(false);
        m_corkboard->prepareMoveSelection();
    }
    event->accept();
}

void Casing::mouseMoveEvent(QMouseEvent *event)
{
    if(m_moving && event->buttons() & Qt::LeftButton)
    {
        QPointF displacement = event->scenePosition() - initialClickPos;
        m_corkboard->moveSelection(displacement);
    }

    if(m_resizing && event->buttons() & Qt::LeftButton)
    {
        QPointF displacement = (event->scenePosition() - initialClickPos) / corkboard()->scale();
        //The corkboard can be scaled. (Zoom in/Zoom out). The scale affects the children of the corkboard, aka the casing too.
        // So the displacement needs to be scaled. If this wasn't here the resizing and mouse move at different rates.
        QQuickItem* embed = m_backend->getEmbeddedQML();
        embed->setWidth(initialSize.x() + displacement.x());
        embed->setHeight(initialSize.y() + displacement.y());

        m_backend->moveWires();
    }
    event->accept();
}

void Casing::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_moving)
    {
        m_corkboard->finishMoveSelection();
        m_backend->moveWires();
        m_moving = false;
        m_backend->setEmbeddedQMLEnabled(true);
    }
    if(m_resizing)
    {
        QPointF displacement = (event->scenePosition() - initialClickPos) / corkboard()->scale();
        QQuickItem* embed = m_backend->getEmbeddedQML();
        embed->setWidth(initialSize.x() + displacement.x());
        embed->setHeight(initialSize.y() + displacement.y());

        m_resizing = false;
        m_corkboard->reportSizeChanged(casingBackend()->id(), initialSize, QPointF(embed->width(),embed->height()));
    }
    event->accept();
}

void Casing::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(m_backend->getEmbeddedQMLLoaded())
        m_backend->getEmbeddedQML()->setFocus(true);
    else if(m_backend->getMinimizedQMLLoaded())
        m_backend->getMinimizedQML()->setFocus(true);

    event->accept();
}

void Casing::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete)
        m_corkboard->deleteFromCasing(this);
    else
        m_corkboard->keyPressEvent(event);
}

bool Casing::selected() const
{
    return m_selected;
}

void Casing::setSelected(bool selected)
{
    if (m_selected == selected)
        return;

    m_selected = selected;
    emit selectedChanged(m_selected);

    update();
}

void Casing::setFeedbackColor(QColor feedbackColor)
{
    if (m_feedbackColor == feedbackColor)
        return;

    m_feedbackColor = feedbackColor;
    emit feedbackColorChanged(m_feedbackColor);
}

void Casing::setFeedbackMessage(QString feedbackMessage)
{
    if (m_feedbackMessage == feedbackMessage)
        return;

    m_feedbackMessage = feedbackMessage;
    emit feedbackMessageChanged(m_feedbackMessage);
}

void Casing::setInLabelWidth(int inLabelWidth)
{
    if (m_inLabelWidth == inLabelWidth)
        return;

    m_inLabelWidth = inLabelWidth;
    emit inLabelWidthChanged(m_inLabelWidth);
}

void Casing::setOutLabelWidth(int outLabelWidth)
{
    if (m_outLabelWidth == outLabelWidth)
        return;

    m_outLabelWidth = outLabelWidth;
    emit outLabelWidthChanged(m_outLabelWidth);
}

void Casing::setFeedbackBoxTop(int feedbackBoxTop)
{
    if (m_feedbackBoxTop == feedbackBoxTop)
        return;

    m_feedbackBoxTop = feedbackBoxTop;
    emit feedbackBoxTopChanged(m_feedbackBoxTop);
}

void Casing::setInPlugBoxTop(int inPlugBoxTop)
{
    if (m_inPlugBoxTop == inPlugBoxTop)
        return;

    m_inPlugBoxTop = inPlugBoxTop;
    emit inPlugBoxTopChanged(m_inPlugBoxTop);
}

void Casing::setOutPlugBoxTop(int outPlugBoxTop)
{
    if (m_outPlugBoxTop == outPlugBoxTop)
        return;

    m_outPlugBoxTop = outPlugBoxTop;
    emit outPlugBoxTopChanged(m_outPlugBoxTop);
}

QString Casing::ideaName() const
{
    return m_ideaName;
}

QColor Casing::feedbackColor() const
{
    return m_feedbackColor;
}

QString Casing::feedbackMessage() const
{
    return m_feedbackMessage;
}

int Casing::inLabelWidth() const
{
    return m_inLabelWidth;
}

int Casing::outLabelWidth() const
{
    return m_outLabelWidth;
}

int Casing::feedbackBoxTop() const
{
    return m_feedbackBoxTop;
}

bool Casing::embeddedQML() const
{
    return m_hasEmbeddedQML;
}

int Casing::inPlugBoxTop() const
{
    return m_inPlugBoxTop;
}

int Casing::outPlugBoxTop() const
{
    return m_outPlugBoxTop;
}

bool Casing::resizeable() const
{
    return m_resizeable;
}

bool Casing::minimizable() const
{
    return m_minimizable;
}

bool Casing::minimized() const
{
    return m_minimized;
}

bool Casing::hasExtraQML() const
{
    return m_hasExtraQML;
}

void Casing::setInPlugColor(int plug, QColor color)
{
    m_inPlugLabelColors[plug] = color;
    m_backend->idea()->setInPlugLabelColor(plug, color);
    emit inPlugLabelColorsChanged(m_inPlugLabelColors);
}

void Casing::setInPlugToDefaultColor(int plug)
{
    QColor color = LabelStyle::defaultColor();
    setInPlugColor(plug, color);
}

void Casing::setOutPlugColor(int plug, QColor color)
{
    m_outPlugLabelColors[plug] = color;
    m_backend->idea()->setOutPlugLabelColor(plug, color);
    emit outPlugLabelColorsChanged(m_outPlugLabelColors);
}

QColor Casing::getOutPlugColor(int plug)
{
    return m_outPlugLabelColors.at(plug);
}

void Casing::setInPlugLabelsColor(QList<QColor> inPlugLabelColors)
{
    if(m_inPlugLabelColors == inPlugLabelColors)
        return;

    m_inPlugLabelColors = inPlugLabelColors;
    emit inPlugLabelColorsChanged(m_inPlugLabelColors);
}

void Casing::setOutPlugLabelsColor(QList<QColor> outPlugLabelColors)
{
    if(m_outPlugLabelColors == outPlugLabelColors)
        return;

    m_outPlugLabelColors = outPlugLabelColors;
    emit outPlugLabelColorsChanged(m_outPlugLabelColors);
}

QList<QColor> Casing::inPlugLabelColors() const
{
    return m_inPlugLabelColors;
}

QList<QColor> Casing::outPlugLabelColors() const
{
    return m_outPlugLabelColors;
}

QSize Casing::casingSize() const
{
    return m_casingSize;
}

QSize Casing::nameSize() const
{
    return m_nameSize;
}

QSize Casing::feedbackBoxSize() const
{
    return m_feedbackBoxSize;
}

QStringList Casing::inPlugLabels() const
{
    return m_inPlugLabels;
}

QStringList Casing::outPlugLabels() const
{
    return m_outPlugLabels;
}

void Casing::recalculateSize()
{
    CasingStyle style;

    int qmlHeight = 0;
    int qmlWidth = 0;
    int borderWidth = style.bordersWidth();

    if(m_backend->getEmbeddedQMLLoaded())
    {
        QQuickItem* qmlItem = m_backend->getEmbeddedQML();
        if(qmlItem)
        {
            qmlHeight = static_cast<int>(qmlItem->height() + 1);
            qmlWidth = static_cast<int>(qmlItem->width() + 1);
        }
    }
    else if(m_backend->getMinimizedQMLLoaded())
    {
        QQuickItem* qmlItem = m_backend->getMinimizedQML();
        if(qmlItem)
        {
            qmlHeight = static_cast<int>(qmlItem->height() + 1);
            qmlWidth = static_cast<int>(qmlItem->width() + 1);
        }
    }

    int casingHeight = borderWidth + 5;
    if(m_ideaName != QString())
        casingHeight += 5 + nameSize().height();

    m_QMLBoxY = casingHeight;

    int inPlugHeight = style.plugSize()*m_inPlugLabels.length() + style.plugSpacing()*(m_inPlugLabels.length()-1);
    int outPlugHeight = style.plugSize()*m_outPlugLabels.length() + style.plugSpacing()*(m_outPlugLabels.length()-1);

    //Case for no in Plugs.
    inPlugHeight = qMax(inPlugHeight, 0);

    //Case for no out Plugs.
    outPlugHeight = qMax(outPlugHeight, 0);

    if(inPlugHeight >= outPlugHeight && inPlugHeight >= qmlHeight) // In plugs tallest
    {
        setInPlugBoxTop(casingHeight);
        m_QMLBoxHeight = inPlugHeight;
        setOutPlugBoxTop(static_cast<int>(casingHeight + 0.5 * (inPlugHeight - outPlugHeight)));
        casingHeight += inPlugHeight;
    }
    else if(outPlugHeight >= inPlugHeight && outPlugHeight >= qmlHeight) // out plugs tallest
    {
        setOutPlugBoxTop(casingHeight);
        m_QMLBoxHeight = outPlugHeight;
        setInPlugBoxTop(static_cast<int>(casingHeight + 0.5 * (outPlugHeight - inPlugHeight)));
        casingHeight += outPlugHeight;
    }
    else // QML tallest
    {
        m_QMLBoxHeight = qmlHeight;
        setInPlugBoxTop(casingHeight + static_cast<int>(0.5*(qmlHeight - inPlugHeight)));
        setOutPlugBoxTop(casingHeight + static_cast<int>(0.5*(qmlHeight - outPlugHeight)));
        casingHeight += qmlHeight;
    }

    casingHeight += 5;
    casingHeight += borderWidth;
    setFeedbackBoxTop(casingHeight);

    if(feedbackBoxSize().height() != 0)
        casingHeight += borderWidth + 2 * 5 + feedbackBoxSize().height();

    double totalCaptionWidth = nameSize().width() + 8 + 2*5;
    double totalFeedbackWidth = feedbackBoxSize().width() + 8 + 2*5;

    double totalPlugWidth = 0;

    if(m_inPlugs != 0)
        totalPlugWidth += 2 + 0.5* 15;
    else
        totalPlugWidth += 4;

    totalPlugWidth += 2;
    if(inLabelWidth() != 0)
        totalPlugWidth += 3 + inLabelWidth();

    m_QMLBoxX = totalPlugWidth;
    totalPlugWidth += qmlWidth;

    if(outLabelWidth() != 0)
        totalPlugWidth += 3 + outLabelWidth();

    if(m_outPlugs != 0)
        totalPlugWidth += 2 +0.5*15;
    else
        totalPlugWidth += 4;

    double casingWidth = qMax(totalCaptionWidth,totalFeedbackWidth);

    if(totalPlugWidth > casingWidth)
    {
        m_QMLBoxWidth = qmlWidth;
        setCasingSize(QSize(totalPlugWidth,casingHeight));
    }
    else
    {
        m_QMLBoxWidth = qmlWidth - totalPlugWidth + casingWidth;
        setCasingSize(QSize(casingWidth,casingHeight));
    }

    if(m_hasEmbeddedQML || m_hasMinimizedQML)
        repositionQML();

    m_backend->moveWires();
}

QPointF Casing::plugScenePosition(int plug, PlugType plugType) const
{
    CasingStyle style;

    float y = 0.2*15; // Top of Casing Rect.

    if(plugType == PlugType::IN)
        y += inPlugBoxTop();
    else
        y+= outPlugBoxTop();

    y += (style.plugSpacing() + style.plugSize()) * plug;
    y += style.plugSize()/2.0;

    double x;
    if(plugType == PlugType::OUT)
        x = casingSize().width() + 0.7* style.plugSize() - 2;
    else
        x = 0.7* style.plugSize() + 2;
    return QPointF(x, y);
}

int Casing::mouseOverPlug(PlugType plugType, QPointF mousePoint, double scale)
{
    if(plugType == PlugType::NA)
        return -1;

    CasingStyle style;
    const double searchDist2 = 1.5 * style.plugSize() * style.plugSize() * scale * scale;

    uint plugs;
    if(plugType == PlugType::IN)
        plugs = m_inPlugs;
    else
        plugs = m_outPlugs;

    int plug = -1;
    double closestDis = 999999.9;
    for(uint i = 0; i < plugs; i++)
    {
        //QVector2d? ehh. Point is fine. Points are vectors.
        QPointF plugPos = plugScenePosition(i, plugType) - mousePoint;
        double dist = QPointF::dotProduct(plugPos, plugPos);
        if (dist < searchDist2)
        {
            if(dist < closestDis)
            {
                plug = i;
                closestDis = dist;
            }
        }
    }
    return plug;
}

bool Casing::checkHitResizer(QPointF point) const
{
    QPointF resizerPosition = QPointF(0.7*15 + casingSize().width() - 7.0,
                                      0.2*15 + casingSize().height() - 7.0);

    QPointF p = resizerPosition - point;
    auto distance = QPointF::dotProduct(p,p);
    if(distance < 100)
        return true;
    else
        return false;
}

void Casing::repositionQML() const
{
    QQuickItem* qmlItem;
    if(m_backend->getEmbeddedQMLLoaded())
        qmlItem = m_backend->getEmbeddedQML();
    else if(m_backend->getMinimizedQMLLoaded())
        qmlItem = m_backend->getMinimizedQML();
    else
        return; //There is no guarentee that the QML is loaded before this method is called.

    int qmlHeight = static_cast<int>(qmlItem->height() + 0.5);
    int qmlWidth = static_cast<int>(qmlItem->width() + 0.5);

    int xPos = static_cast<int>(0.7*15) + m_QMLBoxX;
    if(m_QMLBoxWidth > qmlWidth)
        xPos += (m_QMLBoxWidth - qmlWidth) / 2;
    qmlItem->setX(xPos);

    int yPos = static_cast<int>(0.2*15) + m_QMLBoxY;
    if(m_QMLBoxHeight > qmlHeight)
        yPos += (m_QMLBoxHeight - qmlHeight) / 2;
    qmlItem->setY(yPos);
}

void Casing::minimizeButtonClicked()
{
    m_backend->onDeleteExternalQML();
    m_corkboard->reportIdeaMinized(m_backend->id(),m_minimized);
    setMinimized(!m_minimized);
}

void Casing::setMinimized(bool minimized)
{
    if (m_minimized == minimized)
        return;

    if(!m_minimizable)
        return;

    m_minimized = minimized;
    m_backend->idea()->setMinimized(m_minimized);

    emit minimizedChanged(m_minimized);
    if(minimized)
    {
        casingBackend()->deleteEmbeddedQML();
        if(m_hasMinimizedQML)
            m_backend->loadMinimizedQML();
    }
    else
    {
        if(m_hasMinimizedQML)
            m_backend->deleteMinimizedQML();
        casingBackend()->loadEmbeddedQML();
    }

    if(m_changeNameOnMinimized && !m_nameChanged)
        setupCaption();
    m_backend->idea()->resetDisplay();
    recalculateSize();
}

void Casing::extraButtonClicked()
{
    if(!m_backend->getExternalQMLLoaded())
        m_backend->loadExternalQML();
    else
        m_backend->onDeleteExternalQML();
}

void Casing::setHasExtraQML(bool hasExtraQML)
{
    if (m_hasExtraQML == hasExtraQML)
        return;

    m_hasExtraQML = hasExtraQML;
    emit hasExtraQMLChanged(m_hasExtraQML);
}

void Casing::setFeedbackBoxSize(QSize feedbackBoxSize)
{
    if (m_feedbackBoxSize == feedbackBoxSize)
        return;

    m_feedbackBoxSize = feedbackBoxSize;
    emit feedbackBoxSizeChanged(m_feedbackBoxSize);
}

void Casing::setCasingSize(QSize casingSize)
{
    if (m_casingSize == casingSize)
        return;

    m_casingSize = casingSize;
    emit casingSizeChanged(m_casingSize);
}

void Casing::setIdeaName(const QString &newIdeaName)
{
    if (m_ideaName == newIdeaName)
        return;

    m_ideaName = newIdeaName;
    emit ideaNameChanged(m_ideaName);
    m_nameChanged = true;

    QFont nameFont("verdana",20, QFont::Bold);
    nameFont.setBold(true);
    QFontMetrics nameFontMetrics(nameFont);
    m_nameSize = nameFontMetrics.size(Qt::TextShowMnemonic,newIdeaName);
    emit nameSizeChanged(m_nameSize);
}
