#ifndef CASINGSTYLE_H
#define CASINGSTYLE_H

#include <QObject>
#include <QColor>
#include <QFont>

class CasingStyle : public QObject
{
    Q_OBJECT

public:
    explicit CasingStyle(QObject *parent = nullptr);

    Q_INVOKABLE double bordersWidth() const;
    Q_INVOKABLE double frameRadius() const;
    Q_INVOKABLE QColor borderColor() const;
    Q_INVOKABLE QColor borderSelectedColor() const;

    Q_INVOKABLE int glowRadius() const;
    Q_INVOKABLE double glowSpread() const;
    Q_INVOKABLE QColor glowColor() const;

    Q_INVOKABLE double plugSize() const;
    Q_INVOKABLE double plugSpacing() const;
    Q_INVOKABLE double plugBorderWidth() const;

    Q_INVOKABLE QColor nameColor() const;
    Q_INVOKABLE QFont nameFont() const;

    Q_INVOKABLE QFont plugLabelsFont() const;

    Q_INVOKABLE QFont feedbackTextFont() const;
    Q_INVOKABLE QColor feedbackTextColor() const;

    Q_INVOKABLE double buttonSize() const;
    Q_INVOKABLE QColor buttonColor() const;
    Q_INVOKABLE double buttonHoverDarkenFactor() const;

    Q_INVOKABLE double resizerSize() const;
    Q_INVOKABLE QColor resizerColor() const;
    Q_INVOKABLE QColor resizerBorderColor() const;

    QColor getColor();

private:
    double m_bordersWidth = 4;
    double m_frameRadius = 20;
    QColor m_borderColor = QColor(0x70460c);
    QColor m_borderSelectedColor = QColor(0x0b0b0b);

    int m_glowRadius = 6;
    double m_glowSpread = 0.4;
    QColor m_glowColor = QColor(0x323232);

    double m_plugSize = 15;
    double m_plugSpacing = 15;
    double m_plugBorderWidth = 3;

    QColor m_nameColor = QColor(0x000001);
    QFont m_nameFont;

    QFont m_plugLabelsFont;

    QColor m_plugLabelsColor = QColor(0x000001);
    QFont m_feedbackTextFont;
    QColor m_feedbackTextColor = QColor(0x000001);

    double m_buttonSize = 22;
    QColor m_buttonColor = QColor(0x845a35);
    double m_buttonHoverDarkenFactor = 1.5;

    double m_resizerSize = 14;
    QColor m_resizerColor = QColor(0x000001);
    QColor m_resizerBorderColor = QColor(0x808080);

    QColor m_defaultIColor = QColor(0xa6b0cb);
    QList<QColor> m_colorList;
    int colorListItr = 0;
};

#endif // CASINGSTYLE_H
