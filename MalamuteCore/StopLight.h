#ifndef STOPLIGHT_H
#define STOPLIGHT_H

#include "Plug.h"
#include "QColor"

class StopLight
{
public:

    StopLight(PlugType plugType, int plugNumber, QColor color)
    {
        m_plugType = plugType;
        m_int = plugNumber;
        m_color = color;
    }

    PlugType m_plugType;
    int m_int;
    QColor m_color;
};

#endif // STOPLIGHT_H
