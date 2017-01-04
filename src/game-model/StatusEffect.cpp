#include "game/StatusEffect.hpp"
#include <glog/logging.h>

StatusEffect::StatusEffect(int durationInSeconds) {
    CHECK(durationInSeconds > 0) << "Invalid duration";
    calcEndTime(durationInSeconds);
}

time_t StatusEffect::getEndTime() const {
    return this->endTime;
}

time_t StatusEffect::calcEndTime(int duration) {
    time_t timer;
    time(&timer);

    endTime = timer + duration;
}

StatusType StatusEffect::getType() {
    return StatusType::UNDEFINED;
}

BodySwapStatus::BodySwapStatus(int durationInSeconds, const std::string& id) :
        swappedID(id), StatusEffect(durationInSeconds)
{

}

StatusType BodySwapStatus::getType() {
    return StatusType::BODYSWAP;
}

std::string BodySwapStatus::getSwappedID() {
    return this->swappedID;
}