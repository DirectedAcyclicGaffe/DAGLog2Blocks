#ifndef DELETECASINGCOMMAND_H
#define DELETECASINGCOMMAND_H

#include "UndoCommand.h"
#include <QJsonObject>
#include <QJsonArray>

class CorkboardBackend;

class DeleteCasingCommand: public UndoCommand
{
public:
    DeleteCasingCommand(const QJsonObject& obj, CorkboardBackend* corkboard);

    void undo() override;
    void redo() override;

private:
    CorkboardBackend* m_cb;
    QJsonObject m_obj;
    QJsonArray m_extraWireArray;
};

#endif // DELETECASINGCOMMAND_H
