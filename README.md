# NinjaHSM

NinjaHSM is a simple hierarchical state machine library written in C++ which has been designed to work on embedded systems.

## Features

### General

* Easy installation if you use CMake via `FetchContent`.
* No dependencies other than C++14.
* No dynamic memory allocation.
* Suitable for embedded systems.

### State Features

* Hierarchical state are supported by providing the parent state when creating a child state.
* Each state can have an `entry()`, `event()` and `exit()` method.
* The `event()` method takes a user defined `Event` object as a parameter.
* Events bubble up to parent states if the child state does not handle the event.
* Events stop bubbling up if a child state calls `transitionTo()` or `eventHandled()`.

## Installation

If you are using CMake, you can add NinjaHSM to your project by using `FetchContent` in your `CMakeLists.txt` file like so:

```cmake
include(FetchContent)
FetchContent_Declare(NinjaHSM
    GIT_REPOSITORY https://github.com/gbmhunter/NinjaHSM.git
    GIT_TAG v0.1.0 # This can be a hash, tag or branch.
)
FetchContent_MakeAvailable(NinjaHSM)

# Then later once you've defined your target
target_link_libraries(your_app NinjaHsm)
```

Then all you need to do is include the NinjaHSM header in your source files and begin using it (see the Usage section below for more details):

```cpp
#include "NinjaHSM/NinjaHSM.hpp"
```

## Usage

Firstly, include the NinjaHSM header in your source files. It's also a good idea to use the `using namespace NinjaHSM;` directive to make the code less verbose.

```cpp
#include "NinjaHSM/NinjaHSM.hpp"

using namespace NinjaHSM;
```

### Defining Events

You will need to define an `Event` class. This will be used to pass events to the `onEvent()` method.

```cpp
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

class Event {
public:
    Event(EventId id) : id(id) {}
    EventId id;
    union {
        EventWithData1 data1;
        EventWithData2 data2;
    };
};
```

## Creating A State Machine

Now we have our events defined, we can create a state machine. You need to make your own state machine class which inherits from `NinjaHSM::StateMachine<Event>`. This of your class AS A state machine, but the `NinjaHSM::StateMachine` class provides a lot of the boilerplate code for you, including the transition logic.

In your class, you will need to create a `State<Event>` object for each state. These are initilized in the constructor, and take in a human readable name, `entry()`, `event()`, `exit()` functions, and a pointer to the parent state (`nullptr` if it has no parent).

```cpp
class MyStateMachine : public StateMachine<Event> {
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
    State<Event> state1;
    State<Event> state1a;
    State<Event> state2;

    //============================================================================================//
    // state1
    //============================================================================================//

    void state1_entry() {}
    void state1_event(const Event * event) {
        if (event->id == EventId::EVENT_WITH_NO_DATA) {
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
    void state1a_event(const Event * event) {}
    void state1a_exit() {}

    //============================================================================================//
    // state2
    //============================================================================================//

    void state2_entry() {}
    void state2_event(const Event * event) {}
    void state2_exit() {}
};
```

The template parameter `<Event>` is just so that rather than passing in the events a `void *`, we have proper typing.

Inheriting from `StateMachine<Event>` gives you the following methods available on your state machine class:

* `initialTransitionTo(State<Event> * state)`: Perform an initial transition to the provided state. Designed to be called from the constructor of your state machine class. We used that above in the constructor of `MyStateMachine`.
* `handleEvent(Event * event)`: Pass an event to the state machine. The state machine will call then current state's `onEvent()` function. This is designed to be called from outside your state machine, and is how you pass events (and data) to the state machine. We use the below in our `main()` function.
* `transitionTo(State<Event> * state)`: Transition to the provided state. This is designed to be called from within a state's `onEvent()` method (or in rarer cases, from within a state's `entry()` or `exit()` methods --- see below for more details).
* `getCurrentState()`: Gets the current state.
* `eventHandled()`: Call this from within a state's `onEvent()` method when you have handled an event. This prevents the event from bubbling up to parent states.

Now we can create an instance of our state machine and start sending events to it:

```cpp
int main() {
    printf("basic_example running...\n");
    // Create the state machine
    MyStateMachine stateMachine;
    printf("State machine created. state is: %s\n", stateMachine.getCurrentState()->name);

    // Send an event with no data, this causes a change in state (see the state1_event() method).
    Event event1(EventId::EVENT_WITH_NO_DATA);
    stateMachine.handleEvent(&event1);
    printf("Event 1 handled. state is now: %s\n", stateMachine.getCurrentState()->name);

    // Send an event with data, this doesn't change the state, but just shows how you can
    // react to data passed in with the event.
    Event event2(EventId::EVENT_WITH_DATA_1);
    event2.data1.data = 123;
    stateMachine.handleEvent(&event2);
    printf("Event 2 handled. state is now: %s\n", stateMachine.getCurrentState()->name);
    
    return 0;
}
```

This prints:

```text
basic_example running...
State machine created. state is: State1
Event 1 handled. state is now: State1a
Got event with data: 123
Event 2 handled. state is now: State1a
```

This example is available in the `examples/basic_example/` directory.

### Entry and Exit Guards

NinjaHSM supports the ability to call `transitionTo()` from within a state's `entry()` or `exit()` methods. One use case for this is to add entry guards to states, protecting them from being entered if certain conditions are not met. When a state can be transitions to from many other states, it is less code repetition to add the guard to the entry function of the destination state rather than in every source state. An example would be a rocket's LAUNCH state. You could add guards to LAUNCH's `entry()` method to ensure the rocket has no issues before it can be launched, and if it does, transition to an ERROR state instead.

Because `entry()` and `exit()` states are only every called by `transitionTo()` in the first place, calling `transitionTo()` from within `entry()` or `exit()` is recursive. NinjaHSM supports this, with the following rules:

**entry() Rules:**

* If state A calls `transitionTo(stateB)` from within it's `entry()` method, AND `stateB` IS NOT a child of `stateA`, then NinjaHSM asummes `stateA` was not entered, and does not call `stateA`'s `exit()` method.
* If state A calls `transitionTo(stateB)` from within it's `entry()` method, AND `stateB` IS a child of `stateA`, then NinjaHSM asummes `stateA` was entered succesfully, and does not call `stateA`'s `exit()` method or recall `stateA`'s `entry()` method.

**exit() Rules:**

* If state A calls `transitionTo(stateB)` from within it's `exit()` method, AND `stateB` IS NOT a child of `stateA`, then NinjaHSM asummes `stateA` was exited succesfully, and does not call `stateA`'s `exit()` method again.
* If state A calls `transitionTo(stateB)` from within it's `exit()` method, AND `stateB` IS a child of `stateA`, then NinjaHSM asummes `stateA` was not exited, and does not call `stateA`'s `entry()` method again.

Hopefully these rules make intuitive sense!

### Others

See the `examples/` and `test/` directories for more examples on how to use NinjaHSM.
