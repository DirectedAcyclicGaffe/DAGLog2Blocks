#ifndef PLUG_H
#define PLUG_H

enum PlugType
{
    NA,
    IN,
    OUT
};

class UnplugWiresInfo
{
public:
    UnplugWiresInfo(int plugNumber, PlugType plugType) :
        m_plugNumber(plugNumber),
        m_plugType(plugType)
    {

    }
    int plugNumber() const {return m_plugNumber;};
    PlugType isInput() const {return m_plugType;};

private:
    int m_plugNumber;
    PlugType m_plugType;
};


#endif

