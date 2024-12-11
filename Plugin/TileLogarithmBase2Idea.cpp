#include "TileLogarithmBase2Idea.h"
#include <cmath>

TileLogarithmBase2Attributes::TileLogarithmBase2Attributes()
{
    m_listName = "Block Logarithm Base 2";
    m_displayName = "Logarithm Base 2";
    m_iconPath = "qrc:/Icons/LogarithmBase2.png";

    m_dataTypesIn.push_back(Magic<NumberDataTypeAttributes>::cast().id());

    m_dataTypesOut.push_back(Magic<NumberDataTypeAttributes>::cast().id());

    m_embeddedQMLPath = ":/QML/BlockLogarithmBase2Idea.qml";
    m_minimizedQMLPath = ":/QML/LogarithmBase2Minimized.qml";
    m_minimizedName = "";
    m_startMinimized = false;

    m_gear = true;
}

TileLogarithmBase2Idea::TileLogarithmBase2Idea()
{

}

const IdeaAttributes TileLogarithmBase2Idea::attributes()
{
    return Magic<TileLogarithmBase2Attributes>::cast();
}

void TileLogarithmBase2Idea::resetDisplay()
{
    if(valid())
    {
        double n = m_input.lock()->number();
        double log = std::log2(n);
        emit display(n, log);
    }
}
