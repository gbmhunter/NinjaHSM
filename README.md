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

class EventWrapper {
public:
    EventWrapper(EventId id) : id(id) {}
    EventId id;
    union {
        EventWithData1 data1;
        EventWithData2 data2;
    };
};
```

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

See the `test/` directory for more examples on how to use NinjaHSM.