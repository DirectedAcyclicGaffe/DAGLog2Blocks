#include "Idea.h"

#include <QClipboard>
#include <QMimeData>

QJsonObject Idea::save() const
{
    QJsonObject saveJson;
    saveJson["ic"] = m_iColor.name();

    QJsonArray arr;
    for(int i = 0; i < m_outPlugLabelColors.size(); i++)
        arr.append(m_outPlugLabelColors.at(i).name());
    saveJson["c"] = arr;

    return saveJson;
}

void Idea::load(const QJsonObject &obj)
{
    QJsonValue v = obj["c"];
    if(!v.isUndefined())
    {
        m_outPlugLabelColors.clear();
        QJsonArray arr = v.toArray();
        for(int i = 0; i < arr.size(); i++)
            m_outPlugLabelColors.push_back(QColor(arr.at(i).toString()));
        emit outPlugLabelColorsChanged(m_outPlugLabelColors);
    }

    v = obj["ic"];
    if(!v.isUndefined())
    {
        m_iColor = QColor(v.toString());
        emit iColorChanged(m_iColor);
    }
}

void Idea::onNewDataIn(std::shared_ptr<DataType>, int)
{

}

std::shared_ptr<DataType> Idea::dataOut(int)
{
    return nullptr;
}

void Idea::resetDisplay()
{

}

void Idea::updateOnIColor()
{

}

void Idea::updateOnInPlugLabelColor()
{

}

void Idea::updateOnOutPlugLabelColor()
{

}

void Idea::finishSetupQmlContext()
{

}

QString Idea::statusMessage() const
{
    return m_statusMessage;
}

IdeaStatus Idea::status() const
{
    return m_status;
}

bool Idea::valid() const
{
    if(m_status == IdeaStatus::Working || m_status == IdeaStatus::Warning || m_status == IdeaStatus::DisplayWarning)
        return true;
    else
        return false;
}

bool Idea::minimized() const
{
    return m_minimized;
}

QColor Idea::iColor() const
{
    return m_iColor;
}

QList<QColor> Idea::inPlugLabelColors() const
{
    return m_inPlugLabelColors;
}

QList<QColor> Idea::outPlugLabelColors() const
{
    return m_outPlugLabelColors;
}


void Idea::setStatus(IdeaStatus status, QString message)
{
    m_status = status;
    m_statusMessage = message;
    emit statusChanged();

    if(status == IdeaStatus::StandBy || status == IdeaStatus::Error || status == IdeaStatus::Fatal)
    {
        emit clearDataAllPlugs();
        emit validChanged(false);
    }
    else
    {
        emit validChanged(true);
    }
}

void Idea::setDefaultStatus()
{
    IdeaStatus oldStatus = m_status;
    m_status = IdeaStatus::StandBy;
    m_statusMessage = "Standing By";
    emit statusChanged();
    emit validChanged(false);

    //Clear data if necessary
    if(oldStatus == IdeaStatus::Working || oldStatus == IdeaStatus::Warning || oldStatus == IdeaStatus::DisplayWarning)
        emit clearDataAllPlugs();
}

void Idea::sendTextToClipboard(QString text)
{
    QMimeData* mimeData = new QMimeData;
    mimeData->setText(text);

    QClipboard* clipboard = QGuiApplication::clipboard();
    clipboard->setMimeData(mimeData);
}

void Idea::setMinimized(bool minimized)
{
    if (m_minimized == minimized)
        return;

    m_minimized = minimized;
    emit minimizedChanged(m_minimized);
}

void Idea::setIdeaColor(QColor iColor)
{
    if(m_iColor == iColor)
        return;

    m_iColor = iColor;
    emit iColorChanged(m_iColor);
}

void Idea::setInPlugLabelColor(int i, QColor color)
{
    if(m_inPlugLabelColors.at(i) == color)
        return;

    m_inPlugLabelColors[i] = color;
    emit inPlugLabelColorsChanged(m_inPlugLabelColors);

    updateOnInPlugLabelColor();
}

void Idea::setOutPlugLabelColor(int i, QColor color)
{
    if(m_outPlugLabelColors.at(i) == color)
        return;

    m_outPlugLabelColors[i] = color;
    emit outPlugLabelColorsChanged(m_outPlugLabelColors);

    updateOnOutPlugLabelColor();
}

const QColor Idea::getInPlugLabelColor(int i)
{
    return m_inPlugLabelColors.at(i);
}

const QColor Idea::getOutPlugLabelColor(int i)
{
    return m_outPlugLabelColors.at(i);
}

void Idea::prepareInPlugLabelColors(int n, QColor color)
{
    m_inPlugLabelColors.reserve(n);
    for(int i = 0; i < n; i++)
        m_inPlugLabelColors.push_back(color);
}

void Idea::prepareOutPlugLabelColors(QList<QColor> colorList)
{
    m_outPlugLabelColors = colorList;
    emit outPlugLabelColorsChanged(m_outPlugLabelColors);
}
