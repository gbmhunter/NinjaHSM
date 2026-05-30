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
 * All three handler slots are mandatory, but any of them may be @c nullptr if the state does not
 * need that handler. This avoids having to write empty stub methods. For example, a leaf state
 * that only reacts to events (no entry/exit work) is:
 *
 * @code
 * State<Event> m_state1 = makeState<Event,
 *     nullptr,                          // no entry()
 *     &MyStateMachine::state1_event,
 *     nullptr>("State1", *this);        // no exit()
 * @endcode
 *
 * A nullptr slot leaves the corresponding delegate unbound; the state machine skips calling it
 * (see StateMachine). An unbound event() handler simply lets the event bubble up to the parent.
 *
 * @tparam EventType The state machine's event type.
 * @tparam Entry     Pointer to the member function to use as the entry() handler (e.g.
 *                   &MyStateMachine::state1_entry, signature void()), or nullptr for no entry().
 * @tparam Event     Pointer to the member function to use as the event() handler (signature
 *                   void(const EventType&)), or nullptr for no event().
 * @tparam Exit      Pointer to the member function to use as the exit() handler (signature
 *                   void()), or nullptr for no exit().
 * @tparam Self      The class that owns the handler methods. Deduced from @p self.
 * @param[in] name   Human readable name for the state.
 * @param[in] self   The instance the handler methods are called on.
 * @param[in] parent Pointer to the parent state, or nullptr (the default) for a top-level state.
 * @return A fully constructed State.
 */
namespace detail {

/**
 * Turn a handler member-function-pointer non-type template argument into a bound delegate, or
 * an unbound (default constructed) delegate if the argument is nullptr. Used by makeState() so
 * that any handler slot can be omitted by passing nullptr.
 *
 * @tparam Delegate The delegate type to produce (e.g. State<E>::EntryDelegate).
 * @tparam Ptr      The member function pointer to bind, or nullptr.
 * @tparam Self     The class that owns the member function. Deduced from @p self.
 * @param[in] self  The instance the member function is called on.
 * @return A bound delegate, or an unbound delegate if Ptr is nullptr.
 */
template <typename Delegate, auto Ptr, typename Self>
Delegate bindOrEmpty(Self & self) {
    if constexpr (Ptr == nullptr) {
        (void)self;
        return Delegate();
    } else {
        return Delegate::template create<Self, Ptr>(self);
    }
}

} // namespace detail

template <typename EventType, auto Entry, auto Event, auto Exit, typename Self>
State<EventType> makeState(const char * name, Self & self, State<EventType> * parent = nullptr) {
    return State<EventType>(
        name,
        detail::bindOrEmpty<typename State<EventType>::EntryDelegate, Entry>(self),
        detail::bindOrEmpty<typename State<EventType>::EventDelegate, Event>(self),
        detail::bindOrEmpty<typename State<EventType>::ExitDelegate, Exit>(self),
        parent);
}

} // namespace NinjaHSM
