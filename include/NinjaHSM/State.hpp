#pragma once

#include <functional>

namespace NinjaHSM {

template <typename Event>
class State {
public:
    State(
        const char * name,
        std::function<void()> entry,
        std::function<void(const Event *)> event,
        std::function<void()> exit,
        State * parent) :
            name(name),
            entry(entry),
            event(event),
            exit(exit),
            parent(parent) {}

    const char * name;
    std::function<void()> entry;
    std::function<void(const Event *)> event;
    std::function<void()> exit;

    State * parent = nullptr;
}; // class State

} // namespace NinjaHSM
