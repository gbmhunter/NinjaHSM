#include <functional>

#include "State.hpp"

namespace NinjaHsm {

State::State(const char * name, std::function<void()> entry, std::function<void()> event, std::function<void()> exit) : name(name), entry(entry), event(event), exit(exit) {
}

} // namespace NinjaHsm