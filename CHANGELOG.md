# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

n/a

## [1.3.0] - 2026-05-29

### Added

- Added the `makeState()` helper for constructing `State` objects with much less boilerplate (you name the event type once and each handler method once, instead of writing out the three ETL delegate `::create<>` expressions by hand).
- Added three optional observer hooks to `StateMachine`: `setTransitionObserver()` (notified after every state `entry()`/`exit()`), `setUnhandledEventObserver()` (notified when an event bubbles past the top of the hierarchy unhandled), and `setErrorObserver()` (notified on internal errors such as exceeding the max transition recursion depth, which previously failed silently). All are ETL delegates, unset by default, and have zero cost when unset.
- Added a PlatformIO manifest (`library.json`) and an Arduino manifest (`library.properties`) so NinjaHSM can be consumed from those ecosystems.
- Added a Conan recipe (`conanfile.py`) with a `test_package`, so NinjaHSM can be consumed via Conan. The ETL dependency is pulled in transitively. A CI job validates the recipe with `conan create`.
- Added CI jobs that cross-compile the headers for a bare-metal ARM Cortex-M target (`arm-none-eabi`) and that compile them with C++ exceptions and RTTI disabled, plus a `NINJAHSM_BUILD_COMPILE_CHECK` CMake option and an `arm-none-eabi` toolchain file under `cmake/`.

### Changed

- **Moved the headers from `include/NinjaHSM/` to `src/NinjaHSM/`** so a single layout works for CMake, PlatformIO and the Arduino IDE (which only compiles headers from a library's `src/` directory). The public include path is unchanged: `#include <NinjaHSM/NinjaHSM.hpp>`.
- Updated the example and README to construct states using `makeState()`.
- Changed the basic example to build against the in-repo NinjaHSM source (`add_subdirectory`) instead of a published release tag, so CI exercises the current source.
- Reworked the README: added status badges, a statechart diagram, a comparison table against other C++ state machine libraries, and PlatformIO/Arduino install instructions. Corrected the C++ standard (C++17), the install snippet (linking the `NinjaHSM` target propagates includes and the ETL dependency), and the max-recursion reference (`MAX_RECURSION_COUNT` in `StateMachine.hpp`).

### Removed

- Removed the unused `<functional>` include from `NinjaHSM.hpp` (left over from the pre-ETL-delegate days).

## [1.2.0] - 2025-09-17

### Changed

- Changed to use ETL delegates instead of lambdas to provide class methods as callbacks in the `State` class. This completely removes the chance of dynamic memory allocation, whereas before we were relying on the small number of captures optimization of lambdas.
- Changed the examples and tests to use state machine composition instead of inheritance.

## [1.1.0] - 2025-07-08

### Added

- Added more info to the README.

### Changed

- Change use of pointers to references where appropriate.
- Added `const` qualifiers where appropriate.

### Fixed

- Fixed issue where it was tricky to include this as a library in an embedded project due to this project pulling in GoogleTest for the test suite. Added a `NINJAHSM_BUILD_TESTS` option (which defaults to off) to allow the user to control whether the test suite is built.

## [1.0.0] - 2024-12-31

### Added

- `transitionTo()` can now be called from within a state's `entry()` or `exit()` methods (i.e. `transitionTo()` recursion).
- Added more tests around `transitionTo()` recursion.
- Set the maximum recursion count for `transitionTo()` to 50.
- Added example project under `examples/`.
- Added more installation and examples to the README.
- Setup GitHub Actions to build and run tests on Linux.

### Changed

- Removed debug `std::cout` statements.

## [0.1.0] - 2024-12-28

Initial release.

### Added

- Added CMake support via `FetchContent`.
- Hierarchical states are supported.
- States can have an `entry()`, `event()` and `exit()` method.
- Events bubble up to parent states if the child state does not handle the event.
- Events stop bubbling up if a child state calls `transitionTo()` or `eventHandled()`.
- Added a test suite.

[unreleased]: https://github.com/gbmhunter/NinjaHSM/compare/v1.2.0...HEAD
[1.2.0]: https://github.com/gbmhunter/NinjaHSM/compare/v1.1.0...v1.2.0
[1.1.0]: https://github.com/gbmhunter/NinjaHSM/compare/v1.0.0...v1.1.0
[1.0.0]: https://github.com/gbmhunter/NinjaHSM/compare/v0.1.0...v1.0.0
[0.1.0]: https://github.com/gbmhunter/NinjaHSM/releases/tag/v0.1.0
