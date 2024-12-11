#ifndef TILELOGARITHMBASE2IDEA_H
#define TILELOGARITHMBASE2IDEA_H

#include "LogarithmBase2Idea.h"

struct TileLogarithmBase2Attributes : public IdeaAttributes
{
    TileLogarithmBase2Attributes();
};

class TileLogarithmBase2Idea : public LogarithmBase2Idea
{
    Q_OBJECT

public:
    explicit TileLogarithmBase2Idea();
    const IdeaAttributes attributes() override;

    void resetDisplay() override;
};

#endif // TILELOGARITHMBASE2IDEA_H
