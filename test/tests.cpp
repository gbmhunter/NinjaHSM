#include <gtest/gtest.h>

#include <gmock/gmock.h>

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
      ),
      state2(
        "State2",
        std::bind(&TestHsm::State2_Entry, this),
        std::bind(&TestHsm::State2_Event, this),
        std::bind(&TestHsm::State2_Exit, this),
        nullptr
      ) {
      addState(&state1);
      addState(&state2);
    }

    NinjaHsm::State state1;
    NinjaHsm::State state2;

private:

    virtual void State1_Entry() {
        std::cout << "State1_Entry" << std::endl;
    }

    virtual void State1_Event() {
        std::cout << "State1_Event" << std::endl;
    }

    virtual void State1_Exit() {
        std::cout << "State1_Exit" << std::endl;
    }

    virtual void State2_Entry() {
        std::cout << "State2_Entry" << std::endl;
    }

    virtual void State2_Event() {
        std::cout << "State2_Event" << std::endl;
    }

    virtual void State2_Exit() {
        std::cout << "State2_Exit" << std::endl;
    }
};

class MockTestHsm : public TestHsm {
public:
    MOCK_METHOD0(State1_Entry, void());
    MOCK_METHOD0(State1_Event, void());
    MOCK_METHOD0(State1_Exit, void());
};

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
    // Create test HSM
    MockTestHsm hsm;

    EXPECT_CALL(hsm, State1_Entry());

    // Transition to state
    hsm.initialTransitionTo(&hsm.state1);

    // Make sure we are in state1
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1);
}
