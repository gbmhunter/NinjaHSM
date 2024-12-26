#include <gtest/gtest.h>

#include "NinjaHsm.hpp"

class TestEvent {
public:
  TestEvent(int value) : value(value) {}
  int value;
};

class TestHsm : public NinjaHsm::StateMachine<TestEvent> {
public:
    TestHsm() : 
      state1(
        "State1",
        std::bind(&TestHsm::State1_Entry, this),
        std::bind(&TestHsm::State1_Event, this),
        std::bind(&TestHsm::State1_Exit, this),
        nullptr
      ) {
      addState(&state1);
    }

    NinjaHsm::State state1;

private:

  void State1_Entry() {
    std::cout << "State1_Entry" << std::endl;
  }

  void State1_Event() {
    std::cout << "State1_Event" << std::endl;
  }

  void State1_Exit() {
    std::cout << "State1_Exit" << std::endl;
  }
};

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
    // Create test HSM
    TestHsm hsm;

    // Transition to state
    hsm.initialTransitionTo(&hsm.state1);

    // Make sure we are in state1
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1);
}
