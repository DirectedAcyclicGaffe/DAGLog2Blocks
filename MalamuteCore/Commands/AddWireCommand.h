#ifndef ADDWIRECOMMAND_H
#define ADDWIRECOMMAND_H

#include "UndoCommand.h"
#include <QJsonObject>

class CorkboardBackend;

class AddWireCommand: public UndoCommand
{
public:
    AddWireCommand(const QJsonObject& obj,CorkboardBackend* cb, bool actionNeeded);

    void undo() override;
    void redo() override;

private:
    CorkboardBackend* m_cb;
    QJsonObject m_obj;
    bool m_actionNeeded;
};

#endif // ADDWIRECOMMAND_H
