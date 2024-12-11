#ifndef PLUGIN_H
#define PLUGIN_H

#include "../MalamuteCore/InheritedClasses/MalamutePlugin.h"

class Plugin : public MalamutePlugin
{
    Q_OBJECT

public:
    QString PluginName() override;
    void registerIdeas(std::shared_ptr<IdeaRegistry> ideaRegistry) override;
    void registerDataTypeAttributes(std::shared_ptr<DataTypeRegistry> dataTypeRegistry) override;

    QString defaultFile() override;
};

#endif // PLUGIN_H
