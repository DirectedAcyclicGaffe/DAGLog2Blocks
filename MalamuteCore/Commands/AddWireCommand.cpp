#include "AddWireCommand.h"
#include "../Corkboard.h"
#include "../CorkboardBackend.h"

AddWireCommand::AddWireCommand(const QJsonObject& obj,CorkboardBackend* cb, bool actionNeeded) :
    m_cb(cb),
    m_obj(obj),
    m_actionNeeded(actionNeeded)
{
}

void AddWireCommand::undo()
{
    m_actionNeeded = true;
    m_cb->removeWire(m_obj["id"].toInt());
}

void AddWireCommand::redo()
{
    if(m_actionNeeded)
        m_cb->createWire(m_obj);
}
