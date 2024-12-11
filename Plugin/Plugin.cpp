#include "Plugin.h"

#include "NumberDataType.h"
#include "NumberDisplayIdea.h"
#include "TileLogarithmBase2Idea.h"
#include "NumberInputIdea.h"
#include "LogBase2VisualArea.h"

//#include "LabeledNumberDisplayIdea.h"
//#include "LabeledNumberInputIdea.h"

QString Plugin::PluginName()
{
    return "Logarithm Base 2 Demo";
}

void Plugin::registerIdeas(std::shared_ptr<IdeaRegistry> ideaRegistry)
{
    ideaRegistry->registerIdea<NumberInputIdea>();
    //ideaRegistry->registerIdea<LabeledNumberInputIdea>();
    ideaRegistry->registerIdea<TileLogarithmBase2Idea>();
    ideaRegistry->registerIdea<NumberDisplayIdea>();
    //ideaRegistry->registerIdea<LabeledNumberDisplayIdea>();

    qmlRegisterType<LogBase2VisualArea>("com.malamute.tileCalculator", 1, 0, "LogBase2VisualArea");
}

void Plugin::registerDataTypeAttributes(std::shared_ptr<DataTypeRegistry> dataTypeRegistry)
{
    dataTypeRegistry->registerDataTypeAttributes<NumberDataTypeAttributes>();
}

QString Plugin::defaultFile()
{
    return "qrc:/DemoFiles/InitialMutt.mutt";
}
