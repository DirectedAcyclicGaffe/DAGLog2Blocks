#ifndef DATATYPEATTRIBUTES_H
#define DATATYPEATTRIBUTES_H

#include <QString>
#include <QStringList>

class DataTypeAttributes
{
public:
    QString name() const;
    QString abbreviation() const;

    int id() const;
    void setID(int value);

protected:
    QString m_name;
    QString m_abbreviation;

private:
    int m_id = -1;
};

#endif // DATATYPEATTRIBUTES_H
