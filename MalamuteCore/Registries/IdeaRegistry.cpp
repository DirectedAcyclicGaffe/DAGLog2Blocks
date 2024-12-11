#include "IdeaRegistry.h"

std::unique_ptr<Idea> IdeaRegistry::createIdea(QString const &listName)
{
    auto it = m_creationFunctions.find(listName);
    if (it != m_creationFunctions.end())
        return it->second();

    return nullptr;
}

QString IdeaRegistry::iconPath(QString listName)
{
    return m_iconPaths[listName];
}

QStringList IdeaRegistry::registeredIdeaNames()
{
    return m_registeredIdeas;
}
