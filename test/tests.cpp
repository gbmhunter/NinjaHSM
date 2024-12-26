#include <gtest/gtest.h>

#include <gmock/gmock.h>

#include "NinjaHsm.hpp"

enum class EventId {
    GO_TO_STATE_2,
};

class TestEvent {
public:
  TestEvent(EventId id) : id(id) {}
  EventId id;
};

class TestHsm : public NinjaHsm::StateMachine<TestEvent> {
public:
    TestHsm() : 
      state1(
        "State1",
        std::bind(&TestHsm::State1_Entry, this),
        std::bind(&TestHsm::State1_Event, this, std::placeholders::_1),
        std::bind(&TestHsm::State1_Exit, this),
        nullptr
      ),
      state2(
        "State2",
        std::bind(&TestHsm::State2_Entry, this),
        std::bind(&TestHsm::State2_Event, this, std::placeholders::_1),
        std::bind(&TestHsm::State2_Exit, this),
        nullptr
      ) {
      addState(&state1);
      addState(&state2);
    }

    NinjaHsm::State<TestEvent> state1;
    NinjaHsm::State<TestEvent> state2;

    uint32_t state1EntryCallCount = 0;
    uint32_t state1EventCallCount = 0;
    uint32_t state1ExitCallCount = 0;

    uint32_t state2EntryCallCount = 0;
    uint32_t state2EventCallCount = 0;
    uint32_t state2ExitCallCount = 0;

private:

    virtual void State1_Entry() {
        std::cout << "State1_Entry" << std::endl;
        state1EntryCallCount++;
    }

    virtual void State1_Event(const TestEvent * event) {
        std::cout << "State1_Event" << std::endl;

        if (event->id == EventId::GO_TO_STATE_2) {
            transitionTo(&state2);
        }
        state1EventCallCount++;
    }

    virtual void State1_Exit() {
        std::cout << "State1_Exit" << std::endl;
        state1ExitCallCount++;
    }

    virtual void State2_Entry() {
        std::cout << "State2_Entry" << std::endl;
        state2EntryCallCount++;
    }

    virtual void State2_Event(const TestEvent * event) {
        std::cout << "State2_Event" << std::endl;
        state2EventCallCount++;
    }

    virtual void State2_Exit() {
        std::cout << "State2_Exit" << std::endl;
        state2ExitCallCount++;
    }
};

// class MockTestHsm : public TestHsm {
// public:
//     MOCK_METHOD0(State1_Entry, void());
//     MOCK_METHOD0(State1_Event, void());
//     MOCK_METHOD0(State1_Exit, void());
// };

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
    // Create test HSM
    TestHsm hsm;

    // Transition to state
    hsm.initialTransitionTo(&hsm.state1);

    // Make sure we are in state1
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1);
    EXPECT_EQ(hsm.state1EntryCallCount, 1);

    // Send event to transition to state2
    TestEvent event(EventId::GO_TO_STATE_2);
    hsm.handleEvent(&event);

    // Make sure we are in state2
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state2);
    EXPECT_EQ(hsm.state1ExitCallCount, 1);
    EXPECT_EQ(hsm.state2EntryCallCount, 1);
}