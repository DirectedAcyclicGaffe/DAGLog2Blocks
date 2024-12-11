#ifndef LOGBASE2VISUALAREA_H
#define LOGBASE2VISUALAREA_H

#include <QQuickItem>
#include <QQmlEngine>

class LogBase2VisualArea : public QQuickItem
{
    Q_OBJECT
public:
    LogBase2VisualArea();

signals:

public slots:

    void setup(double n, QColor inColor, QColor outColor);
    void clear();
    void reset();
    void action();

private:
    void prepareSizeVariables(int wholeBoxes);
    int m_xBoxNum;
    double m_spacing;
    int m_borderWidth;

    QColor m_outColor;
    QColor m_inColor;
    double m_currentlyVisible;
    double m_inputNum;

    QList<QQuickItem*> m_tiles;
    QList<QQuickItem*> m_shadows;

};

#endif // LOGBASE2VISUALAREA_H
