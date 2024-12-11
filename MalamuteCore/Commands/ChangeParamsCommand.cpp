#include "ChangeParamsCommand.h"
#include "../Corkboard.h"
#include "../CorkboardBackend.h"

ChangeParamsCommand::ChangeParamsCommand(int id, QJsonObject oldParams, QJsonObject newParams,
                                                 CorkboardBackend *cb, bool actionNeeded) :
    m_cb(cb),
    m_id(id),
    m_oldParams(oldParams),
    m_newParams(newParams),
    m_actionNeeded(actionNeeded)
{

}

void ChangeParamsCommand::undo()
{
    m_actionNeeded = true;
    m_cb->getCasing(m_id)->idea()->load(m_oldParams);
}

void ChangeParamsCommand::redo()
{
    if(m_actionNeeded)
        m_cb->getCasing(m_id)->idea()->load(m_newParams);
}
