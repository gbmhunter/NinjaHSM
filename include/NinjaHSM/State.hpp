#pragma once

#include <functional>

namespace NinjaHSM {

template <typename EventType>
class State {
public:
    State(
        const char * name,
        std::function<void()> entry,
        std::function<void(const EventType&)> event,
        std::function<void()> exit,
        State * parent) :
            name(name),
            entry(entry),
            event(event),
            exit(exit),
            parent(parent) {}

    const char * name;
    std::function<void()> entry;
    std::function<void(const EventType&)> event;
    std::function<void()> exit;

    State * parent = nullptr;
}; // class State

} // namespace NinjaHSM
