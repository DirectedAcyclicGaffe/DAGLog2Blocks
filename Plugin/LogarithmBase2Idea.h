#ifndef LOGARITHMBASE2IDEA_H
#define LOGARITHMBASE2IDEA_H

#include "OneToOneIdeaTemplate.h"

struct LogarithmBase2Attributes : public IdeaAttributes
{
    LogarithmBase2Attributes();
};

class LogarithmBase2Idea : public OneToOneIdeaTemplate
{
    Q_OBJECT

public:
    explicit LogarithmBase2Idea();
    const IdeaAttributes attributes() override;

private:
    void validate() override;
    void compute(double n1);
};

#endif // LOGARITHMBASE2IDEA_H
