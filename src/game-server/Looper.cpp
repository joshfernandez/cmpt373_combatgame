#include "Looper.hpp"
#include <thread>
#include <iostream>
#include <glog/logging.h>

void Looper::run(Loop& loop) {
    //based off http://gameprogrammingpatterns.com/game-loop.html
    auto previous = clock::now();
    bool exit = false;
    while (!exit) {
        auto current = clock::now();
        auto delta = std::chrono::duration_cast<TimePeriod>(current - previous);
        previous = current;

        loop.processInputs(exit);

        if (delta >= targetUpdateTime) {
            //too slow, catch up
            while (delta >= targetUpdateTime) {
                loop.update();
                delta -= targetUpdateTime;
            }
        } else {
            //too fast, sleep
            loop.update();
            auto elapsed = std::chrono::duration_cast<TimePeriod>(clock::now() - previous);
            if (elapsed < targetUpdateTime) {
                std::this_thread::sleep_for(targetUpdateTime - elapsed);
            }
        }

        google::FlushLogFiles(google::INFO);
        google::FlushLogFiles(google::WARNING);
    }
}
