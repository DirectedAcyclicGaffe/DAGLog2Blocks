#include "LogarithmBase2Idea.h"
#include <cmath>

LogarithmBase2Attributes::LogarithmBase2Attributes()
{
    m_listName = "Logarithm Base 2";
    m_displayName = "Logarithm\nBase 2";
    m_iconPath = "qrc:/Icons/LogarithmBase2.png";

    m_dataTypesIn.push_back(Magic<NumberDataTypeAttributes>::cast().id());
    m_dataTypesOut.push_back(Magic<NumberDataTypeAttributes>::cast().id());

    m_embeddedQMLPath = ":/QML/LogarithmBase2Minimized.qml";

    m_gear = true;
}

LogarithmBase2Idea::LogarithmBase2Idea()
{

}

const IdeaAttributes LogarithmBase2Idea::attributes()
{
    return Magic<LogarithmBase2Attributes>::cast();
}

void LogarithmBase2Idea::validate()
{
    auto numberInput = m_input.lock();
    if(numberInput)
    {
        if(numberInput->number() > 0.0)
        {
            setStatus(IdeaStatus::Working);
            compute(numberInput->number());
        }
        else if(qFuzzyIsNull(numberInput->number()))
        {
            setStatus(IdeaStatus::Error, "Negative Infinity");
        }
        else
        {
            setStatus(IdeaStatus::Error, "Imaginary Answer");
            emit clear();
        }
    }
    else
    {
        setDefaultStatus();
        emit clear();
    }
}

void LogarithmBase2Idea::compute(double n1)
{
    emit startSpinningGear();

    double log = log2(n1);

    m_result = std::make_shared<NumberDataType>(log);
    emit newData(0);

    setupDisplay();

    emit stopSpinningGear();
}
