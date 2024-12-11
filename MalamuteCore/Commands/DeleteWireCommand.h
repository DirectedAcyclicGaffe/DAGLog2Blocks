#ifndef DELETEWIRECOMMAND_H
#define DELETEWIRECOMMAND_H

#include "UndoCommand.h"
#include <QJsonObject>

class CorkboardBackend;

class DeleteWireCommand: public UndoCommand
{
public:
    DeleteWireCommand(const QJsonObject& obj, CorkboardBackend* cb);

    void undo() override;
    void redo() override;

private:
    CorkboardBackend* m_cb;
    QJsonObject m_obj;
};

#endif // REMOVEWIRECOMMAND_H
