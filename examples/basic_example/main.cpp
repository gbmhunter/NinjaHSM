#include <iostream>
#include <functional>

#include <NinjaHSM/NinjaHSM.hpp>

using namespace NinjaHSM;

//==============================================================================
// Events
//==============================================================================

// Used to identify what event we have.
enum class EventId {
    EVENT_WITH_NO_DATA,
    EVENT_WITH_DATA_1,
    EVENT_WITH_DATA_2,
};

class EventWithData1 {
public:
    EventWithData1(uint32_t data) : data(data) {}
    uint32_t data;
};

class EventWithData2 {
public:
    EventWithData2(int64_t data) : data(data) {}
    int64_t data;
};

// This is the "wrapper" class.
class Event {
public:
    Event(EventId id) : id(id) {}
    EventId id;
    union {
        EventWithData1 data1;
        EventWithData2 data2;
    };
};

//==============================================================================
// State Machine
//==============================================================================

class MyStateMachine : public StateMachine {
public:
    MyStateMachine() : StateMachine(),
    state1(
        "State1",
        std::bind(&MyStateMachine::state1_entry, this),
        std::bind(&MyStateMachine::state1_event, this, std::placeholders::_1),
        std::bind(&MyStateMachine::state1_exit, this),
        nullptr
      ),
      state1a(
        "State1a",
        std::bind(&MyStateMachine::state1a_entry, this),
        std::bind(&MyStateMachine::state1a_event, this, std::placeholders::_1),
        std::bind(&MyStateMachine::state1a_exit, this),
        &state1
      ),
      state2(
        "State2",
        std::bind(&MyStateMachine::state2_entry, this),
        std::bind(&MyStateMachine::state2_event, this, std::placeholders::_1),
        std::bind(&MyStateMachine::state2_exit, this),
        nullptr
      ) {
        transitionTo(&state1);
    }

private:
    State state1;
    State state1a;
    State state2;

    //============================================================================================//
    // state1
    //============================================================================================//

    void state1_entry() {}
    void state1_event(const void * eventRaw) {
        const Event * event = static_cast<const Event *>(eventRaw);
        if (event->id == EventId::EVENT_WITH_NO_DATA) {
            // Let's go to a different state!
            transitionTo(&state1a);
        }
        else if (event->id == EventId::EVENT_WITH_DATA_1) {
            // We know which event we got, so we can safely access the union member
            EventWithData1 const * eventWithData = &event->data1;
            printf("Got event with data: %d\n", eventWithData->data);
            eventHandled(); // Prevents event from bubbling up to parent states
        }
    }
    void state1_exit() {}

    //============================================================================================//
    // state1/state1a
    //============================================================================================//

    void state1a_entry() {}
    void state1a_event(const void * eventRaw) {}
    void state1a_exit() {}

    //============================================================================================//
    // state2
    //============================================================================================//

    void state2_entry() {}
    void state2_event(const void * eventRaw) {}
    void state2_exit() {}
};

int main() {
    printf("basic_example running...\n");
    // Create the state machine
    MyStateMachine stateMachine;
    printf("State machine created. state is: %s\n", stateMachine.getCurrentState()->name);

    // Send an event with no data, this causes a change in state (see the state1_event() method).
    Event event1(EventId::EVENT_WITH_NO_DATA);
    stateMachine.handleEvent(static_cast<void*>(&event1));
    printf("Event 1 handled. state is now: %s\n", stateMachine.getCurrentState()->name);

    // Send an event with data, this doesn't change the state, but just shows how you can
    // react to data passed in with the event.
    Event event2(EventId::EVENT_WITH_DATA_1);
    event2.data1.data = 123;
    stateMachine.handleEvent(static_cast<void*>(&event2));
    printf("Event 2 handled. state is now: %s\n", stateMachine.getCurrentState()->name);
    
    return 0;
}