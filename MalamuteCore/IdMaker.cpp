#include "IdMaker.h"

IdMaker::IdMaker() {}

int IdMaker::getID()
{
    lastID += 1;
    return lastID;
}
