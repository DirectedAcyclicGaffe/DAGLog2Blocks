#ifndef DELETESELECTIONCOMMAND_H
#define DELETESELECTIONCOMMAND_H

#include "UndoCommand.h"
#include <QJsonObject>
#include <QJsonArray>

class CorkboardBackend;

class DeleteSelectionCommand: public UndoCommand
{
public:
    DeleteSelectionCommand(const QJsonObject& obj, CorkboardBackend* corkboard);

    void undo() override;
    void redo() override;

private:
    CorkboardBackend* m_cb;
    QJsonObject m_obj;
    QJsonArray m_extraWireArray;
};

#endif // REMOVESELECTIONCOMMAND_H
