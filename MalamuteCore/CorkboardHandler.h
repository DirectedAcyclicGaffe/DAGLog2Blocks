#ifndef CORKBOARDHANDLER_H
#define CORKBOARDHANDLER_H

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>

#include "Corkboard.h"
#include "Casing.h"
#include "Wire.h"

#include "InheritedClasses/MalamutePlugin.h"

class CorkboardHandler : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList ideaList READ ideaList NOTIFY ideaListChanged)

public:
    explicit CorkboardHandler(QObject *parent = nullptr);

    ///Call this method after you've loaded new idea collections to add them to corkboard.
    Q_INVOKABLE void resetCorkboard(Corkboard *corkboard);

    Q_INVOKABLE void save();
    Q_INVOKABLE void saveAs(const QUrl &fileUrl);
    Q_INVOKABLE void load(const QUrl &fileUrl);

    Q_INVOKABLE QUrl getOpenFileUrl() const;
    Q_INVOKABLE bool validOpenFileUrl() const;
    void setOpenFileUrl(const QUrl &value);

    Q_INVOKABLE void setVisibleWindowProperties(int width, int height);

    QStringList registeredIdeas() const;
    void loadPlugin(MalamutePlugin *plugin);

    Q_INVOKABLE QString ideaIconPath(QString ideaName);
    QStringList ideaList() const;

signals:
    void ideaListChanged();

private:
    Corkboard* m_corkboard;

    std::shared_ptr<IdeaRegistry> m_ideaRegistry;
    std::shared_ptr<DataTypeRegistry> m_dataTypeRegistry;

    QStringList m_ideaList;

    QUrl openFileUrl;
    QString m_initialFileURL = "";
};

#endif // CORKBOARDHANDLER_H
