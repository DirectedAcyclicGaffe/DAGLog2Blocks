#ifndef ADDSELECTIONCOMMAND_H
#define ADDSELECTIONCOMMAND_H

#include "UndoCommand.h"
#include <QJsonObject>
#include <QPointF>

class CorkboardBackend;

class AddSelectionCommand: public UndoCommand
{
public:
    AddSelectionCommand(const QJsonObject& obj, CorkboardBackend* cb, QPointF center, bool changeUuids);

    void undo() override;
    void redo() override;

private:
    CorkboardBackend* m_cb;
    QJsonObject m_obj;
    QPointF m_center;
    bool m_changeUuids;
};

#endif // ADDSELECTIONCOMMAND_H
