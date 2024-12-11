#include "DataTypeRegistry.h"

QString DataTypeRegistry::dataTypeAbbreviation(int id)
{
    return m_dataTypeAbbreviations.at(id);
}
