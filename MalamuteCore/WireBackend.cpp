#include "WireBackend.h"
#include "IdMaker.h"
#include "MalamuteCore/InheritedClasses/Idea.h"

WireBackend::WireBackend(PlugType plugType, CasingBackend* casingBackend, int plugNumber) :
    m_outPlugNumber(-1),
    m_inPlugNumber(-1)
{
    m_id = Magic<IdMaker>::cast().getID();
    setCasingToPlug(casingBackend, plugType, plugNumber);

    auto requiredType = PlugType::NA;
    if(plugType == PlugType::IN)
        requiredType = PlugType::OUT;
    else if (plugType == PlugType::OUT)
        requiredType = PlugType::IN;
    setRequiredPlugType(requiredType);
}

WireBackend::WireBackend(int id, CasingBackend* inCasingBackend, int inint,
                         CasingBackend* outCasingBackend, int outint) :
    m_outCasingBackend(outCasingBackend),
    m_inCasingBackend(inCasingBackend),
    m_outPlugNumber(outint),
    m_inPlugNumber(inint)
{
    m_id = id;
    setCasingToPlug(inCasingBackend, PlugType::IN, inint);
    setCasingToPlug(outCasingBackend, PlugType::OUT, outint);
}

WireBackend::~WireBackend()
{
    if (plugedIn())
        sendNullData();

    m_wire->deleteLater();
}

QJsonObject WireBackend::save() const
{
    QJsonObject wireJson;

    if (m_inCasingBackend && m_outCasingBackend)
    {
        wireJson["id"] = m_id;
        wireJson["iid"] = m_inCasingBackend->id();
        wireJson["ip"] = m_inPlugNumber;
        wireJson["oid"] = m_outCasingBackend->id();
        wireJson["op"] = m_outPlugNumber;
    }
    return wireJson;
}


int WireBackend::id() const
{
    return m_id;
}


bool WireBackend::plugedIn() const
{
    return m_inCasingBackend != nullptr && m_outCasingBackend != nullptr;
}


void WireBackend::setRequiredPlugType(PlugType plugType)
{
    setPlugTypeNeeded(plugType);

    if(plugType == PlugType::OUT)
    {
        m_outCasingBackend = nullptr;
        m_outPlugNumber = -1;
    }
    if(plugType == PlugType::IN)
    {
        m_inCasingBackend = nullptr;
        m_inPlugNumber = -1;
    }
}

PlugType WireBackend::requiredPlugType() const
{
    return m_plugTypeNeeded;
}

void WireBackend::setWire(Wire *wire)
{
    m_wire = wire;
}

int WireBackend::getPlug(PlugType plugType) const
{
    if(plugType == PlugType::IN)
        return m_inPlugNumber;
    else if(plugType == PlugType::OUT)
        return m_outPlugNumber;
    else
        return -1;
}

void WireBackend::setCasingToPlug(CasingBackend* casingBackend, PlugType plugType, int plugNumber)
{
    bool wasIncomplete = !plugedIn();
    auto& weakCasingBackend = getCasing(plugType);
    weakCasingBackend = casingBackend;
    
    if (plugType == PlugType::OUT)
        m_outPlugNumber = plugNumber;
    else
        m_inPlugNumber = plugNumber;

    m_plugTypeNeeded = PlugType::NA;

    if (plugedIn() && wasIncomplete)
        wireBackendCompleted(*this);
}

void WireBackend::removeFromCasings() const
{
    if (m_inCasingBackend)
        m_inCasingBackend->removeWire(PlugType::IN, m_inPlugNumber, id());

    if (m_outCasingBackend)
        m_outCasingBackend->removeWire(PlugType::OUT, m_outPlugNumber, id());
}

Wire *WireBackend::getWire() const
{
    return m_wire;
}

CasingBackend* WireBackend::getCasing(PlugType plugType) const
{
    if(plugType == PlugType::IN)
        return m_inCasingBackend;
    else
        return m_outCasingBackend;
}

CasingBackend*& WireBackend::getCasing(PlugType plugType)
{
    if(plugType == PlugType::IN)
        return m_inCasingBackend;
    else
        return m_outCasingBackend;

}

void WireBackend::clearCasingBackend(PlugType plugType)
{
    getCasing(plugType) = nullptr;
    if (plugType == PlugType::IN)
        m_inPlugNumber = -1;
    else
        m_outPlugNumber = -1;
}

int WireBackend::dataType(PlugType plugType) const
{
    if (m_inCasingBackend && m_outCasingBackend)
    {
        auto const & idea = (plugType == PlugType::IN) ? m_inCasingBackend->idea() : m_outCasingBackend->idea();
        int plug = (plugType == PlugType::IN) ? m_inPlugNumber : m_outPlugNumber;

        int candidateCasingDataType;
        if(plugType == PlugType::IN)
            candidateCasingDataType = idea->attributes().dataTypesIn()[plug];
        else
            candidateCasingDataType = idea->attributes().dataTypesOut()[plug];

        return candidateCasingDataType;
    }
    else
    {
        CasingBackend* validCasingBackend;
        int plug = -1;

        if ((validCasingBackend = m_inCasingBackend))
        {
            plug = m_inPlugNumber;
            plugType = PlugType::IN;
        }
        else if ((validCasingBackend = m_outCasingBackend))
        {
            plug = m_outPlugNumber;
            plugType = PlugType::OUT;
        }

        if (validCasingBackend)
        {
            auto const &idea = validCasingBackend->idea();

            int candidateCasingDataType;
            if(plugType == PlugType::IN)
                candidateCasingDataType = idea->attributes().dataTypesIn()[plug];
            else
                candidateCasingDataType = idea->attributes().dataTypesOut()[plug];

            return candidateCasingDataType;
        }
    }

    qFatal("Invalid datatype in WireBackend");
}

void WireBackend::sendData(std::shared_ptr<DataType> dataType) const
{
    if(m_inCasingBackend)
        m_inCasingBackend->sendDataToIdea(dataType, m_inPlugNumber);
}

void WireBackend::sendNullData() const
{
    std::shared_ptr<DataType> nullData;
    sendData(nullData);
}

void WireBackend::setPlugTypeNeeded(PlugType plugType)
{
    m_plugTypeNeeded = plugType;
}

PlugType WireBackend::plugTypeNeeded() const
{
    return m_plugTypeNeeded;
}

void WireBackend::resetStopLights()
{
    for (CasingBackend * casingBackend : m_casingWithStopLights)
        casingBackend->clearStopLights();

    m_casingWithStopLights.clear();
}

void WireBackend::casingHasStopLights(CasingBackend *casingBackend)
{
    m_casingWithStopLights.push_back(casingBackend);
}
