#pragma once

#include <functional>

namespace NinjaHsm {

class State {
public:
    State(
        const char * name,
        std::function<void()> entry,
        std::function<void()> event,
        std::function<void()> exit,
        State * parent);

    const char * name;
    std::function<void()> entry;
    std::function<void()> event;
    std::function<void()> exit;

    State * parent = nullptr;
}; // class State

} // namespace NinjaHsm