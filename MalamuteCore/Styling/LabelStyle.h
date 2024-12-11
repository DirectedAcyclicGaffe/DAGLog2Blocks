#ifndef LABELSTYLE_H
#define LABELSTYLE_H

#include <QColor>

class LabelStyle
{
public:
    LabelStyle();

    static QColor defaultColor();
    QColor getColor();

private:
    QList<QColor> m_colorList;
    int colorListItr = 0;
};

#endif // LABELSTYLE_H
