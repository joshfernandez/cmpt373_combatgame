#include "Resets.hpp"

Resets::Resets(std::string &action,
               std::string &actionID,
               std::string &areaID,
               std::string &stateOfDoor,
               int slot,
               int limit)

                : action(action)
                , actionID(actionID)
                , areaID(areaID)
                , stateOfDoor(stateOfDoor)
                , slot(slot)
                , limit(limit)
                {

                }

std::string Resets::getAction() const {
    return action;
}

std::string Resets::getActionID() const{
    return actionID;
}

std::string Resets::getAreaID() const{
    return areaID;
}

std::string Resets::getStateOfDoor() const{
    return stateOfDoor;
}

int Resets::getLimit() const{
    return limit;
}

int Resets::getSlot() const{
    return slot;
}

void Resets::setStateOfDoor(const std::string& newState){
    this->stateOfDoor = newState;
}
void Resets::setLimit(int newLimit){
    this->limit = newLimit;
}
