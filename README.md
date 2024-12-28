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

If you are using CMake, you can add NinjaHSM to your project by adding the following to your `CMakeLists.txt` file:

```cmake
include(FetchContent)
FetchContent_Declare(NinjaHSM
    GIT_REPOSITORY https://github.com/gbmhunter/NinjaHSM.git
    GIT_TAG v0.1.0
)
FetchContent_MakeAvailable(NinjaHSM)
```
