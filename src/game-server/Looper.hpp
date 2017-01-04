#ifndef ADVENTURE2016_LOOPER_HPP
#define ADVENTURE2016_LOOPER_HPP

#include <chrono>

class Loop {
public:
    virtual void processInputs(bool& quit) = 0;
    virtual void update() = 0;
};

class Looper {
public:
    using clock = std::chrono::high_resolution_clock;
    using TimePeriod = std::chrono::nanoseconds;
    const int nsPerSecond = 1000000000;

    Looper(unsigned int targetUpdatesPerSecond)
            : targetUpdateTime{TimePeriod{nsPerSecond / targetUpdatesPerSecond}} {};
    void run(Loop& loop);
private:
    TimePeriod targetUpdateTime;
};


#endif //ADVENTURE2016_LOOPER_HPP
