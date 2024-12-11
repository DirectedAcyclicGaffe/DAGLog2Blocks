#include "AddCasingCommand.h"
#include "../Corkboard.h"
#include "../CorkboardBackend.h"

AddCasingCommand::AddCasingCommand(const QJsonObject& obj,CorkboardBackend* cb) :
    m_cb(cb),
    m_obj(obj)
{
}

void AddCasingCommand::undo()
{
    m_cb->removeCasingBackend(m_obj["id"].toInt());
}

void AddCasingCommand::redo()
{
    m_cb->createIdea(m_obj);
}
