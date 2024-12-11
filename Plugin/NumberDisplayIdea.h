#ifndef NUMBERDISPLAYIDEA_H
#define NUMBERDISPLAYIDEA_H

#include "../MalamuteCore/InheritedClasses/Idea.h"

struct NumberDisplayAttributes : public IdeaAttributes
{
    NumberDisplayAttributes();
};

class NumberDisplayIdea : public Idea
{
    Q_OBJECT

public:
    NumberDisplayIdea();
    const IdeaAttributes attributes() override;

    void onNewDataIn(std::shared_ptr<DataType> data, int) override;

signals:
    void displayNumber(QString value);
    void clearDisplay();
};

#endif
