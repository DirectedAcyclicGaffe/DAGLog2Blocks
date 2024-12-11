#include "AddSelectionCommand.h"
#include "../Corkboard.h"
#include "../CorkboardBackend.h"
#include "MalamuteCore/IdMaker.h"

AddSelectionCommand::AddSelectionCommand(const QJsonObject &obj, CorkboardBackend *cb, QPointF center, bool changeUuids):
    m_cb(cb),
    m_obj(obj),
    m_center(center),
    m_changeUuids(changeUuids)
{

}

void AddSelectionCommand::undo()
{
    //Delete the wires.
    QJsonArray wiresJsonArray = m_obj["w"].toArray();
    for(QJsonValueRef wire: wiresJsonArray)
        m_cb->removeWire(wire.toObject()["id"].toInt());

    //Delete the casings
    QJsonArray casingsJsonArray = m_obj["c"].toArray();
    for(QJsonValueRef casing: casingsJsonArray)
        m_cb->removeCasingBackend(casing.toObject()["id"].toInt());
}

void AddSelectionCommand::redo()
{
    if(m_changeUuids)
    {
        //The selection to be added. As well as the new arrays which will take their place.
        QJsonArray casingsJsonArray = m_obj["c"].toArray();
        QJsonArray newCasingsJsonArray;
        QJsonArray wiresJsonArray = m_obj["w"].toArray();
        QJsonArray newWiresJsonArray;

        std::vector<int> oldCasingIds;
        std::vector<int> newCasingIds;

        double top = 1e100; double bottom = -1e100;
        double left = 1e100; double right = -1e100;

        ///This code find the bounds of the selection to be added.
        for (QJsonValueRef casing : casingsJsonArray)
        {
            QJsonObject obj = casing.toObject();
            QJsonObject positionJson = obj["cp"].toObject();

            top = qMin(top, positionJson["y"].toDouble());
            bottom = qMax(bottom, positionJson["y"].toDouble() + 100.0);
            left = qMin(left,positionJson["x"].toDouble());
            right = qMax(right, positionJson["x"].toDouble() + 100.0);
        }

        // Calculates how to move the selection.
        QPointF oldCenter = QPointF((left + right)/2.0,(top + bottom)/2.0);
        QPointF displacement = m_center - oldCenter;

        //Loop over the new casings.
        for (QJsonValueRef casing : casingsJsonArray)
        {
            QJsonObject obj = casing.toObject();

            // Change the Uuid, record the change.
            oldCasingIds.push_back(obj["id"].toInt());
            int newId = Magic<IdMaker>::cast().getID();
            obj["id"] = newId;
            newCasingIds.push_back(obj["id"].toInt());

            //Shift the idea to the new position.
            QJsonObject positionJson = obj["cp"].toObject();
            positionJson["x"] = positionJson["x"].toDouble() + displacement.x();
            positionJson["y"] = positionJson["y"].toDouble() + displacement.y();
            obj["cp"] = positionJson;

            //Create the new idea. Store what is done for future undo/redo operations.
            m_cb->corkboard()->createCasing(obj);
            newCasingsJsonArray.append(obj);
        }

        // Set up the wires.
        for (QJsonValueRef wire : wiresJsonArray)
        {
            bool ok1 = false; bool ok2 = false;
            QJsonObject obj = wire.toObject();

            int inId = obj["iid"].toInt();
            for(size_t i = 0; i < oldCasingIds.size(); i++)
            {
                if(oldCasingIds[i] == inId)
                {
                    ok1 = true;
                    obj["iid"] = newCasingIds[i];
                    break;
                }
            }

            int outId = obj["oid"].toInt();
            for(size_t i = 0; i < oldCasingIds.size(); i++)
            {
                if(oldCasingIds[i] == outId)
                {
                    ok2 = true;
                    obj["oid"] = newCasingIds[i];
                    break;
                }
            }

            if(ok1 && ok2)
            {
                obj["id"] = Magic<IdMaker>::cast().getID();
                m_cb->createWire(obj);
                newWiresJsonArray.append(obj);
            }
        }

        //Set up the new arrays.
        m_obj["c"] = newCasingsJsonArray;
        m_obj["w"] = newWiresJsonArray;
        m_changeUuids = false;
    }
    else
    {
        // An additional undo operation might call this.
        QJsonArray casingsJsonArray = m_obj["c"].toArray();
        QJsonArray wiresJsonArray = m_obj["w"].toArray();

        //Loop over and create new casings.
        for (QJsonValueRef casing : casingsJsonArray)
            m_cb->createIdea(casing.toObject());

        // loop over and create new wires.
        for (QJsonValueRef wire : wiresJsonArray)
            m_cb->createWire(wire.toObject());
    }
}
