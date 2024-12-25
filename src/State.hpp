#pragma once

#include <functional>

namespace NinjaHsm {

class State {
public:
    State(const char * name, std::function<void()> entry, std::function<void()> event, std::function<void()> exit);

    const char * name;
    std::function<void()> entry;
    std::function<void()> event;
    std::function<void()> exit;
}; // class State

} // namespace NinjaHsm