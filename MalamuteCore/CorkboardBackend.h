#ifndef CORKBOARDBACKEND_H
#define CORKBOARDBACKEND_H

#include <memory>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "Registries/IdeaRegistry.h"
#include "CasingBackend.h"
#include "Corkboard.h"

class CorkboardBackend: public QObject
{
    Q_OBJECT

public:
    CorkboardBackend(std::shared_ptr<IdeaRegistry> ideaRegistry, Corkboard* corkboard, QObject * parent = nullptr);
    ~CorkboardBackend();

    QJsonObject save() const;
    QJsonObject saveSelection() const;
    void load(QByteArray data) const;

    IdeaRegistry&registry() const;
    void setRegistry(std::shared_ptr<IdeaRegistry> registry);

    Casing* createIdea(const QJsonObject &json) const;
    CasingBackend* createCasingBackend(std::unique_ptr<Idea> && idea, Casing *casing, QJsonObject const& json);
    QJsonArray removeCasingBackend(int id);

    Wire* createWire(const QJsonObject & json) const;
    std::shared_ptr<WireBackend> createWireBackend(PlugType connectedPlugType, CasingBackend *casingBackend, int plugNumber);
    std::shared_ptr<WireBackend> createWireBackend(int id, CasingBackend* inCasingBackend, int inPlugNumber, CasingBackend* outCasingBackend, int outPlugNumber);
    std::shared_ptr<WireBackend> createWireBackend(QJsonObject const &wireJson);

    void removeWire(WireBackend *wireBackend);
    void removeWire(int id);

    CasingBackend* getCasing(int id);
    WireBackend* getWireBackend(int id);

    Corkboard *corkboard() const;

private:
    Corkboard* m_corkboard;
    std::map<int, std::shared_ptr<WireBackend>> m_wireBackends;
    std::map<int, std::unique_ptr<CasingBackend>> m_casingBackends;
    std::shared_ptr<IdeaRegistry> m_ideaRegistry;
};

#endif

