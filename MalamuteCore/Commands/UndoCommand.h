#ifndef UNDOCOMMAND_H
#define UNDOCOMMAND_H


class UndoCommand
{
public:
    virtual void undo() = 0;
    virtual void redo() = 0;
};

#endif // UNDOCOMMAND_H
