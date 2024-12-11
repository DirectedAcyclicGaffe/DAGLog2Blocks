#ifndef IDMAKER_H
#define IDMAKER_H

class IdMaker
{
public:
    IdMaker();

    int getID();

private:
    int lastID = 0;
};

#endif // IDMAKER_H
