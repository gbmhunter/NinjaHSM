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

/**
 * Helper for constructing a State with much less boilerplate.
 *
 * Instead of spelling out the three ETL delegate ::create<> expressions by hand:
 *
 * @code
 * State<Event> m_state1(
 *     "State1",
 *     State<Event>::EntryDelegate::create<MyStateMachine, &MyStateMachine::state1_entry>(*this),
 *     State<Event>::EventDelegate::create<MyStateMachine, &MyStateMachine::state1_event>(*this),
 *     State<Event>::ExitDelegate::create<MyStateMachine, &MyStateMachine::state1_exit>(*this),
 *     nullptr);
 * @endcode
 *
 * you can write:
 *
 * @code
 * State<Event> m_state1 = makeState<Event,
 *     &MyStateMachine::state1_entry,
 *     &MyStateMachine::state1_event,
 *     &MyStateMachine::state1_exit>("State1", *this);
 * @endcode
 *
 * Each method is named exactly once (so IDE "go to definition" and grep still work, unlike a
 * macro), the owning class name is not repeated, and the only type you must spell out is the
 * event type. @p Self is deduced from @p self.
 *
 * @tparam EventType The state machine's event type.
 * @tparam Entry     Pointer to the member function to use as the entry() handler, e.g.
 *                   &MyStateMachine::state1_entry. Must have signature void().
 * @tparam Event     Pointer to the member function to use as the event() handler. Must have
 *                   signature void(const EventType&).
 * @tparam Exit      Pointer to the member function to use as the exit() handler. Must have
 *                   signature void().
 * @tparam Self      The class that owns the handler methods. Deduced from @p self.
 * @param[in] name   Human readable name for the state.
 * @param[in] self   The instance the handler methods are called on.
 * @param[in] parent Pointer to the parent state, or nullptr (the default) for a top-level state.
 * @return A fully constructed State.
 */
template <typename EventType, auto Entry, auto Event, auto Exit, typename Self>
State<EventType> makeState(const char * name, Self & self, State<EventType> * parent = nullptr) {
    return State<EventType>(
        name,
        State<EventType>::EntryDelegate::template create<Self, Entry>(self),
        State<EventType>::EventDelegate::template create<Self, Event>(self),
        State<EventType>::ExitDelegate::template create<Self, Exit>(self),
        parent);
}

} // namespace NinjaHSM
