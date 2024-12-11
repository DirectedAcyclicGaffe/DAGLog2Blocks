#include "CasingBackend.h"

#include <utility>

#include "Casing.h"
#include "InheritedClasses/Idea.h"
#include "Corkboard.h"
#include "WireBackend.h"
#include "Commands/DeleteSelectionCommand.h"

#include "IdMaker.h"

CasingBackend::CasingBackend(std::unique_ptr<Idea> && idea) :
    m_idea(std::move(idea)),
    m_casing(nullptr),
    m_embeddedSizeFromLoad(QSizeF(0.0,0.0)),
    m_embeddedQMLLoaded(false),
    m_minimizedQMLLoaded(false),
    m_externalQMLLoaded(false),
    m_gear(nullptr)
{
    m_externalQML.clear();
    m_id = Magic<IdMaker>::cast().getID();
}

CasingBackend::~CasingBackend()
{
    if(m_gear)
        delete m_gear;

    deleteEmbeddedQML();
    deleteMinimizedQML();

    for(size_t i = 0; i < m_externalQML.size(); i++)
        delete m_externalQML[i];

    delete m_casing;
}

QJsonObject CasingBackend::save() const
{
    QJsonObject casingJson;

    casingJson["id"] = m_id;
    casingJson["nm"] = m_idea->attributes().listName();
    casingJson["ia"] = m_idea->save();
    casingJson["cp"] = m_casing->save();
    if(m_embeddedQMLLoaded && m_casing->resizeable())
    {
        casingJson["ew"] = m_embeddedQML->width();
        casingJson["eh"] = m_embeddedQML->height();
    }

    return casingJson;
}

void CasingBackend::load(QJsonObject const& json)
{
    m_id = json["id"].toInt();
    if(m_casing)
        m_casing->load(json["cp"].toObject());

    QJsonValue ew = json["ew"];
    if(!ew.isUndefined())
        m_embeddedSizeFromLoad.setWidth(ew.toDouble());
    QJsonValue eh = json["eh"];
    if(!eh.isUndefined())
        m_embeddedSizeFromLoad.setHeight(eh.toDouble());
}

void CasingBackend::finishSetup(QJsonObject loadJson)
{
    //This class handles all the signals from the idea class.
    //The idea class shouldn't know or depend on any details of the casing class.

    //InterIdea Communication Handlers
    connect(m_idea.get(), &Idea::newData, this, &CasingBackend::onNewData);
    connect(m_idea.get(), &Idea::clearDataAllPlugs, this, &CasingBackend::onClearDataAllPlugs);
    connect(m_idea.get(), &Idea::inPlugLabelColorsChanged,
            this, &CasingBackend::onInPlugLabelColorsChanged);
    connect(m_idea.get(), &Idea::outPlugLabelColorsChanged,
            this, &CasingBackend::onOutPlugLabelColorsChanged);

    //Within Idea handlers
    connect(m_idea.get(), &Idea::statusChanged, this, &CasingBackend::onStatusChanged);
    connect(m_idea.get(), &Idea::reportParamsChanged,this,&CasingBackend::onReportParamsChanged);
    connect(m_idea.get(), &Idea::disconnectPlugs, this, &CasingBackend::onDisconnectPlugs);
    connect(m_idea.get(), &Idea::setContextProperty, this, &CasingBackend::onSetContextProperty);
    connect(m_idea.get(), &Idea::deleteExternalQML, this, &CasingBackend::onDeleteExternalQML);

    //Create and prepare a gear if there is one.
    if(m_idea->attributes().hasGear())
    {
        addGear();
        connect(m_idea.get(), &Idea::startSpinningGear, this, &CasingBackend::onStartSpinningGear);
        connect(m_idea.get(), &Idea::stopSpinningGear, this, &CasingBackend::onStopSpinningGear);
    }

    //Load the initial things from the idea. Finish setup of qml things if necessary.
    m_inWires.resize(idea()->attributes().dataTypesIn().size());
    m_outWires.resize(idea()->attributes().dataTypesOut().size());

    m_idea->load(loadJson);
    m_idea->finishSetupQmlContext();
}

int CasingBackend::id() const
{
    return m_id;
}

std::vector<std::list<WireBackend*>> &CasingBackend::getWiresByType(PlugType plugType)
{
    if (plugType == PlugType::IN)
        return m_inWires;
    else
        return m_outWires;
}

std::list<WireBackend *> CasingBackend::getWiresAtPlug(PlugType plugType, int plugNumber) const
{
    if(plugType == PlugType::IN)
        return m_inWires[static_cast<uint>(plugNumber)];
    else
        return m_outWires[static_cast<uint>(plugNumber)];
}

std::vector<int> CasingBackend::wireIds(PlugType plugType)
{
    std::vector<int> ids;

    if(plugType == PlugType::IN)
    {
        for(ulong i = 0; i < m_inWires.size(); i++)
        {
            for(auto wire : m_inWires[i])
                ids.push_back(wire->id());
        }
    }
    else
    {
        for(ulong i = 0; i < m_outWires.size(); i++)
        {
            for(auto wire : m_outWires[i])
                ids.push_back(wire->id());
        }
    }
    return ids;
}

void CasingBackend::addWire(PlugType plugType, int plugNumber, WireBackend& wireBackend)
{
    if(plugType == PlugType::IN)
        m_inWires.at(static_cast<uint>(plugNumber)).push_back(&wireBackend);
    else
        m_outWires.at(static_cast<uint>(plugNumber)).push_back(&wireBackend);
}

void CasingBackend::removeWire(PlugType plugType, int plugNumber, int id)
{
    if(plugType == PlugType::IN)
    {
        for(WireBackend* wireBackend : m_inWires[plugNumber])
        {
            if(wireBackend->id() == id)
            {
                m_inWires[plugNumber].remove(wireBackend);
                break;
            }
        }
    }
    else if(plugType == PlugType::OUT)
    {
        for(WireBackend* wireBackend : m_outWires[plugNumber])
        {
            if(wireBackend->id() == id)
            {
                m_outWires[plugNumber].remove(wireBackend);
                break;
            }
        }
    }
}

void CasingBackend::moveWires() const
{
    for(uint i = 0; i < m_inWires.size(); i++)
    {
        for(auto & wire : m_inWires[i])
            wire->getWire()->move();
    }
    for(uint i = 0; i < m_outWires.size(); i++)
    {
        for(auto & wire : m_outWires[i])
            wire->getWire()->move();
    }
}

Casing & CasingBackend::casing()
{
    return *m_casing;
}

void CasingBackend::setCasing(Casing *casing)
{
    m_casing = casing;
}

Idea* CasingBackend::idea() const
{
    return m_idea.get();
}

void CasingBackend::sendDataToIdea(std::shared_ptr<DataType> dataType, int inPlug) const
{
    m_idea->onNewDataIn(std::move(dataType), inPlug);
    m_casing->recalculateSize();
    m_casing->update();
}

void CasingBackend::onNewData(int plug)
{
    IdeaStatus status = m_idea->status();

    // Boxes should not send data in standby error or fatal states.
    if(status == IdeaStatus::StandBy || status == IdeaStatus::Error || status == IdeaStatus::Fatal)
        return;

    auto ideaData = m_idea->dataOut(plug);
    auto leWires = getWiresAtPlug(PlugType::OUT, plug);
    for (auto const & c : leWires)
    {
        c->sendData(ideaData);
        c->getWire()->setTransmitting(true);
    }
}

void CasingBackend::onClearDataAllPlugs()
{
    IdeaStatus status = m_idea->status();

    // Never silently clear the output. That cost me way to much time figuring out a bug.
    if(status == IdeaStatus::Working || status == IdeaStatus::Warning || status == IdeaStatus::DisplayWarning)
    {
        qFatal("Clear data called on valid idea. This is super confusing for the user. Don't do it");
        return;
    }

    for (ulong i = 0; i < m_outWires.size(); i++)
    {
        for(auto const & c: m_outWires[i])
            c->sendNullData();
    }
}

void CasingBackend::sendDataNewWire(int id)
{
    IdeaStatus status = m_idea->status();
    // Boxes only send data when they are working or warning.
    if(status == IdeaStatus::StandBy || status == IdeaStatus::Error || status == IdeaStatus::Fatal)
    {
        qFatal("Send data called on idea in standby error or fatal mode. Not allowed");
        return;
    }
    for (ulong i = 0; i < m_outWires.size(); i++)
    {
        for(auto const & c: m_outWires[i])
        {
            if(c->id() == id)
            {
                auto ideaData = m_idea->dataOut(static_cast<int>(i));
                c->sendData(ideaData);
                return;
            }
        }
    }
    qFatal("Send data called on invalid ID");
}

void CasingBackend::onStatusChanged()
{
    if(m_casing)
        m_casing->setupFeedbackBox();

    //The backend is created before the casing.
    //Sometimes the constructor or restore function of the idea sends instructions to the casing
    //before the casing is complete. The error checking here slows things down a tiny amount to ensure
    //that coding is easier.
}

void CasingBackend::onStartSpinningGear()
{
    m_gear->setProperty("rotating", true);
}

void CasingBackend::onStopSpinningGear()
{
    m_gear->setProperty("rotating", false);
}

void CasingBackend::onSetContextProperty(QString name, QObject *obj)
{
    m_context->setContextProperty(name, obj);
}

void CasingBackend::onReportParamsChanged(QJsonObject oldParams, QJsonObject newParams)
{
    m_casing->corkboard()->reportParamsChanged(id(),oldParams, newParams);
}

void CasingBackend::onDisconnectPlugs(std::vector<UnplugWiresInfo> unplugWires)
{
    QJsonObject wiresToDelete;
    QJsonArray wiresJsonArray;

    for(size_t i = 0; i < unplugWires.size(); i++)
    {
        auto wires = getWiresAtPlug(unplugWires.at(0).isInput(), unplugWires.at(0).plugNumber());

        for(const auto& wire: wires)
            wiresJsonArray.append(wire->save());
    }
    wiresToDelete["w"] = wiresJsonArray;
    DeleteSelectionCommand* command = new DeleteSelectionCommand(wiresToDelete, m_casing->corkboard()->backend());
    m_casing->corkboard()->undoStack()->push(command);
}

void CasingBackend::onInPlugLabelColorsChanged(QList<QColor> inPlugLabelColors)
{
    m_casing->setInPlugLabelsColor(inPlugLabelColors);
}

void CasingBackend::onOutPlugLabelColorsChanged(QList<QColor> outPlugLabelColors)
{
    m_casing->setOutPlugLabelsColor(outPlugLabelColors);
}

bool CasingBackend::getExternalQMLLoaded() const
{
    return m_externalQMLLoaded;
}

bool CasingBackend::getMinimizedQMLLoaded() const
{
    return m_minimizedQMLLoaded;
}

bool CasingBackend::getEmbeddedQMLLoaded() const
{
    return m_embeddedQMLLoaded;
}

QQmlContext *CasingBackend::getContext() const
{
    return m_context;
}

void CasingBackend::prepareContext(QQmlContext *value)
{
    m_context = value;
    m_context->setContextProperty("idea",m_idea.get());
}

QQuickItem *CasingBackend::getEmbeddedQML() const
{
    return m_embeddedQML;
}

void CasingBackend::loadEmbeddedQML()
{
    QString path = idea()->attributes().embeddedQMLPath();
    QQmlComponent qmlcomponent(m_casing->corkboard()->qmlEngine(),QUrl::fromLocalFile(path));

    QObject* preEmbeddedQML = qmlcomponent.beginCreate(m_context);

    //To Make the qml component visible, it must have a parent.
    //First we cast it to be a QQuickItem. Then we set the parent to the be qml.
    // This ensures it stays with the qml.
    m_embeddedQML = qobject_cast<QQuickItem*>(preEmbeddedQML);
    if(m_embeddedQML)
    {
        m_embeddedQML->setParentItem(m_casing);
        if(m_embeddedSizeFromLoad.height() > 0.0 && m_embeddedSizeFromLoad.width() > 0.0)
            m_embeddedQML->setSize(m_embeddedSizeFromLoad);

        // This ensures that resizing the embedded qml works.
        connect(m_embeddedQML, &QQuickItem::widthChanged, m_casing, &Casing::recalculateSize);
        connect(m_embeddedQML, &QQuickItem::heightChanged, m_casing, &Casing::recalculateSize);
        m_embeddedQMLLoaded = true;
        qmlcomponent.completeCreate();
    }
}

void CasingBackend::deleteEmbeddedQML()
{
    if(m_embeddedQMLLoaded)
    {
        m_embeddedQMLLoaded = false;
        disconnect(m_embeddedQML, &QQuickItem::widthChanged, m_casing, &Casing::recalculateSize);
        disconnect(m_embeddedQML, &QQuickItem::heightChanged, m_casing, &Casing::recalculateSize);
        delete m_embeddedQML;
    }
}

QQuickItem *CasingBackend::getMinimizedQML() const
{
    return m_minimizedQML;
}

void CasingBackend::loadMinimizedQML()
{
    QString path = idea()->attributes().minimizedQMLPath();
    QQmlComponent qmlcomponent(m_casing->corkboard()->qmlEngine(),QUrl::fromLocalFile(path));

    QObject* preMinimizedQML = qmlcomponent.beginCreate(m_context);

    //To Make the qml component visible, it must have a parent.
    //First we cast it to be a QQuickItem. Then we set the parent to the be qml.
    // This ensures it stays with the qml.
    m_minimizedQML = qobject_cast<QQuickItem*>(preMinimizedQML);
    if(m_minimizedQML)
    {
        m_minimizedQML->setParentItem(m_casing);
        m_minimizedQMLLoaded = true;
        qmlcomponent.completeCreate();
    }
}

void CasingBackend::deleteMinimizedQML()
{
    if(m_minimizedQMLLoaded)
    {
        m_minimizedQMLLoaded = false;
        delete m_minimizedQML;
    }
}

void CasingBackend::addStopLight(StopLight stopLight)
{
    QUrl url(QStringLiteral("qrc:/QML/StopLight.qml"));
    QQmlComponent component(m_casing->corkboard()->qmlEngine(),url);

    QObject* preStopLight = component.create(m_context);

    //Find the y coordinate.
    QQuickItem* qmlStopLight = qobject_cast<QQuickItem*>(preStopLight);
    if(qmlStopLight)
    {
        qmlStopLight->setProperty("stopLightColor",stopLight.m_color);
        QPointF position = m_casing->plugScenePosition(stopLight.m_int, stopLight.m_plugType);
        qmlStopLight->setPosition(position - QPointF(11,11));
        qmlStopLight->setParentItem(m_casing);
    }

    m_stopLights.push_back(qmlStopLight);
}

void CasingBackend::addRedLight(PlugType plugType, int plugNumber)
{
    StopLight stopLight(plugType,plugNumber,QColor(0xb31212));
    if(plugType == PlugType::IN)
    {
        if(!m_inPlugHasStopLight[plugNumber])
        {
            m_inPlugHasStopLight[plugNumber] = true;
            addStopLight(stopLight);
        }
        return;
    }
    if(plugType == PlugType::OUT)
    {
        if(!m_outPlugHasStopLight[plugNumber])
        {
            m_outPlugHasStopLight[plugNumber] = true;
            addStopLight(stopLight);
        }
        return;
    }
    return;
}

void CasingBackend::addGreenLight(PlugType plugType, int plugNumber)
{
    StopLight stopLight(plugType,plugNumber,QColor(0x12b33c));

    if(plugType == PlugType::IN)
    {
        if(!m_inPlugHasStopLight[plugNumber])
        {
            m_inPlugHasStopLight[plugNumber] = true;
            addStopLight(stopLight);
        }
        return;
    }
    if(plugType == PlugType::OUT)
    {
        if(!m_outPlugHasStopLight[plugNumber])
        {
            m_outPlugHasStopLight[plugNumber] = true;
            addStopLight(stopLight);
        }
        return;
    }
    return;
}

void CasingBackend::clearStopLights()
{
    for(QQuickItem* stopLight : m_stopLights)
        delete stopLight;


    m_inPlugHasStopLight.clear();
    m_outPlugHasStopLight.clear();

    for(int i = 0; i < m_casing->inPlugLabels().length(); i++)
        m_inPlugHasStopLight.push_back(false);
    for(int i = 0; i < m_casing->outPlugLabels().length(); i++)
        m_outPlugHasStopLight.push_back(false);

    m_stopLights.clear();
}

std::vector<QQuickItem *> CasingBackend::getExternalQML() const
{
    return m_externalQML;
}

void CasingBackend::loadExternalQML()
{
    IdeaAttributes attributes = m_idea.get()->attributes();
    m_casing->setZ(10);

    if(m_externalQML.size() > 0)
        return; //The External QML has already been loaded
    else
    {
        m_externalQMLLoaded = true;
        for(int i = 0; i < attributes.externalQMLPaths().size(); i++)
        {
            QString path = attributes.externalQMLPaths().at(i);
            double x = attributes.externalQMLLocations()[i].extraXOffset();
            if(attributes.externalQMLLocations()[i].offsetByWidth())
                x += casing().width();

            double y = attributes.externalQMLLocations()[i].extraYOffset();
            if(attributes.externalQMLLocations()[i].offsetByHeight())
                y+= casing().height();

            QQmlComponent qmlComponent(m_casing->corkboard()->qmlEngine(),QUrl::fromLocalFile(path));
            QObject* preExternalQML = qmlComponent.create(m_context);
            m_externalQML.push_back(qobject_cast<QQuickItem*>(preExternalQML));
            m_externalQML[i]->setParentItem(m_casing);
            m_externalQML[i]->setPosition(QPointF(x,y));
        }
    }
}

void CasingBackend::onDeleteExternalQML(int i)
{
    m_casing->setZ(3.0);

    if(i == -1)
    {
        for(size_t j = 0; j < m_externalQML.size(); j++)
            m_externalQML[j]->deleteLater();
        m_externalQML.clear();
        m_externalQMLLoaded = false;
        return;
    }
    m_externalQML[i]->deleteLater();
    m_externalQML.erase(m_externalQML.begin() + i);
}

void CasingBackend::setEmbeddedQMLEnabled(bool enabled)
{
    if(m_embeddedQMLLoaded)
    {
        m_embeddedQML->setEnabled(enabled);
    }
}

void CasingBackend::addGear()
{
    QUrl url(QStringLiteral("qrc:/QML/CasingGear.qml"));
    QQmlComponent qqmlcomponent(m_casing->corkboard()->qmlEngine(),url);

    QObject* preGear = qqmlcomponent.beginCreate(m_context);
    if(!preGear)
        qFatal("Failed to create qml object");

    m_gear = qobject_cast<QQuickItem*>(preGear);
    if(m_gear)
    {
        m_gear->setParentItem(m_casing);
        qqmlcomponent.completeCreate();
    }
}
