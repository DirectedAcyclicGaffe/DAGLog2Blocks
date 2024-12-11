#include "UndoStack.h"

UndoStack::UndoStack() :
    m_pos(0)
{

}

void UndoStack::push(UndoCommand *command)
{
    while(m_commands.length() > m_pos)
        m_commands.pop_back();
    m_commands.push_back(command);
    command->redo();
    m_pos++;

}

void UndoStack::undo()
{
    if(m_pos > 0)
    {
        m_commands[m_pos - 1]->undo();
        m_pos--;
    }
}

void UndoStack::redo()
{
    if(m_pos < m_commands.length() )
    {
        m_pos++;
        m_commands[m_pos - 1]->redo();
    }
}

void UndoStack::clear()
{
    m_pos = 0;
    m_commands.clear();;
}

bool UndoStack::canUndo() const
{
    return m_pos > 0;
}

bool UndoStack::canRedo() const
{
    return m_pos < m_commands.length();
}
