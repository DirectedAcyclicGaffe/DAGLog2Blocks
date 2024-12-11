#include "NumberInputIdea.h"

#include <QJsonValue>

NumberInputAttributes::NumberInputAttributes()
{
    m_listName = "Number Input";
    m_displayName = "";
    m_iconPath = "qrc:/Icons/NumberInput.png";

    m_dataTypesOut.push_back(Magic<NumberDataTypeAttributes>::cast().id());

    m_labelsOut << "";
    m_customLabels = true;

    m_embeddedQMLPath = ":/QML/NumberInput.qml";
}

NumberInputIdea::NumberInputIdea() :
    m_number(0.0)
{
    m_numberOutput = std::make_shared<NumberDataType>(m_number);
    setStatus(IdeaStatus::Working);
}

const IdeaAttributes NumberInputIdea::attributes()
{
    return Magic<NumberInputAttributes>::cast();
}

QJsonObject NumberInputIdea::save() const
{
    QJsonObject ideaJson = Idea::save();
    ideaJson["n"] = m_number;
    return ideaJson;
}

void NumberInputIdea::load(QJsonObject const &p)
{
    QJsonValue v = p["n"];

    if (!v.isUndefined())
    {
        m_number = v.toDouble();
        emit numberChanged(m_number);

        compute();
    }
    Idea::load(p);
}

std::shared_ptr<DataType> NumberInputIdea::dataOut(int)
{
    return m_numberOutput;
}

double NumberInputIdea::number() const
{
    return m_number;
}

void NumberInputIdea::setNumber(double number)
{
    if(!qFuzzyCompare(m_number,number))
    {
        QJsonObject oldParams = this->save();

        m_number = number;
        emit numberChanged(m_number);

        QJsonObject newParams = this->save();
        emit reportParamsChanged(oldParams,newParams);

        compute();
    }
}

void NumberInputIdea::compute()
{
    m_numberOutput = std::make_shared<NumberDataType>(m_number);
    emit newData(0);
}
