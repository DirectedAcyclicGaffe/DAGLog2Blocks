#include "DataTypeAttributes.h"

QString DataTypeAttributes::name() const
{
    return m_name;
}

QString DataTypeAttributes::abbreviation() const
{
    return m_abbreviation;
}

int DataTypeAttributes::id() const
{
    return m_id;
}

void DataTypeAttributes::setID(int value)
{
    m_id = value;
}
