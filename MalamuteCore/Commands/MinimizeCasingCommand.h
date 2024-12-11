#ifndef MINIMIZECASINGCOMMAND_H
#define MINIMIZECASINGCOMMAND_H

#include "UndoCommand.h"

class Corkboard;

class MinimizeCasingCommand : public UndoCommand
{
public:
    MinimizeCasingCommand(const int& id, Corkboard* corkboard, bool oldState, bool actionNeeded = false);

    void undo() override;
    void redo() override;

private:
    Corkboard* m_corkboard;
    int m_id;
    bool m_oldState;
    bool m_actionNeeded;
};

#endif // MINIMIZECASINGCOMMAND_H
