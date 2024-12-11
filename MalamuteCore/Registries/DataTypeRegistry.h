#ifndef DATATYPEREGISTRY_H
#define DATATYPEREGISTRY_H

#include <QString>
#include <QColor>
#include "../InheritedClasses/Magic.h"

class DataTypeRegistry
{

public:

    template<typename DataTypeTemplate> void registerDataTypeAttributes()
    {
        if(Magic<DataTypeTemplate>::cast().id() != -1)
        {   //Data type registered multiple times.
            return;
        }

        Magic<DataTypeTemplate>::cast().setID(m_idCounter);
        m_idCounter++;

        auto instance = Magic<DataTypeTemplate>::cast();
        m_dataTypeAbbreviations.push_back(instance.abbreviation());
    }

    QString dataTypeAbbreviation(int id);

private:
    std::vector<QString> m_dataTypeAbbreviations;

    int m_idCounter = 0;
};

#endif // DATATYPEREGISTRY_H
