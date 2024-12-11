#ifndef UNDOSTACK_H
#define UNDOSTACK_H

#include <QList>
#include "UndoCommand.h"

class UndoStack
{
public:
    UndoStack();

    void push(UndoCommand* command);
    void undo();
    void redo();

    void clear();

    bool canUndo() const;
    bool canRedo() const;

private:
    QList<UndoCommand*> m_commands;
    int m_pos;
};

#endif // UNDOSTACK_H
