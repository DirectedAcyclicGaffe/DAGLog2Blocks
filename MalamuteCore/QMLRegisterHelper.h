#ifndef QMLREGISTERHELPER_H
#define QMLREGISTERHELPER_H

#include "Corkboard.h"
#include "CorkboardHandler.h"
#include "Styling/CorkboardStyle.h"

#include "Casing.h"
#include "Styling/CasingStyle.h"

#include "Wire.h"
#include "Styling/WireStyle.h"
#include "Styling/GearStyle.h"

void registerQML(QObject* app)
{
    qmlRegisterType<Corkboard>("com.malamute.core",1,0,"Corkboard");
    qmlRegisterType<CorkboardHandler>("com.malamute.core", 1, 0, "CorkboardHandler");
    CorkboardStyle* corkboardStyle = new CorkboardStyle(app);
    qmlRegisterSingletonInstance("com.malamute.core", 1, 0, "CorkboardStyle", corkboardStyle);

    qmlRegisterType<Casing>("com.malamute.core",1,0,"MCasing");
    CasingStyle* casingStyle = new CasingStyle(app);
    qmlRegisterSingletonInstance("com.malamute.core", 1, 0, "CasingStyle", casingStyle);

    qmlRegisterType<Wire>("com.malamute.core",1,0,"MWire");
    WireStyle* wireStyle = new WireStyle(app);
    qmlRegisterSingletonInstance("com.malamute.core", 1, 0, "WireStyle", wireStyle);

    GearStyle* gearStyle = new GearStyle(app);
    qmlRegisterSingletonInstance("com.malamute.core", 1, 0, "GearStyle", gearStyle);
}


#endif // QMLREGISTERHELPER_H
