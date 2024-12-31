# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- `transitionTo()` can now be called from within a state's `entry()` or `exit()` methods (i.e. `transitionTo()` recursion).
- Added more tests around `transitionTo()` recursion.
- Set the maximum recursion count for `transitionTo()` to 50.
- Removed debug `std::cout` statements.
- Added example project under `examples/`.
- Added more installation and examples to the README.

## [0.1.0] - 2024-12-28

Initial release.

### Added

- Added CMake support via `FetchContent`.
- Hierarchical states are supported.
- States can have an `entry()`, `event()` and `exit()` method.
- Events bubble up to parent states if the child state does not handle the event.
- Events stop bubbling up if a child state calls `transitionTo()` or `eventHandled()`.
- Added a test suite.

[unreleased]: https://github.com/gbmhunter/NinjaHSM/compare/v0.1.0...HEAD
[0.1.0]: https://github.com/gbmhunter/NinjaHSM/releases/tag/v0.1.0
