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

Then all you need to do is include the NinjaHSM header in your source files:

```cpp
#include "NinjaHSM/NinjaHSM.hpp"
```
