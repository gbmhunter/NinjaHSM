#include <gtest/gtest.h>

#include <gmock/gmock.h>

#include "NinjaHsm.hpp"

enum class EventId {
    GO_TO_STATE_1,
    GO_TO_STATE_1A,
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
            std::bind(&TestHsm::state1_entry, this),
            std::bind(&TestHsm::state1_event, this, std::placeholders::_1),
            std::bind(&TestHsm::state1_exit, this),
            nullptr
      ),
      state1A(
        "State1A",
        std::bind(&TestHsm::state1a_entry, this),
        std::bind(&TestHsm::state1a_event, this, std::placeholders::_1),
        std::bind(&TestHsm::state1a_exit, this),
        &state1
      ),
      state2(
        "State2",
        std::bind(&TestHsm::state2_entry, this),
        std::bind(&TestHsm::state2_event, this, std::placeholders::_1),
        std::bind(&TestHsm::state2_exit, this),
        nullptr
      ) {
        addState(&state1);
        addState(&state1A);
        addState(&state2);
    }

    NinjaHsm::State<TestEvent> state1;
    NinjaHsm::State<TestEvent> state1A;
    NinjaHsm::State<TestEvent> state2;

    uint32_t state1EntryCallCount = 0;
    uint32_t state1EventCallCount = 0;
    uint32_t state1ExitCallCount = 0;

    uint32_t state1aEntryCallCount = 0;
    uint32_t state1aEventCallCount = 0;
    uint32_t state1aExitCallCount = 0;

    uint32_t state2EntryCallCount = 0;
    uint32_t state2EventCallCount = 0;
    uint32_t state2ExitCallCount = 0;

private:

    //========================================================================//
    // state1
    //========================================================================//

    virtual void state1_entry() {
        std::cout << "state1_entry" << std::endl;
        state1EntryCallCount++;
    }

    virtual void state1_event(const TestEvent * event) {
        std::cout << "state1_event" << std::endl;

        if (event->id == EventId::GO_TO_STATE_1) {
            transitionTo(&state1);
        }
        else if (event->id == EventId::GO_TO_STATE_1A) {
            transitionTo(&state1A);
        }
        state1EventCallCount++;
    }

    virtual void state1_exit() {
        std::cout << "state1_exit" << std::endl;
        state1ExitCallCount++;
    }

    //========================================================================//
    // state1/state1a
    //========================================================================//

    virtual void state1a_entry() {
        std::cout << "state1a_entry" << std::endl;
        state1aEntryCallCount++;
    }

    virtual void state1a_event(const TestEvent * event) {
        std::cout << "state1a_event" << std::endl;

        if (event->id == EventId::GO_TO_STATE_2) {
            transitionTo(&state2);
        }
        state1aEventCallCount++;
    }

    virtual void state1a_exit() {
        std::cout << "state1a_exit" << std::endl;
        state1aExitCallCount++;
    }

    //========================================================================//
    // state2
    //========================================================================//

    virtual void state2_entry() {
        std::cout << "state2_entry" << std::endl;
        state2EntryCallCount++;
    }

    virtual void state2_event(const TestEvent * event) {
        std::cout << "state2_event" << std::endl;
        state2EventCallCount++;
    }

    virtual void state2_exit() {
        std::cout << "state2_exit" << std::endl;
        state2ExitCallCount++;
    }
};

// class MockTestHsm : public TestHsm {
// public:
//     MOCK_METHOD0(state1_entry, void());
//     MOCK_METHOD0(state1_event, void());
//     MOCK_METHOD0(state1_exit, void());
// };

// Demonstrate some basic assertions.
TEST(HsmTests, ChildStateTransitionsWork) {
    // Create test HSM
    TestHsm hsm;

    // Transition to state
    hsm.initialTransitionTo(&hsm.state1);

    // Make sure we are in state1
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1);
    EXPECT_EQ(hsm.state1EntryCallCount, 1);

    // Send event to transition to state1A
    {
        TestEvent event(EventId::GO_TO_STATE_1A);
        hsm.handleEvent(&event);
    }

    // Make sure we are in state1A
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1A);
    EXPECT_EQ(hsm.state1ExitCallCount, 0); // Should not have exited state1
    EXPECT_EQ(hsm.state1aEntryCallCount, 1);

    // Send event to transition to state2
    {
        TestEvent event(EventId::GO_TO_STATE_2);
        hsm.handleEvent(&event);
    }

    // Make sure we are in state2
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state2);
    EXPECT_EQ(hsm.state1ExitCallCount, 1);
    EXPECT_EQ(hsm.state2EntryCallCount, 1);
}

TEST(HsmTests, TransitionToSameStateCallsExitEntryAgain) {
    // Create test HSM
    TestHsm hsm;

    // Transition to state
    hsm.initialTransitionTo(&hsm.state1);

    // Make sure we are in state1
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1);
    EXPECT_EQ(hsm.state1EntryCallCount, 1);

    // Send event to transition to same state
    {
        TestEvent event(EventId::GO_TO_STATE_1);
        hsm.handleEvent(&event);
    }

    // Make sure we are still in state1, but the exit and entry were called again
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1);
    EXPECT_EQ(hsm.state1ExitCallCount, 1); 
    EXPECT_EQ(hsm.state1EntryCallCount, 2);
}