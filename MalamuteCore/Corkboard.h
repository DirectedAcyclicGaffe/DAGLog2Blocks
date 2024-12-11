#ifndef CORKBOARD_H
#define CORKBOARD_H

#include <QQuickItem>
#include <QQmlContext>
#include <QMimeData>
#include <QClipboard>
#include <QGuiApplication>
#include <QCursor>
#include "Commands/UndoStack.h"

#include "Casing.h"
#include "Wire.h"
#include "CasingBackend.h"
#include "Registries/IdeaRegistry.h"
#include "Registries/DataTypeRegistry.h"
#include "WireBackend.h"


const QString corkboardOperationsMimeType = "application/malamuteCasing";
const QString casingStateMimeType = "application/malamuteCasingState";

class CorkboardBackend;

class Corkboard : public QQuickItem
{
    Q_OBJECT

    //Used to draw selection rectangle.
    Q_PROPERTY(bool selecting READ selecting WRITE setSelecting NOTIFY selectingChanged)
    Q_PROPERTY(QRectF selectingBox READ selectingBox WRITE setSelectingBox NOTIFY selectingBoxChanged)

    //Controls the menu with undo/redo cut copy paste etc.
    Q_PROPERTY(bool contextMenuActive READ contextMenuActive WRITE setContextMenuActive NOTIFY contextMenuActiveChanged)
    Q_PROPERTY(QPointF contextMenuPt READ contextMenuPt WRITE setContextMenuPt NOTIFY contextMenuPtChanged)

    //Controls the menu to interact with casing things.
    Q_PROPERTY(bool casingMenuActive READ casingMenuActive WRITE setCasingMenuActive NOTIFY casingMenuActiveChanged)
    Q_PROPERTY(QPointF casingMenuPt READ casingMenuPt WRITE setCasingMenuPt NOTIFY casingMenuPtChanged)

    Q_PROPERTY(int headerHeight READ headerHeight WRITE setHeaderHeight NOTIFY headerHeightChanged)

public:
    explicit Corkboard(QQuickItem *parent = nullptr);

    CorkboardBackend* backend() const;
    void createBackend(std::shared_ptr<IdeaRegistry> ideaRegistry);

    /// Creates an idea based on a specific json;
    void createIdea(const QJsonObject &jsonObj);
    /// Creates an idea based on a specific name and point.
    /// Uses createIdea(QJsonObject) behind the scenes.
    Q_INVOKABLE void createIdea(QString name, QPointF point = QPointF());

    Casing* createCasing(const QJsonObject &json);

    //Used to create wires.
    Wire* createWire(WireBackend *wireBackend);

    /// Call this method from the command classes. Calling this method can break the undo/redo stack
    /// and cause unexpected crashes. I'd avoid using it.
    Wire* createWireFront(const QJsonObject & jsonObj);

    void shiftWire(const QJsonObject &oldWire, const QJsonObject &newWire);
    void changeParams(int id, QJsonObject oldParams, QJsonObject newParams, bool actionNeeded);
    void changeSize(int id, QPointF oldSize, QPointF newSize);

    void createOrShiftWire(const QJsonObject &jsonObj);
    bool destroyLooseWire();

    void moveItemFromCommand(int id, QPointF pos);
    void reportParamsChanged(int id, QJsonObject oldParams, QJsonObject newParams);
    void prepareMoveSelection();
    void moveSelection(QPointF displacement);
    void finishMoveSelection();
    void reportSizeChanged(int id, QPointF oldSize, QPointF newSize);
    void resizeCasingFromCommand(int id, QPointF size);
    void reportIdeaMinized(int id, bool oldState);
    void minimizeCasingFromCommand(int id, bool state);

    std::vector<CasingBackend*> locateCasingNear(QPointF pos, int padding = 30);

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    QQmlEngine *qmlEngine() const;
    void setQmlEngine(QQmlEngine *qmlEngine);

    //Selection Options.
    void selectInBox(QRectF selectionBox);

    QObjectList selected();
    std::vector<Casing*> selectedCasings();
    std::vector<QQuickItem*> selectedNonWires();
    std::vector<Wire*> selectedWires();

    void clearStopLights();

    bool canUndo();
    Q_INVOKABLE void undo();
    Q_INVOKABLE void redo();
    Q_INVOKABLE void cut();
    Q_INVOKABLE void copy();
    Q_INVOKABLE void paste();
    Q_INVOKABLE void duplicate();
    Q_INVOKABLE void deleteSelection();
    Q_INVOKABLE void deleteAll();

    Q_INVOKABLE void deselectAll();
    Q_INVOKABLE void selectAll();

    Q_INVOKABLE QByteArray save();
    Q_INVOKABLE void load(QByteArray data);

    //View menu stuff.
    Q_INVOKABLE void setVisibleWindowParams(int width, int height);

    Q_INVOKABLE void centerView();
    Q_INVOKABLE void resetScale();
    Q_INVOKABLE void zoomIn();
    Q_INVOKABLE void zoomOut();

    Q_INVOKABLE void minimizeFromCasingMenu();
    Q_INVOKABLE void showControlsFromCasingMenu();
    Q_INVOKABLE void changeNameFromCasingMenu(const QString &newIdeaName);
    Q_INVOKABLE void deleteFromCasingMenu();
    void deleteFromCasing(Casing* casing);

    void zoomIn(QPointF pos);
    void zoomOut(QPointF pos);

    QRectF selectingBox() const;
    bool selecting() const;

    bool contextMenuActive() const;
    QPointF contextMenuPt() const;
    bool casingMenuActive() const;
    QPointF casingMenuPt() const;
    void readyCasingMenu(Casing* casing, QPointF point);

    void setWirePlugSwitch(const QJsonObject &wirePlugSwitch);

    std::shared_ptr<DataTypeRegistry> dataTypeRegistry() const;
    void setDataTypeRegistry(const std::shared_ptr<DataTypeRegistry> &dataTypeRegistry);

    UndoStack* undoStack() const;

    int headerHeight() const;

public slots:
    void setSelecting(bool selecting);
    void setSelectingBox(QRectF selectingBox);

    void setContextMenuActive(bool contextMenuActive);
    void setContextMenuPt(QPointF contextMenuPt); 
    void setCasingMenuActive(bool casingMenuActive);
    void setCasingMenuPt(QPointF casingMenuPt);

    void setHeaderHeight(int headerHeight);

signals:
    void selectingChanged(bool selecting);
    void selectingBoxChanged(QRectF selectingBox);

    void contextMenuActiveChanged(bool contextMenuActive);
    void contextMenuPtChanged(QPointF contextMenuPt);
    void casingMenuActiveChanged(bool casingMenuActive);
    void casingMenuPtChanged(QPointF casingMenuPt);

    void headerHeightChanged(int headerHeight);

private:
    CorkboardBackend* m_backend;
    QQmlContext* m_rootContext;
    QQmlEngine* m_qmlEngine;
    UndoStack* m_undoStack;

    std::shared_ptr<DataTypeRegistry> m_dataTypeRegistry;

    QPointF m_selectionOrPanStartPoint;
    QPointF m_startingPos;
    QRectF m_selectingBox;

    void addInternalWiresToSelection();

    bool m_selecting;
    bool m_panning;
    bool m_drawGrid;
    bool m_contextMenuActive;

    double zoomFactor;

    QJsonObject m_wirePlugSwitch;

    int m_visibleWindowWidth;
    int m_visibleWindowHeight;
    QPointF m_contextMenuPt;
    std::vector<QQuickItem*> m_movingItems;
    std::vector<QPointF> m_initialPositions;

    //Holds the things necessary to help the casing menu do stuff.
    bool m_casingMenuActive;
    QPointF m_casingMenuPt;
    Casing* casingWhichCreatedMenu;
    int m_headerHeight;
};

#endif // CORKBOARD_H
