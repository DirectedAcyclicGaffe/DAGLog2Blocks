#include "DeleteSelectionCommand.h"
#include "../Corkboard.h"
#include "../CorkboardBackend.h"

DeleteSelectionCommand::DeleteSelectionCommand(const QJsonObject &obj, CorkboardBackend *cb) :
    m_cb(cb),
    m_obj(obj)
{

}

void DeleteSelectionCommand::undo()
{
    //Recreate the casings.
    QJsonArray casingsJsonArray = m_obj["c"].toArray();
    for(QJsonValueRef casing: casingsJsonArray)
        m_cb->corkboard()->createCasing(casing.toObject());

    //Recreate the selected connections.
    QJsonArray wiresJsonArray = m_obj["w"].toArray();
    for(QJsonValueRef wire: wiresJsonArray)
        m_cb->createWire(wire.toObject());

    //Recreate connections which were no longer valid.
    for (QJsonValueRef wire : m_extraWireArray)
        m_cb->createWire(wire.toObject());
}

void DeleteSelectionCommand::redo()
{
    //Delete the connections.

    QJsonArray wiresJsonArray = m_obj["w"].toArray();
    for(QJsonValueRef wire: wiresJsonArray)
        m_cb->removeWire(wire.toObject()["id"].toInt());

    //Delete the ideas, store a record of the extra deleted objects
    m_extraWireArray = QJsonArray();
    QJsonArray tempArray;
    QJsonArray casingsJsonArray = m_obj["c"].toArray();
    for(QJsonValueRef casing: casingsJsonArray)
    {
        tempArray = m_cb->removeCasingBackend(casing.toObject()["id"].toInt());
        for(int i = 0; i < tempArray.size(); i++)
            m_extraWireArray.append(tempArray[i]);
    }
}
