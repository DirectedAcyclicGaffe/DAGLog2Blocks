#ifndef IDEA_H
#define IDEA_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QGuiApplication>

#include "../Plug.h"
#include "DataType.h"
#include "IdeaAttributes.h"

// The current status of the idea.
// change this from working to show a colored rectangle to show local information.
// Errors should be shown locally whenever possible. This is how you show errors.
enum class IdeaStatus
{
    Working, // Normal. Doesn't show anything. You need to click on the idea to see what's happening.
    StandBy, // Waiting for a task. This is designed to look unobtrusive. Not a warning, but not doing stuff
    Warning, // Something is fishy. The idea is doing something, but you should think for a second if it is right.
    Error, // An error. Should be fixed here. Important but not urgent. Do not confuse important and urgent. They are different
    DisplayWarning, // The box is doing the correct thing. The visual can't handle this situation.
    Fatal // Important and Urgent. This should almost never be used.
};


class Idea : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool valid READ valid NOTIFY validChanged)
    Q_PROPERTY(bool minimized READ minimized WRITE setMinimized NOTIFY minimizedChanged)
    Q_PROPERTY(QColor iColor READ iColor NOTIFY iColorChanged)
    Q_PROPERTY(QList<QColor> inPlugLabelColors READ inPlugLabelColors NOTIFY inPlugLabelColorsChanged)
    Q_PROPERTY(QList<QColor> outPlugLabelColors READ outPlugLabelColors NOTIFY outPlugLabelColorsChanged)

public:

    ///Tell the program where the attributes (name, icon, etc) for the idea are. Use the Magic<NameHereIdea>(); way.
    virtual const IdeaAttributes attributes() = 0;

    ///Override this if you want the idea to save/copy/undo/redo any internal variables.
    virtual QJsonObject save() const;

    ///Override this if you want the idea to load/paste/undo/redo any internal variables.
    virtual void load(QJsonObject const & obj);

    ///Override this to add things to a custom qml Context.
    virtual void finishSetupQmlContext();

    ///Override this to allow the idea to accept new data. Remember to cast the data type.
    virtual void onNewDataIn(std::shared_ptr<DataType> dataType, int plugNumber);

    ///Override this so the program knows what data to send out.
    virtual std::shared_ptr<DataType> dataOut(int plugNumber);

    ///The system calls this when a display might need to be reset. An example is when maximizing a box
    /// The display might not come back correct. Override this method to make it work.
    virtual void resetDisplay();

    // This is called whenever the idea color is changed;
    virtual void updateOnIColor();

    // This is called whenever the color on an in plug is changed.
    virtual void updateOnInPlugLabelColor();

    // This is called whenever the color on an out plug is changed.
    virtual void updateOnOutPlugLabelColor();

    ///Set the status of the idea. Working or Warning status is required for the idea to communicate with other ideas.
    void setStatus(IdeaStatus status, QString message = "");
    ///Sets the idea to StandBy, StandBy
    void setDefaultStatus();

    // These methods shouldn't be called by inherited classes.
    QString statusMessage() const;
    IdeaStatus status() const;

    /// The idea is working, warning or has a display error.
    bool valid() const;

    /// Returns true is the idea is minimized.
    bool minimized() const;

    QColor iColor() const;

    //The colors of the labels on the text for the plugs going into the idea
    QList<QColor> inPlugLabelColors() const;
    //The colors of the labels on the text for the plugs going out of the idea
    QList<QColor> outPlugLabelColors() const;

    void prepareInPlugLabelColors(int n, QColor color);
    void prepareOutPlugLabelColors(QList<QColor> colorList);

signals:

    /// Lets the program know that new data is available on a specific plug out.
    void newData(int plugNumber);

    /// Lets the program know to remove all out data from an idea. Called automatically
    /// on status of standby, error, or fatal. (You probably don't need to use it).
    void clearDataAllPlugs();

    /// Lets the program know the status has changed. (Called automatically).
    void statusChanged();

    /// Starts the gear spinning.
    void startSpinningGear();

    /// Stops the gear. The gear doesn't stop immediately, but instead 0.5-1 second later.
    void stopSpinningGear();

    /// Lets the program know a parameter changed. This is so undo/redo can effect the internal idea
    /// variables.
    void reportParamsChanged(QJsonObject oldParams, QJsonObject newParams);

    /// Tells the system to remove plugs. Done this way so the undo/redo framework can be used.
    void disconnectPlugs(std::vector<UnplugWiresInfo> unplugWires);

    /// Sets a new context property in the embedded or external qml. Probably shouldn't need this.
    void setContextProperty(QString name, QObject* obj);

    /// Call this signal from a qml file to delete the ith external qml file.
    void deleteExternalQML(int i);

    void validChanged(bool valid);

    void minimizedChanged(bool minimized);

    void iColorChanged(QColor iColor);

    void inPlugLabelColorsChanged(QList<QColor> inPlugLabelColors);
    void outPlugLabelColorsChanged(QList<QColor> outPlugLabelColors);

public slots:
    ///Sends any text to the system-wide clipboard. Useful when you want to copy an output from a box.
    void sendTextToClipboard(QString text);
    /// Can be used to minimize the idea.
    void setMinimized(bool minimized);

    void setIdeaColor(QColor iColor);
    void setInPlugLabelColor(int i, QColor color);
    void setOutPlugLabelColor(int i, QColor color);
    const QColor getInPlugLabelColor(int i);
    const QColor getOutPlugLabelColor(int i);

private:
    QString m_statusMessage = QString("Standing By");
    IdeaStatus m_status = IdeaStatus::StandBy;
    bool m_minimized = false;

protected:
    QColor m_iColor = QColor(0xa6b0cb);
    QList<QColor> m_inPlugLabelColors;
    QList<QColor> m_outPlugLabelColors;
};

#endif // IDEA_H
