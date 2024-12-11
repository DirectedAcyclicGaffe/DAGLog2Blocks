#include "PlugEvents.h"

#include "Wire.h"
#include "Casing.h"
#include "InheritedClasses/Idea.h"
#include "CasingBackend.h"

PlugEvents::PlugEvents(CasingBackend &casingBackend, WireBackend& wireBackend) :
    m_casingBackend(&casingBackend),
    m_wireBackend(&wireBackend)
{

}

bool PlugEvents::tryPlugIn() const
{
    int plugNumber = canPlugIn();
    if(plugNumber == -1)
        return false;
    else
    {
        finishPlugIn(plugNumber);
        return true;
    }
}

int PlugEvents::canPlugIn() const
{
    PlugType requiredPlug = m_wireBackend->plugTypeNeeded();
    if (requiredPlug == PlugType::NA)
        return false;

    QPointF wirePoint = wireEndPos(requiredPlug);
    int plugNumber = plugNumberAtPoint(requiredPlug, wirePoint);
    if (plugNumber == -1) // No Plug found.
        return -1;

    if (!plugIsEmpty(requiredPlug, plugNumber))
        return -1; // Something else already plugged in.

    int wireDataType = m_wireBackend->dataType(oppositeEnd(requiredPlug));

    int candidateCasingDataType;
    auto const &idea = m_casingBackend->idea();
    if(requiredPlug == PlugType::IN)
        candidateCasingDataType = idea->attributes().dataTypesIn()[plugNumber];
    else
        candidateCasingDataType = idea->attributes().dataTypesOut()[plugNumber];

    if(candidateCasingDataType != wireDataType)
        return -1; // Wrong data type.

    CasingBackend* casingBackend = m_wireBackend->getCasing(oppositeEnd(requiredPlug));
    if (casingBackend == m_casingBackend) // No Connecting to the same casing.
        return -1;

    return plugNumber;
}

void PlugEvents::finishPlugIn(int plugNumber) const
{
    PlugType requiredPlug = m_wireBackend->plugTypeNeeded();
    m_wireBackend->setCasingToPlug(m_casingBackend, requiredPlug, plugNumber);

    int inPlugNumber = m_wireBackend->getPlug(PlugType::IN);
    int outPlugNumber = m_wireBackend->getPlug(PlugType::OUT);
    auto inCasingBackend = m_wireBackend->getCasing(PlugType::IN);
    auto outCasingBackend = m_wireBackend->getCasing(PlugType::OUT);
    
    const QColor color = outCasingBackend->casing().outPlugLabelColors().at(outPlugNumber);
    m_wireBackend->getWire()->setColor(color);
    inCasingBackend->casing().setInPlugColor(inPlugNumber, color);
    
    if(requiredPlug == PlugType::OUT)
        outCasingBackend->addWire(PlugType::OUT, outPlugNumber, *m_wireBackend);
    else
        inCasingBackend->addWire(PlugType::IN, inPlugNumber, *m_wireBackend);

    IdeaStatus status = outCasingBackend->idea()->status();
    if(status == IdeaStatus::Working || status == IdeaStatus::Warning || status == IdeaStatus::DisplayWarning)
        outCasingBackend->sendDataNewWire(m_wireBackend->id());
}


bool PlugEvents::unPlug(PlugType disconnectingPlug) const
{
    int plugNumber = m_wireBackend->getPlug(disconnectingPlug);

    //This might cause a problem if unplugging is implemented at PlugType::IN. Might delete another wire.
    m_casingBackend->getWiresByType(disconnectingPlug)[plugNumber].clear();

    m_wireBackend->clearCasingBackend(disconnectingPlug);
    m_wireBackend->setRequiredPlugType(disconnectingPlug);
    m_wireBackend->getWire()->grabMouse();

    return true;
}

bool PlugEvents::plugHover()
{
    PlugType requiredPlug = m_wireBackend->plugTypeNeeded();
    if (requiredPlug == PlugType::NA)
        return false;

    //Find the plug number. else, return out of this.
    int plugNumber = plugNumberAtPoint(requiredPlug, wireEndPos(requiredPlug));
    if (plugNumber == -1)
        return false;

    //Things cannot connect to themselves. This checks that. No Recursion! No red circle? Might put in later.
    CasingBackend* casingBackend = m_wireBackend->getCasing(oppositeEnd(requiredPlug));
    if (casingBackend == m_casingBackend)
        return false;

    //Cannot connect to a plug that already has a wire. Red Circle!
    if (!plugIsEmpty(requiredPlug, plugNumber))
    {
        m_casingBackend->addRedLight(requiredPlug, plugNumber);
        return true;
    }

    //Wrong data type.
    int wireDataType = m_wireBackend->dataType(oppositeEnd(requiredPlug));
    auto const &idea = m_casingBackend->idea();

    int candidateCasingDataType;
    if(requiredPlug == PlugType::IN)
        candidateCasingDataType = idea->attributes().dataTypesIn()[plugNumber];
    else
        candidateCasingDataType = idea->attributes().dataTypesOut()[plugNumber];

    if(candidateCasingDataType != wireDataType)
    {
        m_casingBackend->addRedLight(requiredPlug, plugNumber);
        return true;
    }

    //Correct data type. Draw a green circle.
    m_casingBackend->addGreenLight(requiredPlug, plugNumber);
    return true;
}

// ------------------ util functions below


QPointF PlugEvents::wireEndPos(PlugType plugType) const
{
    auto wire = m_wireBackend->getWire();

    QPointF wirePos = wire->getEndPoint(plugType);
    QPointF corkboardPos = QPointF(wirePos.x() + wire->x(), wirePos.y() + wire->y());

    return corkboardPos;
}

int PlugEvents::plugNumberAtPoint(PlugType plugType, QPointF const & scenePoint) const
{
    double x_shift = m_casingBackend->casing().x();
    double y_shift = m_casingBackend->casing().y();
    QPointF casingPos = QPointF(scenePoint.x()-x_shift,scenePoint.y()-y_shift);
    int plugNumber = m_casingBackend->casing().mouseOverPlug(plugType, casingPos, 3.0);

    return plugNumber;
}

bool PlugEvents::plugIsEmpty(PlugType plugType, int plugNumber) const
{
    if (m_casingBackend->getWiresAtPlug(plugType, plugNumber).empty())
        return true;

    return (plugType == PlugType::OUT);
}

PlugType PlugEvents::oppositeEnd(const PlugType type) const
{
    if(type == PlugType::IN)
        return PlugType::OUT;
    else if (type == PlugType::OUT)
        return PlugType::IN;
    else
        return PlugType::NA;
}
