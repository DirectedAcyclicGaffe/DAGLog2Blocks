#ifndef ADDCASINGCOMMAND_H
#define ADDCASINGCOMMAND_H

#include "UndoCommand.h"
#include <QJsonObject>

class CorkboardBackend;

class AddCasingCommand: public UndoCommand
{
public:
    AddCasingCommand(const QJsonObject& obj, CorkboardBackend* cb);

    void undo() override;
    void redo() override;

private:
    CorkboardBackend* m_cb;
    QJsonObject m_obj;
};

#endif // ADDCASINGCOMMAND_H
