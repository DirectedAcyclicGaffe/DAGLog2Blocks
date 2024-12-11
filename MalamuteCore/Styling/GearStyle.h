#ifndef GEARSTYLE_H
#define GEARSTYLE_H

#include <QObject>
#include <QColor>

class GearStyle : public QObject
{
    Q_OBJECT

public:
    explicit GearStyle(QObject *parent = nullptr);

    Q_INVOKABLE QString gearPath() const;
    Q_INVOKABLE int gearSpeed() const;

private:
    QString m_gearPath = "qrc:/Images/Gear.png";
    int m_gearSpeed = 3000;
};

#endif // GEARSTYLE_H
