#include "OneToOneIdeaTemplate.h"

OneToOneIdeaTemplate::OneToOneIdeaTemplate()
{

}

void OneToOneIdeaTemplate::onNewDataIn(std::shared_ptr<DataType> dataType, int)
{
    auto number = std::dynamic_pointer_cast<NumberDataType>(dataType);
    m_input = number;

    validate();
}

std::shared_ptr<DataType> OneToOneIdeaTemplate::dataOut(int)
{
    return std::static_pointer_cast<DataType>(m_result);
}

void OneToOneIdeaTemplate::setupDisplay()
{
    emit display(m_input.lock()->number(),
                 m_result->number());
}

void OneToOneIdeaTemplate::updateOnOutPlugLabelColor()
{
    if(valid())
        setupDisplay();
}

void OneToOneIdeaTemplate::updateOnInPlugLabelColor()
{
    if(valid())
        setupDisplay();
}

