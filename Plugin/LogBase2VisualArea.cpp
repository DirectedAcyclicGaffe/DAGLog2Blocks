#include "LogBase2VisualArea.h"
#include <cmath>

LogBase2VisualArea::LogBase2VisualArea()
{
    setFlag(ItemHasContents);
    setWidth(400);
    setHeight(400);
}

void LogBase2VisualArea::setup(double n, QColor inColor, QColor outColor)
{
    m_currentlyVisible = n;
    m_inputNum = n;
    m_outColor = outColor;
    m_inColor = inColor;

    clear();
    int totalBoxes = std::ceil(n);
    prepareSizeVariables(totalBoxes);

    m_shadows.reserve(totalBoxes);
    for(int i = 0; i < totalBoxes; i++)
    {
        QQmlComponent comp(qmlEngine(this), QUrl(QStringLiteral("qrc:/QML/BlockLogarithmBoxShadow.qml")));

        QVariantMap map;
        map["x"] = 0.5 + m_spacing * (i % m_xBoxNum);
        map["y"] = 0.5 + m_spacing * std::floor(i/m_xBoxNum);

        if(i == totalBoxes - 1 && !qFuzzyIsNull(n - totalBoxes))
            map["width"] = (n - std::floor(n)) * (m_spacing - 1.0);
        else
            map["width"] = m_spacing - 1.0;

        map["height"] = m_spacing - 1.0;
        map["color"] = inColor;
        map["opacity"] = 0.3;
        map["border.width"] = m_borderWidth;
        map["border.color"] = inColor.darker();

        QObject* obj = comp.createWithInitialProperties(map, qmlEngine(this)->rootContext());
        QQuickItem* visibleObj = static_cast<QQuickItem*>(obj);

        m_shadows.push_back(visibleObj);
        visibleObj->setParent(this);
        visibleObj->setParentItem(this);
    }

    m_tiles.reserve(totalBoxes);
    for(int i = 0; i < totalBoxes; i++)
    {
        QQmlComponent comp(qmlEngine(this), QUrl(QStringLiteral("qrc:/QML/BlockLogarithmBox.qml")));

        QVariantMap map;
        map["x"] = 0.5 + m_spacing * (i % m_xBoxNum);
        map["y"] = 0.5 + m_spacing * std::floor(i/m_xBoxNum);

    if(i == totalBoxes - 1 && !qFuzzyIsNull(n - totalBoxes))
            map["width"] = (n - std::floor(n)) * (m_spacing - 1.0);
        else
            map["width"] = m_spacing - 1.0;

        map["height"] = m_spacing - 1.0;
        map["color"] = inColor;
        map["border.width"] = m_borderWidth;
        map["border.color"] = inColor.darker();

        QObject* obj = comp.createWithInitialProperties(map, qmlEngine(this)->rootContext());
        QQuickItem* visibleObj = static_cast<QQuickItem*>(obj);

        m_tiles.push_back(visibleObj);
        visibleObj->setParent(this);
        visibleObj->setParentItem(this);
    }

}

void LogBase2VisualArea::clear()
{
    for(int i = 0; i < m_shadows.length(); i++)
        m_shadows[i]->deleteLater();
    m_shadows.clear();
    for(int i = 0; i < m_tiles.length(); i++)
        m_tiles[i]->deleteLater();
    m_tiles.clear();
}

void LogBase2VisualArea::reset()
{
    m_currentlyVisible = m_inputNum;
    for(int i = 0; i < m_tiles.length(); i++)
    {
        QMetaObject::invokeMethod(m_tiles[i], "resetColor", Q_ARG(QVariant, m_inColor));
        m_tiles[i]->setWidth(m_spacing - 1.0);
        m_tiles[i]->setHeight(m_spacing - 1.0);
        m_tiles[i]->setX(0.5 + m_spacing * (i % m_xBoxNum));
    }
}

void LogBase2VisualArea::action()
{
    //fadeOut()
    int m_lowLim = std::ceil(m_currentlyVisible / 2.0);
    for(int i = m_lowLim; i < m_currentlyVisible; i++)
    {
        QMetaObject::invokeMethod(m_tiles[i], "fadeOut", Q_ARG(QVariant, m_outColor));
    }
    if(!qFuzzyIsNull((m_currentlyVisible/ 2.0) - std::floor(m_currentlyVisible / 2.0)) && m_currentlyVisible > 2.0)
    {
        //shrinkAnimation
        QMetaObject::invokeMethod(m_tiles[std::floor(m_currentlyVisible / 2.0)], "shrink",
                                  Q_ARG(QVariant, (((m_currentlyVisible / 2.0) - std::floor(m_currentlyVisible / 2.0)) *(m_spacing - 1.0))));

    }
    m_currentlyVisible /= 2;
}

void LogBase2VisualArea::prepareSizeVariables(int wholeBoxes)
{
    if(wholeBoxes <= 10)
    {
        m_borderWidth = 8;
        m_spacing = 80;
        m_xBoxNum = 5;
    }
    else if(wholeBoxes <= 50)
    {
        m_borderWidth = 4;
        m_spacing = 40;
        m_xBoxNum = 10;
    }
    else if(wholeBoxes <= 200)
    {
        m_borderWidth = 2;
        m_spacing = 20;
        m_xBoxNum = 20;
    }
    else if(wholeBoxes <= 800)
    {
        m_borderWidth = 1;
        m_spacing = 10;
        m_xBoxNum = 40;
    }
    else if(wholeBoxes <= 5000)
    {
        m_borderWidth = 0;
        m_spacing = 4;
        m_xBoxNum = 100;
    }
    else if(wholeBoxes <= 20000)
    {
        m_borderWidth = 0;
        m_spacing = 2;
        m_xBoxNum = 200;
    }
}
