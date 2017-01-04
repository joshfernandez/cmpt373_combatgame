#ifndef STATUS_EFFECT_HPP_
#define STATUS_EFFECT_HPP_

#include <time.h>
#include <string>
#include <ctime>

enum class StatusType {
    UNDEFINED,
    BODYSWAP,
    PIG_LATIN,
};

class StatusEffect {

public:

    StatusEffect(int durationInSeconds);
    time_t getEndTime() const;
    virtual StatusType getType();

private:

    time_t calcEndTime(int duration);
    time_t endTime;

};

class BodySwapStatus : public StatusEffect {

public:

    BodySwapStatus(int durationInSeconds, const std::string& swappedID);
    virtual StatusType getType();
    std::string getSwappedID();

private:

    std::string swappedID;

};

class PigLatinSwapStatus : public StatusEffect {
public:
    PigLatinSwapStatus(int durationInSeconds) : StatusEffect{durationInSeconds} {}
    virtual StatusType getType() {return StatusType::PIG_LATIN;}
};

#endif