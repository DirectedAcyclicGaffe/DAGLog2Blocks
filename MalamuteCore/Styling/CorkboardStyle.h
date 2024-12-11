#ifndef CORKBOARDSTYLE_H
#define CORKBOARDSTYLE_H

#include <QObject>
#include <QColor>

class CorkboardStyle : public QObject
{
    Q_OBJECT
public:
    explicit CorkboardStyle(QObject *parent = nullptr);

    Q_INVOKABLE QColor selectionRectangleColor() const;
    Q_INVOKABLE int selectionRectangleWidth() const;
    Q_INVOKABLE int selectionRectangleRadius() const;
    Q_INVOKABLE QString backgroundImagePath() const;

private:
    QColor m_selectionRectangleColor = QColor("teal");
    int m_selectionRectangleWidth = 4;
    int m_selectionRectangleRadius = 4;

    QString m_backgroundImagePath = "qrc:/Images/background.png";
};

#endif // CORKBOARDSTYLE_H
