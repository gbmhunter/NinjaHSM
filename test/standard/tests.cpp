#include <gtest/gtest.h>

#include <gmock/gmock.h>

#include "NinjaHSM/NinjaHSM.hpp"

using namespace NinjaHSM;

enum class EventId {
    GO_TO_STATE_1,
    GO_TO_STATE_1A,
    GO_TO_STATE_1B,
    GO_TO_STATE_2,
    GO_TO_STATE_3,
    GO_TO_STATE_4,
    GO_TO_STATE_5,
    GO_TO_STATE_6,
    GO_TO_STATE_7,
    GO_TO_STATE_8,
    NO_ONE_HANDLES_THIS,
    EVERYONE_HANDLES_THIS,
    EVENT_WITH_DATA_1,
    EVENT_WITH_DATA_2,
};

class EventWithData1 {
public:
    EventWithData1(uint32_t data) : data(data) {}
    uint32_t data;
};

class EventWithData2 {
public:
    EventWithData2(int64_t data) : data(data) {}
    int64_t data;
};

class EventWrapper {
public:
  EventWrapper(EventId id) : id(id) {}
  EventId id;
  union {
    EventWithData1 data1;
    EventWithData2 data2;
  };
};

class TestHsm : public StateMachine<EventWrapper> {
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
      state1B(
        "State1B",
        std::bind(&TestHsm::state1b_entry, this),
        std::bind(&TestHsm::state1b_event, this, std::placeholders::_1),
        std::bind(&TestHsm::state1b_exit, this),
        &state1
      ),
      state1C(
        "State1C",
        std::bind(&TestHsm::state1c_entry, this),
        std::bind(&TestHsm::state1c_event, this, std::placeholders::_1),
        std::bind(&TestHsm::state1c_exit, this),
        &state1
      ),
      state2(
        "State2",
        std::bind(&TestHsm::state2_entry, this),
        std::bind(&TestHsm::state2_event, this, std::placeholders::_1),
        std::bind(&TestHsm::state2_exit, this),
        nullptr
      ),
      state3(
        "State3",
        std::bind(&TestHsm::state3_entry, this),
        std::bind(&TestHsm::state3_event, this, std::placeholders::_1),
        std::bind(&TestHsm::state3_exit, this),
        nullptr
      ),
      state4(
        "State4",
        std::bind(&TestHsm::state4_entry, this),
        std::bind(&TestHsm::state4_event, this, std::placeholders::_1),
        std::bind(&TestHsm::state4_exit, this),
        nullptr
      ),
      state4A(
        "State4A",
        std::bind(&TestHsm::state4a_entry, this),
        std::bind(&TestHsm::state4a_event, this, std::placeholders::_1),
        std::bind(&TestHsm::state4a_exit, this),
        &state4
      ),
      state5(
        "State5",
        std::bind(&TestHsm::state5_entry, this),
        std::bind(&TestHsm::state5_event, this, std::placeholders::_1),
        std::bind(&TestHsm::state5_exit, this),
        nullptr
      ),
      state5A(
        "State5A",
        std::bind(&TestHsm::state5a_entry, this),
        std::bind(&TestHsm::state5a_event, this, std::placeholders::_1),
        std::bind(&TestHsm::state5a_exit, this),
        &state5
      ),
      state5A1(
        "State5A1",
        std::bind(&TestHsm::state5a1_entry, this),
        std::bind(&TestHsm::state5a1_event, this, std::placeholders::_1),
        std::bind(&TestHsm::state5a1_exit, this),
        &state5A
      ),
      state6(
        "State6",
        std::bind(&TestHsm::state6_entry, this),
        std::bind(&TestHsm::state6_event, this, std::placeholders::_1),
        std::bind(&TestHsm::state6_exit, this),
        nullptr
      ),
      state6A(
        "State6A",
        std::bind(&TestHsm::state6a_entry, this),
        std::bind(&TestHsm::state6a_event, this, std::placeholders::_1),
        std::bind(&TestHsm::state6a_exit, this),
        &state6
      ),
      state7(
        "State7",
        std::bind(&TestHsm::state7_entry, this),
        std::bind(&TestHsm::state7_event, this, std::placeholders::_1),
        std::bind(&TestHsm::state7_exit, this),
        nullptr
      ),
      state8(
        "State8",
        std::bind(&TestHsm::state8_entry, this),
        std::bind(&TestHsm::state8_event, this, std::placeholders::_1),
        std::bind(&TestHsm::state8_exit, this),
        nullptr
      ) {
        addState(&state1);
        addState(&state1A);
        addState(&state2);
        addState(&state3);
        addState(&state4);
        addState(&state4A);
        addState(&state5);
        addState(&state5A);
        addState(&state5A1);
        addState(&state6);
        addState(&state6A);
        addState(&state7);
        addState(&state8);
    }

    State<EventWrapper> state1;
    State<EventWrapper> state1A;
    State<EventWrapper> state1B;
    State<EventWrapper> state1C;
    State<EventWrapper> state2;
    State<EventWrapper> state3;
    State<EventWrapper> state4;
    State<EventWrapper> state4A;

    /**
     * State 5 is for testing multiple transitionTo() calls within entry functions.
     */
    State<EventWrapper> state5;
    State<EventWrapper> state5A;
    State<EventWrapper> state5A1;

    /**
     * State 6 is for testing transitionTo() calls to
     * child states within exit functions.
     */
    State<EventWrapper> state6;
    State<EventWrapper> state6A;

    /**
     * State 7 is for testing transitionTo() calls to
     * parent states within exit functions.
     */
    State<EventWrapper> state7;

    /**
     * State 8 is for testing transitionTo() calls to
     * the entry function to oneself. This should cause a re-entry.
     */
    State<EventWrapper> state8;

    uint32_t state1EntryCallCount = 0;
    uint32_t state1EventCallCount = 0;
    uint32_t state1ExitCallCount = 0;

    uint32_t state1aEntryCallCount = 0;
    uint32_t state1aEventCallCount = 0;
    uint32_t state1aExitCallCount = 0;

    uint32_t state1bEntryCallCount = 0;
    uint32_t state1bEventCallCount = 0;
    uint32_t state1bExitCallCount = 0;

    uint32_t state1cEntryCallCount = 0;
    uint32_t state1cEventCallCount = 0;
    uint32_t state1cExitCallCount = 0;

    uint32_t state2EntryCallCount = 0;
    uint32_t state2EventCallCount = 0;
    uint32_t state2ExitCallCount = 0;

    uint32_t state3EntryCallCount = 0;
    uint32_t state3EventCallCount = 0;
    uint32_t state3ExitCallCount = 0;

    uint32_t state4EntryCallCount = 0;
    uint32_t state4EventCallCount = 0;
    uint32_t state4ExitCallCount = 0;

    uint32_t state4aEntryCallCount = 0;
    uint32_t state4aEventCallCount = 0;
    uint32_t state4aExitCallCount = 0;

    uint32_t state5EntryCallCount = 0;
    uint32_t state5EventCallCount = 0;
    uint32_t state5ExitCallCount = 0;

    uint32_t state5aEntryCallCount = 0;
    uint32_t state5aEventCallCount = 0;
    uint32_t state5aExitCallCount = 0;

    uint32_t state5a1EntryCallCount = 0;
    uint32_t state5a1EventCallCount = 0;
    uint32_t state5a1ExitCallCount = 0;

    uint32_t state6EntryCallCount = 0;
    uint32_t state6EventCallCount = 0;
    uint32_t state6ExitCallCount = 0;

    uint32_t state6aEntryCallCount = 0;
    uint32_t state6aEventCallCount = 0;
    uint32_t state6aExitCallCount = 0;

    uint32_t state7EntryCallCount = 0;
    uint32_t state7EventCallCount = 0;
    uint32_t state7ExitCallCount = 0;

    uint32_t state8EntryCallCount = 0;
    uint32_t state8EventCallCount = 0;
    uint32_t state8ExitCallCount = 0;
    uint32_t state8ReEntryCallCount = 0;

private:

    //========================================================================//
    // state1
    //========================================================================//

    virtual void state1_entry() {
        std::cout << "state1_entry" << std::endl;
        state1EntryCallCount++;
    }

    virtual void state1_event(const EventWrapper * event) {
        std::cout << "state1_event" << std::endl;

        if (event->id == EventId::GO_TO_STATE_1) {
            transitionTo(&state1);
        }
        else if (event->id == EventId::GO_TO_STATE_1A) {
            transitionTo(&state1A);
        }
        else if (event->id == EventId::GO_TO_STATE_1B) {
            transitionTo(&state1B);
        }
        else if (event->id == EventId::GO_TO_STATE_3) {
            transitionTo(&state3);
        }
        else if (event->id == EventId::GO_TO_STATE_4) {
            transitionTo(&state4);
        }
        else if (event->id == EventId::GO_TO_STATE_5) {
            transitionTo(&state5);
        }
        else if (event->id == EventId::GO_TO_STATE_6) {
            transitionTo(&state6);
        }
        else if (event->id == EventId::GO_TO_STATE_7) {
            transitionTo(&state7);
        }
        else if (event->id == EventId::GO_TO_STATE_8) {
            transitionTo(&state8);
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

    virtual void state1a_event(const EventWrapper * event) {
        std::cout << "state1a_event" << std::endl;

        if (event->id == EventId::GO_TO_STATE_2) {
            transitionTo(&state2);
        }
        else if (event->id == EventId::EVERYONE_HANDLES_THIS) {
            eventHandled();
        }
        state1aEventCallCount++;
    }

    virtual void state1a_exit() {
        std::cout << "state1a_exit" << std::endl;
        state1aExitCallCount++;
    }

    //========================================================================//
    // state1/state1b
    //========================================================================//

    virtual void state1b_entry() {
        std::cout << __PRETTY_FUNCTION__ << " called." << std::endl;
        state1bEntryCallCount++;
        // Go directly to state1C
        transitionTo(&state1C);
    }

    virtual void state1b_event(const EventWrapper * event) {
        std::cout << __PRETTY_FUNCTION__ << " called." << std::endl;
        state1bEventCallCount++;
    }

    virtual void state1b_exit() {
        std::cout << __PRETTY_FUNCTION__ << " called." << std::endl;
        state1bExitCallCount++;
    }

    //========================================================================//
    // state1/state1c
    //========================================================================//

    virtual void state1c_entry() {
        std::cout << __PRETTY_FUNCTION__ << " called." << std::endl;
        state1cEntryCallCount++;
    }

    virtual void state1c_event(const EventWrapper * event) {
        std::cout << __PRETTY_FUNCTION__ << " called." << std::endl;
        state1cEventCallCount++;
    }

    virtual void state1c_exit() {
        std::cout << __PRETTY_FUNCTION__ << " called." << std::endl;
        state1cExitCallCount++;
    }

    //========================================================================//
    // state2
    //========================================================================//

    virtual void state2_entry() {
        std::cout << "state2_entry" << std::endl;
        state2EntryCallCount++;
    }

    virtual void state2_event(const EventWrapper * event) {
        std::cout << "state2_event" << std::endl;
        state2EventCallCount++;
    }

    virtual void state2_exit() {
        std::cout << "state2_exit" << std::endl;
        state2ExitCallCount++;
    }

    //========================================================================//
    // state3
    //========================================================================//

    virtual void state3_entry() {
        std::cout << "state3_entry" << std::endl;
        state3EntryCallCount++;
        // We have a state guard here that always transitions to state1
        transitionTo(&state1);
    }

    virtual void state3_event(const EventWrapper * event) {
        std::cout << "state3_event" << std::endl;
        state3EventCallCount++;
    }

    virtual void state3_exit() {
        std::cout << "state3_exit" << std::endl;
        state3ExitCallCount++;
    }

    //========================================================================//
    // state4
    //========================================================================//

    virtual void state4_entry() {
        std::cout << "state4_entry" << std::endl;
        state4EntryCallCount++;
        // Go directly to state4A, a child state
        transitionTo(&state4A);
    }

    virtual void state4_event(const EventWrapper * event) {
        std::cout << "state4_event" << std::endl;
        state4EventCallCount++;
    }

    virtual void state4_exit() {
        std::cout << "state4_exit" << std::endl;
        state4ExitCallCount++;
    }

    //========================================================================//
    // state4/state4a
    //========================================================================//

    virtual void state4a_entry() {
        std::cout << "state4a_entry" << std::endl;
        state4aEntryCallCount++;
    }

    virtual void state4a_event(const EventWrapper * event) {
        std::cout << "state4a_event" << std::endl;
        state4aEventCallCount++;
    }

    virtual void state4a_exit() {
        std::cout << "state4a_exit" << std::endl;
        state4aExitCallCount++;
    }

    //========================================================================//
    // state5
    //========================================================================//

    virtual void state5_entry() {
        std::cout << "state5_entry" << std::endl;
        state5EntryCallCount++;
        transitionTo(&state5A);
    }

    virtual void state5_event(const EventWrapper * event) {
        std::cout << "state5_event" << std::endl;
        state5EventCallCount++;
    }

    virtual void state5_exit() {
        std::cout << "state5_exit" << std::endl;
        state5ExitCallCount++;
    }

    //========================================================================//
    // state5/state5a
    //========================================================================//

    virtual void state5a_entry() {
        std::cout << "state5a_entry" << std::endl;
        state5aEntryCallCount++;
        transitionTo(&state5A1);
    }

    virtual void state5a_event(const EventWrapper * event) {
        std::cout << "state5a_event" << std::endl;
        state5aEventCallCount++;
    }

    virtual void state5a_exit() {
        std::cout << "state5a_exit" << std::endl;
        state5aExitCallCount++;
    }

    //========================================================================//
    // state5/state5a/state5a1
    //========================================================================//

    virtual void state5a1_entry() {
        std::cout << "state5a1_entry" << std::endl;
        state5a1EntryCallCount++;
        transitionTo(&state1);
    }

    virtual void state5a1_event(const EventWrapper * event) {
        std::cout << "state5a1_event" << std::endl;
        state5a1EventCallCount++;
    }

    virtual void state5a1_exit() {
        std::cout << "state5a1_exit" << std::endl;
        state5a1ExitCallCount++;
    }

    //========================================================================//
    // state6
    //========================================================================//

    virtual void state6_entry() {
        std::cout << "state6_entry" << std::endl;
        state6EntryCallCount++;
    }

    virtual void state6_event(const EventWrapper * event) {
        std::cout << "state6_event" << std::endl;
        state6EventCallCount++;

        if (event->id == EventId::GO_TO_STATE_1) {
            transitionTo(&state1);
        }
    }

    virtual void state6_exit() {
        std::cout << "state6_exit" << std::endl;
        state6ExitCallCount++;
        transitionTo(&state6A);
    }

    //========================================================================//
    // state6/state6a
    //========================================================================//

    virtual void state6a_entry() {
        std::cout << "state6a_entry" << std::endl;
        state6aEntryCallCount++;
    }

    virtual void state6a_event(const EventWrapper * event) {
        std::cout << "state6a_event" << std::endl;
        state6aEventCallCount++;
    }

    virtual void state6a_exit() {
        std::cout << "state6a_exit" << std::endl;
        state6aExitCallCount++;
    }

    //========================================================================//
    // state7
    //========================================================================//

    virtual void state7_entry() {
        std::cout << "state7_entry" << std::endl;
        state7EntryCallCount++;
    }

    virtual void state7_event(const EventWrapper * event) {
        std::cout << "state7_event" << std::endl;
        state7EventCallCount++;
        if (event->id == EventId::GO_TO_STATE_1) {
            transitionTo(&state1);
        }
    }

    virtual void state7_exit() {
        std::cout << "state7_exit" << std::endl;
        state7ExitCallCount++;
        // Override transition and go to state2 instead
        transitionTo(&state2);
    }

    //========================================================================//
    // state8
    //========================================================================//

    virtual void state8_entry() {
        std::cout << "state8_entry" << std::endl;
        state8EntryCallCount++;

        // This should cause a single re-entry of state8
        // upon entry (i.e. 2x entry and 1x exit calls).
        if (state8ReEntryCallCount == 0) {
            state8ReEntryCallCount++;
            transitionTo(&state8);
        }
    }

    virtual void state8_event(const EventWrapper * event) {
        std::cout << "state8_event" << std::endl;
        state8EventCallCount++;
    }

    virtual void state8_exit() {
        std::cout << "state8_exit" << std::endl;
        state8ExitCallCount++;
    }
};

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
        EventWrapper event(EventId::GO_TO_STATE_1A);
        hsm.handleEvent(&event);
    }

    // Make sure we are in state1A
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1A);
    EXPECT_EQ(hsm.state1ExitCallCount, 0); // Should not have exited state1
    EXPECT_EQ(hsm.state1aEntryCallCount, 1);

    // Send event to transition to state2
    {
        EventWrapper event(EventId::GO_TO_STATE_2);
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
        EventWrapper event(EventId::GO_TO_STATE_1);
        hsm.handleEvent(&event);
    }

    // Make sure we are still in state1, but the exit and entry were called again
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1);
    EXPECT_EQ(hsm.state1ExitCallCount, 1); 
    EXPECT_EQ(hsm.state1EntryCallCount, 2);
}

TEST(HsmTests, EventsBubbleUpToParentStates) {
    // Create test HSM
    TestHsm hsm;

    // Transition to state
    hsm.initialTransitionTo(&hsm.state1A);

    // Make sure we are in state1A
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1A);
    EXPECT_EQ(hsm.state1EntryCallCount, 1);
    EXPECT_EQ(hsm.state1aEntryCallCount, 1);

    // Send event that no state handles
    {
        EventWrapper event(EventId::NO_ONE_HANDLES_THIS);
        hsm.handleEvent(&event);
    }

    // Make sure that the event was bubbled up to the parent state
    EXPECT_EQ(hsm.state1aEventCallCount, 1); 
    EXPECT_EQ(hsm.state1EventCallCount, 1);
}

TEST(HsmTests, EventHandledStopsBubbleUp) {
    TestHsm hsm;

    hsm.initialTransitionTo(&hsm.state1A);

    // Make sure we are in state1A
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1A);
    EXPECT_EQ(hsm.state1EntryCallCount, 1);
    EXPECT_EQ(hsm.state1aEntryCallCount, 1);

    // Send event that we be handled
    {
        EventWrapper event(EventId::EVERYONE_HANDLES_THIS);
        hsm.handleEvent(&event);
    }

    // Make sure that the event was NOT bubbled up to the parent state
    EXPECT_EQ(hsm.state1aEventCallCount, 1); 
    EXPECT_EQ(hsm.state1EventCallCount, 0);
}

TEST(HsmTests, TransitionToStopsBubbleUp) {
    TestHsm hsm;

    hsm.initialTransitionTo(&hsm.state1A);

    // Make sure we are in state1A
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1A);
    EXPECT_EQ(hsm.state1EntryCallCount, 1);
    EXPECT_EQ(hsm.state1aEntryCallCount, 1);

    // Transition to state2
    {
        EventWrapper event(EventId::GO_TO_STATE_2);
        hsm.handleEvent(&event);
    }

    // Make sure that the event was NOT bubbled up to the parent state
    EXPECT_EQ(hsm.state1aEventCallCount, 1); 
    EXPECT_EQ(hsm.state1EventCallCount, 0);
}

TEST(HsmTests, EntryGuardsWorkWithTopLevelStates) {
    TestHsm hsm;

    hsm.initialTransitionTo(&hsm.state1);

    // Make sure we are in state1
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1);
    EXPECT_EQ(hsm.state1EntryCallCount, 1);

    // Send event to transition to state3, which has am entry guard
    // that always transitions to state1
    {
        EventWrapper event(EventId::GO_TO_STATE_3);
        hsm.handleEvent(&event);
    }

    // Make sure we are back in state1, not state3
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1);
    EXPECT_EQ(hsm.state1EntryCallCount, 2);
    EXPECT_EQ(hsm.state3EntryCallCount, 1);
}

TEST(HsmTests, EntryGuardsWorkWithChildStates) {
    TestHsm hsm;

    hsm.initialTransitionTo(&hsm.state1);

    // Make sure we are in state1
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1);
    EXPECT_EQ(hsm.state1EntryCallCount, 1);

    // Send event to transition to state1B, which has an entry guard
    // that always transitions to state1C
    {
        EventWrapper event(EventId::GO_TO_STATE_1B);
        hsm.handleEvent(&event);
    }

    // Make sure we are in state1C
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1C);
    EXPECT_EQ(hsm.state1EntryCallCount, 1);
    EXPECT_EQ(hsm.state1bEntryCallCount, 1);
    EXPECT_EQ(hsm.state1cEntryCallCount, 1);
}

TEST(HsmTests, CanTransitionToChildStateFromParentEntry) {
    TestHsm hsm;

    hsm.initialTransitionTo(&hsm.state1);

    // Make sure we are in state1
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1);
    EXPECT_EQ(hsm.state1EntryCallCount, 1);

    // Send event to transition to state4, which has an entry guard
    // that always transitions to state4A
    {
        EventWrapper event(EventId::GO_TO_STATE_4);
        hsm.handleEvent(&event);
    }

    // Make sure we are in state4A
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state4A);
    EXPECT_EQ(hsm.state4EntryCallCount, 1);
    EXPECT_EQ(hsm.state4ExitCallCount, 0);
    EXPECT_EQ(hsm.state4aEntryCallCount, 1);
}

TEST(HsmTests, ChainedTransitionToInEntryFunctions) {
    TestHsm hsm;

    hsm.initialTransitionTo(&hsm.state1);

    // Make sure we are in state1
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1);
    EXPECT_EQ(hsm.state1EntryCallCount, 1);

    // Send event to transition to state5, which will:
    // 1. Transition to state5A from 5's entry function
    // 2. Transition to state5A1 from 5A's entry function
    // 3. Transition to state1 from 5A1's entry function
    {
        EventWrapper event(EventId::GO_TO_STATE_5);
        hsm.handleEvent(&event);
    }

    // Make sure we are in state1
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1);
    EXPECT_EQ(hsm.state1EntryCallCount, 2);
    EXPECT_EQ(hsm.state5EntryCallCount, 1);
    EXPECT_EQ(hsm.state5ExitCallCount, 1);
    
    EXPECT_EQ(hsm.state5aEntryCallCount, 1);
    EXPECT_EQ(hsm.state5aExitCallCount, 1);
    
    // We never entered state5A since we transitioned to state1
    // in it's entry function. The entry function would have been called,
    // but not the exit
    EXPECT_EQ(hsm.state5a1EntryCallCount, 1);
    EXPECT_EQ(hsm.state5a1ExitCallCount, 0);
}

TEST(HsmTests, CanTransitionToChildStateFromParentExit) {
    TestHsm hsm;

    hsm.initialTransitionTo(&hsm.state1);

    // Make sure we are in state1
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1);
    EXPECT_EQ(hsm.state1EntryCallCount, 1);

    // Send event to transition to state6
    {
        EventWrapper event(EventId::GO_TO_STATE_6);
        hsm.handleEvent(&event);
    }

    // Make sure we are in state6
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state6);
    EXPECT_EQ(hsm.state1ExitCallCount, 1);
    EXPECT_EQ(hsm.state6EntryCallCount, 1);

    // Now try and exit from state6, which will call transitionTo() to state 6a
    // in it's exit function
    {
        EventWrapper event(EventId::GO_TO_STATE_1);
        hsm.handleEvent(&event);
    }

    // Make sure we are in state6A
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state6A);
    // 6 exit function should have been called, even though
    // we didn't end up exiting from this state
    EXPECT_EQ(hsm.state6ExitCallCount, 1);
    EXPECT_EQ(hsm.state6aEntryCallCount, 1);
}

TEST(HsmTests, CanTransitionToParentStateFromChildExit) {
    TestHsm hsm;

    hsm.initialTransitionTo(&hsm.state1);

    // Make sure we are in state1
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1);
    EXPECT_EQ(hsm.state1EntryCallCount, 1);

    // Send event to transition to state7
    {
        EventWrapper event(EventId::GO_TO_STATE_7);
        hsm.handleEvent(&event);
    }

    // Make sure we are in state7
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state7);
    EXPECT_EQ(hsm.state1ExitCallCount, 1);
    EXPECT_EQ(hsm.state7EntryCallCount, 1);

    // Send event to transition to state1, but state7's exit function
    // overrides this and transitions to state2 instead
    {
        EventWrapper event(EventId::GO_TO_STATE_1);
        hsm.handleEvent(&event);
    }

    // Make sure we are in state2
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state2);
    EXPECT_EQ(hsm.state7EventCallCount, 1);
    EXPECT_EQ(hsm.state7ExitCallCount, 1);
    EXPECT_EQ(hsm.state2EntryCallCount, 1);
}

TEST(HsmTests, CanTransitionToSelfFromEntry) {
    TestHsm hsm;

    hsm.initialTransitionTo(&hsm.state1);

    // Send event to transition to state8. This should cause a single
    // re-entry of state8 upon entry (i.e. 2x entry and 1x exit calls).
    {
        EventWrapper event(EventId::GO_TO_STATE_8);
        hsm.handleEvent(&event);
    }

    // Make sure we are in state8
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state8);
    EXPECT_EQ(hsm.state8EntryCallCount, 2);
    EXPECT_EQ(hsm.state8ExitCallCount, 1);
}
