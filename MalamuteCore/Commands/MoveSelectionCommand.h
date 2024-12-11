#ifndef MOVESELECTIONCOMMAND_H
#define MOVESELECTIONCOMMAND_H

#include "UndoCommand.h"
#include <vector>
#include <QPointF>

class Corkboard;

class MoveSelectionCommand: public UndoCommand
{
public:
    MoveSelectionCommand(std::vector<int> idList, std::vector<QPointF> oldPosList, std::vector<QPointF> newPosList,
                     Corkboard* corkboard, bool actionNeeded);

    void undo() override;
    void redo() override;

private:
    Corkboard* m_corkboard;
    std::vector<int> m_idList;
    std::vector<QPointF> m_oldPosList;
    std::vector<QPointF> m_newPosList;
    bool m_actionNeeded;
};

#endif // MOVESELECTIONCOMMAND_H
