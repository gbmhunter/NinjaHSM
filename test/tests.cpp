#include <gtest/gtest.h>

#include "NinjaHsm.hpp"

class TestHsm : public NinjaHsm::StateMachine {
public:
    TestHsm() : state1("State1", std::bind(&TestHsm::State1_Entry, this), std::bind(&TestHsm::State1_Event, this), std::bind(&TestHsm::State1_Exit, this)) {
      addState(state1);
    }

private:

  NinjaHsm::State state1;

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
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);


  // Create test HSM
  TestHsm hsm;

  // Transition to state
  // hsm.transitionTo(hsm.state1);
}
