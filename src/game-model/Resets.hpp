#ifndef RESETS_H
#define RESETS_H

#include <string>

class Resets {
public:

    Resets(  std::string& action,
             std::string& actionID,
             std::string& areaID,
             std::string& stateOfDoor,
             int slot,
             int limit

    );

    std::string getAction() const;
    std::string getActionID() const;
    std::string getAreaID() const;
    std::string getStateOfDoor() const;
    int getLimit() const;
    int getSlot() const;

    void setStateOfDoor(const std::string& state);
    void setLimit(int newLimit);

private:
    std::string action;
    std::string actionID;
    std::string areaID;
    std::string stateOfDoor;
    int slot;
    int limit;

};





#endif //ADVENTURE2016_RESETS_H
