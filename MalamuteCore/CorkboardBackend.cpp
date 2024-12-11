#include "CorkboardBackend.h"

CorkboardBackend::CorkboardBackend(std::shared_ptr<IdeaRegistry> ideaRegistry, Corkboard *corkboard, QObject * parent):
    QObject(parent), m_ideaRegistry(std::move(ideaRegistry))
{
    m_corkboard = corkboard;
}

CorkboardBackend::~CorkboardBackend()
{

}

QJsonObject CorkboardBackend::save() const
{
    QJsonObject sceneJson;
    QJsonArray casingsJsonArray;

    for (auto const &pair : m_casingBackends)
        casingsJsonArray.append(pair.second->save());

    sceneJson["c"] = casingsJsonArray;

    QJsonArray wireJsonArray;
    for (auto const &pair : m_wireBackends)
    {
        QJsonObject wireJson = pair.second->save();
        if (!wireJson.isEmpty())
            wireJsonArray.append(wireJson);
    }
    sceneJson["w"] = wireJsonArray;

    return sceneJson;
}

QJsonObject CorkboardBackend::saveSelection() const
{
    QJsonObject selectionJson;
    QJsonArray casingsJsonArray;

    for (auto const & pair : m_casingBackends)
    {
        auto const &casingBackend = pair.second;
        if(casingBackend->casing().selected())
            casingsJsonArray.append(casingBackend->save());
    }
    selectionJson["c"] = casingsJsonArray;

    QJsonArray wiresJsonArray;
    for (auto const & pair : m_wireBackends)
    {
        auto const &wireBackend = pair.second;
        if(wireBackend->getWire()->selected())
        {
            QJsonObject wireJson = wireBackend->save();
            if (!wireJson.isEmpty())
                wiresJsonArray.append(wireJson);
        }
    }
    selectionJson["w"] = wiresJsonArray;

    return selectionJson;
}

void CorkboardBackend::load(QByteArray data) const
{
    QJsonObject const jsonDocument = QJsonDocument::fromJson(data).object();

    QJsonArray casingsJsonArray = jsonDocument["c"].toArray();
    for (QJsonValueRef casing : casingsJsonArray)
    {
        createIdea(casing.toObject());
    }

    QJsonArray wiresJsonArray = jsonDocument["w"].toArray();
    for (QJsonValueRef wire : wiresJsonArray)
    {
        createWire(wire.toObject());
    }
}

std::shared_ptr<WireBackend> CorkboardBackend::createWireBackend(PlugType connectedPlugType, CasingBackend* casingBackend, int plugNumber)
{
    auto wireBackend = std::make_shared<WireBackend>(connectedPlugType, casingBackend, plugNumber);
    m_wireBackends[wireBackend->id()] = wireBackend;

    return wireBackend;
}

std::shared_ptr<WireBackend> CorkboardBackend::createWireBackend(int id, CasingBackend* inCasingBackend, int inPlugNumber, CasingBackend* outCasingBackend, int outPlugNumber)
{
    auto wireBackend = std::make_shared<WireBackend>(id, inCasingBackend, inPlugNumber, outCasingBackend, outPlugNumber);

    m_wireBackends[wireBackend->id()] = wireBackend;

    inCasingBackend->addWire(PlugType::IN, inPlugNumber, *wireBackend);
    outCasingBackend->addWire(PlugType::OUT, outPlugNumber, *wireBackend);

    return wireBackend;
}

std::shared_ptr<WireBackend> CorkboardBackend::createWireBackend(QJsonObject const &wireBackendJson)
{
    int wireID = wireBackendJson["id"].toInt();

    int casingInId  = wireBackendJson["iid"].toInt();
    int casingOutId = wireBackendJson["oid"].toInt();

    int intIn  = wireBackendJson["ip"].toInt();
    int intOut = wireBackendJson["op"].toInt();

    auto casingIn  = m_casingBackends[casingInId].get();
    auto casingOut = m_casingBackends[casingOutId].get();

    std::shared_ptr<WireBackend> wire = createWireBackend(wireID, casingIn, intIn, casingOut, intOut);

    return wire;
}

void CorkboardBackend::removeWire(WireBackend* wireBackend)
{
    auto it = m_wireBackends.find(wireBackend->id());
    if (it != m_wireBackends.end())
    {
        wireBackend->removeFromCasings();
        m_wireBackends.erase(it);
    }
}

void CorkboardBackend::removeWire(int id)
{
    m_wireBackends[id]->removeFromCasings();
    m_wireBackends.erase(m_wireBackends.find(id));
}

CasingBackend *CorkboardBackend::createCasingBackend(std::unique_ptr<Idea> && idea, Casing *casing, QJsonObject const& json)
{
    auto casingBackend = std::make_unique<CasingBackend>(std::move(idea));
    casingBackend->setCasing(casing);
    casingBackend->load(json);

    auto casingBackendPtr = casingBackend.get();
    m_casingBackends[casingBackend->id()] = std::move(casingBackend);
    return casingBackendPtr;
}

QJsonArray CorkboardBackend::removeCasingBackend(int id)
{
    CasingBackend* casingBackend = m_casingBackends[id].get();

    QJsonArray wiresJsonArray;

    std::vector<std::list<WireBackend*>> inWires = casingBackend->getWiresByType(PlugType::IN);
    for(uint i = 0; i < inWires.size(); i++)
    {
        for(WireBackend* backend : inWires[i])
        {
            wiresJsonArray.append(backend->save());
            removeWire(backend);
        }
    }

    std::vector<std::list<WireBackend*>> outWires = casingBackend->getWiresByType(PlugType::OUT);
    for(uint i = 0; i < outWires.size(); i++)
    {
        for(WireBackend* backend : outWires[i])
        {
            wiresJsonArray.append(backend->save());
            removeWire(backend);
        }
    }

    m_casingBackends.erase(casingBackend->id());
    return wiresJsonArray;
}

Wire *CorkboardBackend::createWire(const QJsonObject &json) const
{
    return m_corkboard->createWireFront(json);
}

IdeaRegistry& CorkboardBackend::registry() const
{
    return *m_ideaRegistry;
}

void CorkboardBackend::setRegistry(std::shared_ptr<IdeaRegistry> registry)
{
    m_ideaRegistry = std::move(registry);
}

Casing *CorkboardBackend::createIdea(const QJsonObject &json) const
{
    return m_corkboard->createCasing(json);
}

CasingBackend *CorkboardBackend::getCasing(int id)
{
    return m_casingBackends[id].get();
}

WireBackend *CorkboardBackend::getWireBackend(int id)
{
    return m_wireBackends[id].get();
}

Corkboard *CorkboardBackend::corkboard() const
{
    return m_corkboard;
}
