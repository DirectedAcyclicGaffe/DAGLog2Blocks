#ifndef RESIZECASINGCOMMAND_H
#define RESIZECASINGCOMMAND_H

#include "UndoCommand.h"
#include <QPointF>

class Corkboard;

class ResizeCasingCommand : public UndoCommand
{
public:
    ResizeCasingCommand(const int& id, Corkboard* corkboard, QPointF oldSize, QPointF newSize);

    void undo() override;
    void redo() override;

private:
    Corkboard* m_corkboard;
    int m_id;
    QPointF m_oldSize;
    QPointF m_newSize;
    bool m_actionNeeded;
};

#endif // RESIZECASINGCOMMAND_H
