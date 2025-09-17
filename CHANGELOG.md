# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

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

[unreleased]: https://github.com/gbmhunter/NinjaHSM/compare/v1.1.0...HEAD
[1.1.0]: https://github.com/gbmhunter/NinjaHSM/compare/v1.0.0...v1.1.0
[1.0.0]: https://github.com/gbmhunter/NinjaHSM/compare/v0.1.0...v1.0.0
[0.1.0]: https://github.com/gbmhunter/NinjaHSM/releases/tag/v0.1.0
