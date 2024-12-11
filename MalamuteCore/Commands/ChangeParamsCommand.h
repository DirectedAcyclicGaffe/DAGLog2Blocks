#ifndef CHANGEPARAMSCOMMAND_H
#define CHANGEPARAMSCOMMAND_H

#include "UndoCommand.h"
#include <QJsonObject>

class CorkboardBackend;

class ChangeParamsCommand: public UndoCommand
{
public:
    ChangeParamsCommand(int id, QJsonObject oldParams, QJsonObject newParams,
                            CorkboardBackend* sb, bool actionNeeded);

    void undo() override;
    void redo() override;

private:
    CorkboardBackend* m_cb;
    int m_id;
    QJsonObject m_oldParams;
    QJsonObject m_newParams;
    bool m_actionNeeded;
};

#endif // CHANGEPARAMSCOMMAND_H
