#include "DeleteWireCommand.h"
#include "../Corkboard.h"
#include "../CorkboardBackend.h"

DeleteWireCommand::DeleteWireCommand(const QJsonObject& obj, CorkboardBackend* cb) :
    m_cb(cb),
    m_obj(obj)
{

}

void DeleteWireCommand::undo()
{
    m_cb->createWire(m_obj);
}

void DeleteWireCommand::redo()
{
    m_cb->removeWire(m_obj["id"].toInt());
}
