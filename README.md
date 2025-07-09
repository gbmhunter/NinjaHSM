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
    GIT_TAG v1.0.1 # This can be a hash, tag or branch.
)
FetchContent_MakeAvailable(NinjaHSM)

# Then later once you've defined your target
target_link_libraries(your_app PRIVATE NinjaHsm)
target_include_directories(your_app PRIVATE ${NinjaHSM_SOURCE_DIR}/include)
```

Then all you need to do is include the NinjaHSM header in your source files and begin using it (see the Usage section below for more details):

```cpp
#include "NinjaHSM/NinjaHSM.hpp"
```

If you want to see `FetchContent` in action, see the `examples/basic_example/` CMake project.

## Usage

Firstly, include the NinjaHSM header in your source files. It's also a good idea to use the `using namespace NinjaHSM;` directive to make the code less verbose.

```cpp
#include "NinjaHSM/NinjaHSM.hpp"

using namespace NinjaHSM;
```

### Defining Events

You will need to define an `Event` class. This will be used to pass events to the `onEvent()` method, so your state machine can react to things. But you will normally want to define more than one event type. The best way to create a generic "typesafe union" of all the events using `std::variant` as shown below.

```cpp
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
```

## Creating A State Machine

Now we have our events defined, we can create a state machine. You need to make your own state machine class which inherits from `NinjaHSM::StateMachine`. This of your class AS A state machine, but the `NinjaHSM::StateMachine` class provides a lot of the boilerplate code for you, including the transition logic.

NOTE: You don't actually need to inherit from `NinjaHSM::StateMachine` if you don't want to. You can just use composition and create a `NinjaHSM::StateMachine` object and call it's methods directly. Inheritance does feel like the more intuitive way though since you have to define all the states and state functions in your class anyway.

In your class, you will need to create a `NinjaHSM::State` object for each state. These are initilized in the constructor, and take in a human readable name, `entry()`, `event()`, `exit()` functions, and a pointer to the parent state (`nullptr` if it has no parent). Use the pointer to the parent state to create a hierarchical state machine (HSM).

The following example creates a basic hierarchical state machine with three states, one of which is a child state. The hierarchy looks like this:

```text
State1
   |-- State1a
State2
```

Lambas are used instead of `std::bind` to provide class methods as callbacks since they use no dynamic memory allocation when there are small number of captures (small buffer optimization). Here is the C++ code:

```cpp
#include <NinjaHSM/NinjaHSM.hpp>

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
```

Notice how in the `state1_event()` method, we listen to some events and take actions (like transitioning to a different state, or handling data passed in with the event).

Inheriting from `StateMachine` gives you the following methods available on your state machine class:

* `initialTransitionTo(State& state)`: Perform an initial transition to the provided state. Designed to be called from the constructor of your state machine class. We used that above in the constructor of `MyStateMachine`.
* `handleEvent(const Event& event)`: Pass an event to the state machine. The state machine will call then current state's `onEvent()` function. This is designed to be called from outside your state machine, and is how you pass events (and data) to the state machine. We use the below in our `main()` function.
* `transitionTo(State& state)`: Call this to transition to the provided state. This is designed to be called from within a state's `onEvent()` method (or in rarer cases, from within a state's `entry()` or `exit()` methods --- see below for more details). The transition is NOT queued, it happens immediately. When `transitionTo()` returns, the transition has completed.
* `State* getCurrentState()`: Gets the current state. May be `nullptr` if the state machine has not been initialized yet.
* `eventHandled()`: Call this from within a state's `onEvent()` method when you have handled an event. This prevents the event from bubbling up to parent states.

Now we can create an instance of our state machine and start sending events to it:

```cpp
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

```cpp
void stateRunning_entry() {
    // Example of an entry guard.
    if (notReady) {
        transitionTo(stateIdle);
        return;
    }
    // Do normal entry logic here.
}
```

Because `entry()` and `exit()` states are only every called by `transitionTo()` in the first place, calling `transitionTo()` from within `entry()` or `exit()` is recursive. NinjaHSM supports this, with the following rules:

**entry() Rules:**

* If `stateA` calls `transitionTo(stateB)` from within it's `entry()` method, AND `stateB` IS NOT a child of `stateA`, then NinjaHSM asummes `stateA` was not entered, and does not call `stateA`'s `exit()` method.
* If `stateA` calls `transitionTo(stateB)` from within it's `entry()` method, AND `stateB` IS a child of `stateA`, then NinjaHSM asummes `stateA` was entered succesfully, and does not call `stateA`'s `exit()` method or recall `stateA`'s `entry()` method.

**exit() Rules:**

* If `stateA` calls `transitionTo(stateB)` from within it's `exit()` method, AND `stateB` IS NOT a child of `stateA`, then NinjaHSM asummes `stateA` was exited succesfully, and does not call `stateA`'s `exit()` method again.
* If `stateA` calls `transitionTo(stateB)` from within it's `exit()` method, AND `stateB` IS a child of `stateA`, then NinjaHSM asummes `stateA` was not exited, and does not call `stateA`'s `entry()` method again.

Hopefully these rules make intuitive sense! There is also a max. recursion depth of 50 (set by `MAX_RECURSION_DEPTH` in `NinjaHSM.hpp`) to prevent infinite recursion in the case of bugs (e.g. if you unconditionally call `transitionTo(stateB)` in `stateA`'s `entry()` method, and unconditionally call `transitionTo(stateA)` in `stateB`'s `entry()` method).

### Others

See the `examples/` and `test/` directories for more examples on how to use NinjaHSM.

## Building the Test Suite

This project defines a CMake option `NINJAHSM_BUILD_TESTS` (which defaults to off) to allow the user to control whether the test suite is built. This defaults to off because if embedded projects use this as a library, they will likely encounter build errors if they try and pull in GoogleTest and/or build the test application.

To build the test suite, you can use the following CMake command:

```bash
cmake -DNINJAHSM_BUILD_TESTS=ON ..
```

Alternatively, if you are including NinjaHSM using `FetchContent`, you can set the option in your `CMakeLists.txt` file like so:

```cmake
include(FetchContent)
set(NINJAHSM_BUILD_TESTS ON CACHE BOOL "" FORCE)
FetchContent_Declare(NinjaHSM
    GIT_REPOSITORY https://github.com/gbmhunter/NinjaHSM.git
    GIT_TAG v1.0.1 # This can be a hash, tag or branch.
)
FetchContent_MakeAvailable(NinjaHSM)
target_link_libraries(your_app NinjaHsm)
```
