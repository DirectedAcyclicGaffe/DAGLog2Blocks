#ifndef CASINGBACKEND_H
#define CASINGBACKEND_H

#include <memory>

#include <QObject>
#include <QJsonObject>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickItem>

#include "Plug.h"
#include "InheritedClasses/DataType.h"
#include "StopLight.h"

class WireBackend;
class Casing;
class Idea;

class CasingBackend: public QObject
{
    Q_OBJECT

public:
    CasingBackend(std::unique_ptr<Idea> && idea);
    ~CasingBackend() override;

    QJsonObject save() const;
    void load(QJsonObject const &json);

    void finishSetup(QJsonObject loadJson);

    int id() const;
    void moveWires() const;

    Casing & casing();

    void setCasing(Casing* casing);

    Idea* idea() const;

    // Wire control
    void addWire(PlugType plugType, int plugNumber, WireBackend& wireBackend);
    void removeWire(PlugType plugType, int plugNumber, int id);

    std::vector<std::list<WireBackend*>> & getWiresByType(PlugType);
    std::list<WireBackend*> getWiresAtPlug(PlugType plugType, int plugNumber) const;
    std::vector<int> wireIds(PlugType plugType);

    //Extra QML control.
    QQmlContext *getContext() const;
    void prepareContext(QQmlContext *value);

    void loadEmbeddedQML();
    void deleteEmbeddedQML();
    QQuickItem *getEmbeddedQML() const;

    void loadMinimizedQML();
    void deleteMinimizedQML();
    QQuickItem *getMinimizedQML() const;

    void loadExternalQML();
    std::vector<QQuickItem*> getExternalQML() const;

    void onDeleteExternalQML(int i = -1); /// Deletes an external QML FILE. default parameter removes all items.
    void setEmbeddedQMLEnabled(bool enabled);
    bool getEmbeddedQMLLoaded() const;
    bool getMinimizedQMLLoaded() const;
    bool getExternalQMLLoaded() const;

    //Gear to show things happening under the hood.
    void addGear();

    //Stoplights to show connection possible/impossible.
    void addStopLight(StopLight stopLight);
    void addRedLight(PlugType plugType, int plugNumber);
    void addGreenLight(PlugType plugType, int plugNumber);
    void clearStopLights();

public slots:

    /// Sends data to the underlying model.
    void sendDataToIdea(std::shared_ptr<DataType> dataType, int inPlug) const;

    /// Fetches data from model's OUT plug
    void onNewData(int plugNumber);
    void onClearDataAllPlugs();
    void sendDataNewWire(int id);

    /// Updates the information for the visual casing.
    void onStatusChanged();
    void onStartSpinningGear();
    void onStopSpinningGear();

    /// Registers a new Context Property.
    /// This allows for individual ideas to have there own context properties.
    /// Useful for model/view programming.
    void onSetContextProperty(QString name, QObject* obj);

    /// Reports the change in paramaters in one file to the undostack.
    void onReportParamsChanged(QJsonObject oldParams, QJsonObject newParams);

    /// Disconnect a bunch of plugs.
    void onDisconnectPlugs(std::vector<UnplugWiresInfo> unplugWires);

    void onInPlugLabelColorsChanged(QList<QColor> inPlugLabelColors);
    void onOutPlugLabelColorsChanged(QList<QColor> outPlugLabelColors);

private:
    int m_id;
    std::unique_ptr<Idea> m_idea;

    // The user visible stuff.
    Casing* m_casing;

    std::vector<std::list<WireBackend*>> m_inWires;
    std::vector<std::list<WireBackend*>> m_outWires;

    std::list<QQuickItem*> m_stopLights;
    std::vector<bool> m_inPlugHasStopLight;
    std::vector<bool> m_outPlugHasStopLight;

    // Each casing backend holds a qmlContext. This allows the casing to pass data between
    // the idea and the custom qml component.
    QQmlContext* m_context;

    QQuickItem* m_embeddedQML;
    QSizeF m_embeddedSizeFromLoad;
    bool m_embeddedQMLLoaded;

    QQuickItem* m_minimizedQML;
    bool m_minimizedQMLLoaded;

    std::vector<QQuickItem*> m_externalQML;
    bool m_externalQMLLoaded;

    QQuickItem* m_gear;
};

#endif // CASINGBACKEND_H
