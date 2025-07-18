#include <iostream>
#include <functional>
#include <variant>

#include <NinjaHSM/NinjaHSM.hpp>

using namespace NinjaHSM;

//==============================================================================
// Events
//==============================================================================

/**
 * Wrap all the events in a namespace for ease of use.
 */
namespace Events {

/**
 * Events don't have to have any data associated with them, like this one!
 */
struct TimerExpired {};

/**
 * This event has some data associated with it.
 */
struct ButtonPressed {
    uint32_t buttonId;
};

/**
 * This is like a typesafe "union" of all the events.
 */
using Generic = std::variant<TimerExpired, ButtonPressed>;

}

//==============================================================================
// State Machine
//==============================================================================

class MyStateMachine : public StateMachine<Events::Generic> {
public:
    MyStateMachine() : StateMachine(),
    m_state1(
        "State1",
        [this]() { state1_entry(); },
        [this](Events::Generic const & event) { state1_event(event); },
        [this]() { state1_exit(); },
        nullptr
      ),
      m_state1a(
        "State1a",
        [this]() { state1a_entry(); },
        [this](Events::Generic const & event) { state1a_event(event); },
        [this]() { state1a_exit(); },
        &m_state1
      ),
      m_state2(
        "State2",
        [this]() { state2_entry(); },
        [this](Events::Generic const & event) { state2_event(event); },
        [this]() { state2_exit(); },
        nullptr
      ) {
        initialTransitionTo(m_state1);
    }

private:
    //============================================================================================//
    // state1
    //============================================================================================//

    void state1_entry() {}
    void state1_event(Events::Generic const & event) {
        if (std::holds_alternative<Events::TimerExpired>(event)) {
            // Let's go to a different state!
            transitionTo(m_state1a);
        }
        else if (std::holds_alternative<Events::ButtonPressed>(event)) {
            // We know which event we got, so we can safely access the union member
            Events::ButtonPressed const & buttonPressed = std::get<Events::ButtonPressed>(event);
            printf("Got event with data: %d\n", buttonPressed.buttonId);
            eventHandled(); // Prevents event from bubbling up to parent states
        }
    }
    void state1_exit() {}

    //============================================================================================//
    // state1/state1a
    //============================================================================================//

    void state1a_entry() {}
    void state1a_event(Events::Generic const & event) {}
    void state1a_exit() {}

    //============================================================================================//
    // state2
    //============================================================================================//

    void state2_entry() {}
    void state2_event(Events::Generic const & event) {}
    void state2_exit() {}

    //============================================================================================//
    // State variables
    //============================================================================================//

    State<Events::Generic> m_state1;
    State<Events::Generic> m_state1a;
    State<Events::Generic> m_state2;
};

int main() {
    printf("basic_example running...\n");
    // Create the state machine
    MyStateMachine stateMachine;
    printf("State machine created. state is: %s\n", stateMachine.getCurrentState()->name);

    // Send an event with no data, this causes a change in state (see the state1_event() method).
    Events::TimerExpired event1;
    stateMachine.handleEvent(event1);
    printf("Event 1 handled. state is now: %s\n", stateMachine.getCurrentState()->name);

    // Send an event with data, this doesn't change the state, but just shows how you can
    // react to data passed in with the event.
    Events::ButtonPressed event2;
    event2.buttonId = 123;
    stateMachine.handleEvent(event2);
    printf("Event 2 handled. state is now: %s\n", stateMachine.getCurrentState()->name);
    
    return 0;
}