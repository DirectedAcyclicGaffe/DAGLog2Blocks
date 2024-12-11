#include "DeleteCasingCommand.h"
#include "../Corkboard.h"
#include "../CorkboardBackend.h"

DeleteCasingCommand::DeleteCasingCommand(const QJsonObject &obj, CorkboardBackend *cb) :
    m_cb(cb),
    m_obj(obj)
{

}

void DeleteCasingCommand::undo()
{
    //Recreate the casing.
    m_cb->corkboard()->createCasing(m_obj);

    //Recreate connections which were no longer valid.
    for (QJsonValueRef wire : m_extraWireArray)
         m_cb->createWire(wire.toObject());
}

void DeleteCasingCommand::redo()
{

    //Delete the idea, store a record of the extra deleted objects
    m_extraWireArray = QJsonArray();
    QJsonArray tempArray;

    //Save hanging wires removed from casing
    tempArray = m_cb->removeCasingBackend(m_obj["id"].toInt());
    for(int i = 0; i < tempArray.size(); i++)
        m_extraWireArray.append(tempArray[i]);
}
