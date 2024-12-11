#include "ShiftWireCommand.h"
#include "../Corkboard.h"
#include "../CorkboardBackend.h"

ShiftWireCommand::ShiftWireCommand(const QJsonObject &oldWire, const QJsonObject &newWire,
                                               CorkboardBackend *cb, bool actionNeeded) :
    m_cb(cb),
    m_oldWire(oldWire),
    m_newWire(newWire),
    m_actionNeeded(actionNeeded)
{

}

void ShiftWireCommand::undo()
{
    // Always remember to unplug the old wires before adding new ones in.
    // I just spent 5 hours before figuring this out...
    // I have brought shame to the electrical engineer, my grandpa bob,
    // who gave me the nickname dBob. Time for sleep.
    m_cb->removeWire(m_newWire["id"].toInt());
    m_cb->createWire(m_oldWire);
}

void ShiftWireCommand::redo()
{
    if(m_actionNeeded)
    {
        m_cb->removeWire(m_oldWire["id"].toInt());
        m_cb->createWire(m_newWire);
    }
    m_actionNeeded = true;
}
