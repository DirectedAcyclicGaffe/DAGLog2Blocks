#ifndef SHIFTWIRECOMMAND_H
#define SHIFTWIRECOMMAND_H

#include "UndoCommand.h"
#include <QJsonObject>

class CorkboardBackend;

class ShiftWireCommand: public UndoCommand
{
public:
    ShiftWireCommand(const QJsonObject& oldWire, const QJsonObject& newWire, CorkboardBackend* cb, bool actionNeeded);

    void undo() override;
    void redo() override;

private:
    CorkboardBackend* m_cb;
    QJsonObject m_oldWire;
    QJsonObject m_newWire;
    bool m_actionNeeded;
};

#endif // SHIFTWIRECOMMAND_H
