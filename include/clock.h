#ifndef __CLOCK_H_
#define __CLOCK_H_

#include <chrono>
#include <unordered_set>

#include <base_unique_singleton.h>
#include <interface_observable.h>
#include <interface_observer.h>

class Clock
{
public:
    using clock = std::chrono::steady_clock;
    using time_point = std::chrono::time_point<clock>;
    using duration = std::chrono::duration<long double>;

    Clock();

    void        start();
    void        update();
    void        stop();
    void        reset();
    long double elapsed_time();
    bool        running() const;

private:
    time_point                      start_time__;
    time_point                      end_time__;
    bool                            running__ = false;
};

#endif
