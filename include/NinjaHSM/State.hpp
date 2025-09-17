#pragma once

#include <etl/delegate.h>

namespace NinjaHSM {

template <typename EventType>
class State {
public:
    using EntryDelegate = etl::delegate<void()>;
    using EventDelegate = etl::delegate<void(const EventType&)>;
    using ExitDelegate = etl::delegate<void()>;

    State(
        const char * name,
        EntryDelegate entry,
        EventDelegate event,
        ExitDelegate exit,
        State * parent) :
            name(name),
            entry(entry),
            event(event),
            exit(exit),
            parent(parent) {}

    const char * name;
    EntryDelegate entry;
    EventDelegate event;
    ExitDelegate exit;

    State * parent = nullptr;
}; // class State

} // namespace NinjaHSM
