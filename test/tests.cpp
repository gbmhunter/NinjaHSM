#include <gtest/gtest.h>

#include "NinjaHsm.hpp"

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);

  // Create state
  NinjaHsm::State state;

  // Create state machine
  NinjaHsm::StateMachine stateMachine;

  // Add state to state machine
  stateMachine.addState(state);

  // Transition to state
  stateMachine.transitionTo(state);
}
