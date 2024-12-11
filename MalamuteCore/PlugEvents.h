#ifndef PLUGEVENTS_H
#define PLUGEVENTS_H

#include "CasingBackend.h"
#include "WireBackend.h"

/// Handles the interaction between a wire and a casing.
/// Created and destroyed frequently.

class PlugEvents
{
public:
    PlugEvents(CasingBackend& casingBackend, WireBackend& wireBackend);

    bool tryPlugIn() const;
    int canPlugIn() const;
    void finishPlugIn(int plugNumber) const;
    bool unPlug(PlugType disconnectingPlug) const;
    bool plugHover(); //Return lets you know if stoplight added or not

private:
    PlugType oppositeEnd(PlugType const type) const;
    QPointF wireEndPos(PlugType plugType) const;
    int plugNumberAtPoint(PlugType plugType, QPointF const &p) const;
    bool plugIsEmpty(PlugType plugType, int plugNumber) const;

    CasingBackend* m_casingBackend;
    WireBackend* m_wireBackend;
};

#endif
