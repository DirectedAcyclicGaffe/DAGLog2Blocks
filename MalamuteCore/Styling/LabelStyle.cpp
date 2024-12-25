#include "LabelStyle.h"

LabelStyle::LabelStyle()
{
    m_colorList = {QColor(0xd4985d),

                   QColor(0xd28888),
                   QColor(0xd29cf4),
                   QColor(0x8cea8c),
                   QColor(0x8ae6f9),
                   QColor(0xd1d693),
                   QColor(0xab9af4),
                   QColor(0xf49ce4),
                   QColor(0xa4efdc),

                   QColor(0xd4b35d),
                   QColor(0xb15e43),
                   QColor(0xd242ea),
                   QColor(0x26d363),
                   QColor(0x0b80d1),
                   QColor(0x7cb143),
                   QColor(0xf5580a),
                   QColor(0x8e1155),
                   QColor(0x1e9fa5)};
}

QColor LabelStyle::defaultColor()
{
    return QColor(0x7c91b6);
}

QColor LabelStyle::getColor()
{
    colorListItr++;
    if(colorListItr == m_colorList.length())
        colorListItr = 0;
    
    return m_colorList.at(colorListItr);
}
