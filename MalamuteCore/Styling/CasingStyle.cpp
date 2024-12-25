#include "CasingStyle.h"

CasingStyle::CasingStyle(QObject *parent) : QObject(parent)
{
    m_nameFont = QFont("verdana", 20, QFont::Bold);
    m_plugLabelsFont = QFont("verdana", 11, QFont::Bold);
    m_feedbackTextFont = QFont("verdana", 14,QFont::Bold);

    m_colorList = {QColor(0xa6b0cb),
                   QColor(0xa6c6cb),
                   QColor(0xcbb2a6),
                   QColor(0xa6cbb7),
                   QColor(0xcba6b5),
                   QColor(0xbca6cb)};
}

double CasingStyle::bordersWidth() const
{
    return m_bordersWidth;
}

QColor CasingStyle::borderColor() const
{
    return m_borderColor;
}

QColor CasingStyle::borderSelectedColor() const
{
    return m_borderSelectedColor;
}

int CasingStyle::glowRadius() const
{
    return m_glowRadius;
}

double CasingStyle::glowSpread() const
{
    return m_glowSpread;
}

QColor CasingStyle::glowColor() const
{
    return m_glowColor;
}

double CasingStyle::plugSize() const
{
    return m_plugSize;
}

double CasingStyle::plugSpacing() const
{
    return m_plugSpacing;
}

double CasingStyle::plugBorderWidth() const
{
    return m_plugBorderWidth;
}

QColor CasingStyle::nameColor() const
{
    return m_nameColor;
}

QFont CasingStyle::nameFont() const
{
    return m_nameFont;
}

QFont CasingStyle::plugLabelsFont() const
{
    return m_plugLabelsFont;
}

QFont CasingStyle::feedbackTextFont() const
{
    return m_feedbackTextFont;
}

QColor CasingStyle::feedbackTextColor() const
{
    return m_feedbackTextColor;
}

double CasingStyle::buttonSize() const
{
    return m_buttonSize;
}

QColor CasingStyle::buttonColor() const
{
    return m_buttonColor;
}

double CasingStyle::buttonHoverDarkenFactor() const
{
    return m_buttonHoverDarkenFactor;
}

QColor CasingStyle::resizerColor() const
{
    return m_resizerColor;
}

QColor CasingStyle::resizerBorderColor() const
{
    return m_resizerBorderColor;
}

QColor CasingStyle::getColor()
{
    colorListItr++;
    if(colorListItr == m_colorList.length())
        colorListItr = 0;

    return m_colorList.at(colorListItr);
}

double CasingStyle::frameRadius() const
{
    return m_frameRadius;
}

double CasingStyle::resizerSize() const
{
    return m_resizerSize;
}
