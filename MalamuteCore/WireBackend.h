#ifndef WIREBACKEND_H
#define WIREBACKEND_H

#include <memory>
#include <unordered_set>

#include <QObject>
#include <QVariant>

#include "Plug.h"
#include "InheritedClasses/DataType.h"
#include "CasingBackend.h"
#include "Wire.h"

class WireBackend: public QObject
{
    Q_OBJECT

public:
    WireBackend(PlugType plugType, CasingBackend* casingBackend, int plugNumber);
    WireBackend(int id, CasingBackend *inCasingBackend, int inPlugNumber, CasingBackend *outCasingBackend, int outPlugNumber);

    ~WireBackend();

    QJsonObject save() const;

    int id() const;

    void setRequiredPlugType(PlugType plugType);
    PlugType requiredPlugType() const;

    void setWire(Wire* wire);

    /// Assigns a casing to the required plug.
    /// It is assumed that there is a required plug, no extra checks
    void setCasingToPlug(CasingBackend *casingBackend, PlugType plugType, int plugNumber);
    void removeFromCasings() const;
    Wire *getWire() const;

    CasingBackend* getCasing(PlugType plugType) const;
    CasingBackend*& getCasing(PlugType plugType);

    int getPlug(PlugType plugType) const;

    void clearCasingBackend(PlugType plugType);

    int dataType(PlugType plugType) const;

    bool plugedIn() const;

    void sendData(std::shared_ptr<DataType> dataType) const;
    void sendNullData() const;

    void setPlugTypeNeeded(PlugType plugType);
    PlugType plugTypeNeeded() const;

    void resetStopLights();
    void casingHasStopLights(CasingBackend* casingBackend);

signals:
    void wireBackendCompleted(WireBackend const&);

private:
    int m_id;
    Wire* m_wire;

    PlugType m_plugTypeNeeded = PlugType::NA;

    CasingBackend* m_outCasingBackend = nullptr;
    CasingBackend* m_inCasingBackend  = nullptr;

    int m_outPlugNumber;
    int m_inPlugNumber;

    std::list<CasingBackend*> m_casingWithStopLights;
};

#endif //WIREBACKEND_H
