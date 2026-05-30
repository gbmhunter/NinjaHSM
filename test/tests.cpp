#include <string>
#include <vector>

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

class Event {
public:
  Event(EventId id) : id(id) {}
  EventId id;
  union {
    EventWithData1 data1;
    EventWithData2 data2;
  };
};

class TestHsm {
public:
    TestHsm() :
      state1(
        "State1",
        State<Event>::EntryDelegate::create<TestHsm, &TestHsm::state1_entry>(*this),
        State<Event>::EventDelegate::create<TestHsm, &TestHsm::state1_event>(*this),
        State<Event>::ExitDelegate::create<TestHsm, &TestHsm::state1_exit>(*this),
        nullptr
      ),
      state1A(
        "State1A",
        State<Event>::EntryDelegate::create<TestHsm, &TestHsm::state1a_entry>(*this),
        State<Event>::EventDelegate::create<TestHsm, &TestHsm::state1a_event>(*this),
        State<Event>::ExitDelegate::create<TestHsm, &TestHsm::state1a_exit>(*this),
        &state1
      ),
      state1B(
        "State1B",
        State<Event>::EntryDelegate::create<TestHsm, &TestHsm::state1b_entry>(*this),
        State<Event>::EventDelegate::create<TestHsm, &TestHsm::state1b_event>(*this),
        State<Event>::ExitDelegate::create<TestHsm, &TestHsm::state1b_exit>(*this),
        &state1
      ),
      state1C(
        "State1C",
        State<Event>::EntryDelegate::create<TestHsm, &TestHsm::state1c_entry>(*this),
        State<Event>::EventDelegate::create<TestHsm, &TestHsm::state1c_event>(*this),
        State<Event>::ExitDelegate::create<TestHsm, &TestHsm::state1c_exit>(*this),
        &state1
      ),
      state2(
        "State2",
        State<Event>::EntryDelegate::create<TestHsm, &TestHsm::state2_entry>(*this),
        State<Event>::EventDelegate::create<TestHsm, &TestHsm::state2_event>(*this),
        State<Event>::ExitDelegate::create<TestHsm, &TestHsm::state2_exit>(*this),
        nullptr
      ),
      state3(
        "State3",
        State<Event>::EntryDelegate::create<TestHsm, &TestHsm::state3_entry>(*this),
        State<Event>::EventDelegate::create<TestHsm, &TestHsm::state3_event>(*this),
        State<Event>::ExitDelegate::create<TestHsm, &TestHsm::state3_exit>(*this),
        nullptr
      ),
      state4(
        "State4",
        State<Event>::EntryDelegate::create<TestHsm, &TestHsm::state4_entry>(*this),
        State<Event>::EventDelegate::create<TestHsm, &TestHsm::state4_event>(*this),
        State<Event>::ExitDelegate::create<TestHsm, &TestHsm::state4_exit>(*this),
        nullptr
      ),
      state4A(
        "State4A",
        State<Event>::EntryDelegate::create<TestHsm, &TestHsm::state4a_entry>(*this),
        State<Event>::EventDelegate::create<TestHsm, &TestHsm::state4a_event>(*this),
        State<Event>::ExitDelegate::create<TestHsm, &TestHsm::state4a_exit>(*this),
        &state4
      ),
      state5(
        "State5",
        State<Event>::EntryDelegate::create<TestHsm, &TestHsm::state5_entry>(*this),
        State<Event>::EventDelegate::create<TestHsm, &TestHsm::state5_event>(*this),
        State<Event>::ExitDelegate::create<TestHsm, &TestHsm::state5_exit>(*this),
        nullptr
      ),
      state5A(
        "State5A",
        State<Event>::EntryDelegate::create<TestHsm, &TestHsm::state5a_entry>(*this),
        State<Event>::EventDelegate::create<TestHsm, &TestHsm::state5a_event>(*this),
        State<Event>::ExitDelegate::create<TestHsm, &TestHsm::state5a_exit>(*this),
        &state5
      ),
      state5A1(
        "State5A1",
        State<Event>::EntryDelegate::create<TestHsm, &TestHsm::state5a1_entry>(*this),
        State<Event>::EventDelegate::create<TestHsm, &TestHsm::state5a1_event>(*this),
        State<Event>::ExitDelegate::create<TestHsm, &TestHsm::state5a1_exit>(*this),
        &state5A
      ),
      state6(
        "State6",
        State<Event>::EntryDelegate::create<TestHsm, &TestHsm::state6_entry>(*this),
        State<Event>::EventDelegate::create<TestHsm, &TestHsm::state6_event>(*this),
        State<Event>::ExitDelegate::create<TestHsm, &TestHsm::state6_exit>(*this),
        nullptr
      ),
      state6A(
        "State6A",
        State<Event>::EntryDelegate::create<TestHsm, &TestHsm::state6a_entry>(*this),
        State<Event>::EventDelegate::create<TestHsm, &TestHsm::state6a_event>(*this),
        State<Event>::ExitDelegate::create<TestHsm, &TestHsm::state6a_exit>(*this),
        &state6
      ),
      state7(
        "State7",
        State<Event>::EntryDelegate::create<TestHsm, &TestHsm::state7_entry>(*this),
        State<Event>::EventDelegate::create<TestHsm, &TestHsm::state7_event>(*this),
        State<Event>::ExitDelegate::create<TestHsm, &TestHsm::state7_exit>(*this),
        nullptr
      ),
      state8(
        "State8",
        State<Event>::EntryDelegate::create<TestHsm, &TestHsm::state8_entry>(*this),
        State<Event>::EventDelegate::create<TestHsm, &TestHsm::state8_event>(*this),
        State<Event>::ExitDelegate::create<TestHsm, &TestHsm::state8_exit>(*this),
        nullptr
      ), m_stateMachine() {}

    State<Event> state1;
    State<Event> state1A;
    State<Event> state1B;
    State<Event> state1C;
    State<Event> state2;
    State<Event> state3;
    State<Event> state4;
    State<Event> state4A;

    /**
     * State 5 is for testing multiple transitionTo() calls within entry functions.
     */
    State<Event> state5;
    State<Event> state5A;
    State<Event> state5A1;

    /**
     * State 6 is for testing transitionTo() calls to
     * child states within exit functions.
     */
    State<Event> state6;
    State<Event> state6A;

    /**
     * State 7 is for testing transitionTo() calls to
     * parent states within exit functions.
     */
    State<Event> state7;

    /**
     * State 8 is for testing transitionTo() calls to
     * the entry function to oneself. This should cause a re-entry.
     */
    State<Event> state8;

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

    // Public interface methods
    void initialTransitionTo(State<Event>& state) {
        m_stateMachine.initialTransitionTo(state);
    }

    void handleEvent(const Event& event) {
        m_stateMachine.handleEvent(event);
    }

    const State<Event>* getCurrentState() {
        return m_stateMachine.getCurrentState();
    }

    void eventHandled() {
        m_stateMachine.eventHandled();
    }

    void transitionTo(State<Event>& state) {
        m_stateMachine.transitionTo(state);
    }

private:
    // State machine instance
    StateMachine<Event> m_stateMachine;

    //========================================================================//
    // state1
    //========================================================================//

    virtual void state1_entry() {
        std::cout << "state1_entry" << std::endl;
        state1EntryCallCount++;
    }

    virtual void state1_event(const Event& event) {
        std::cout << "state1_event" << std::endl;

        if (event.id == EventId::GO_TO_STATE_1) {
            m_stateMachine.transitionTo(state1);
        }
        else if (event.id == EventId::GO_TO_STATE_1A) {
            m_stateMachine.transitionTo(state1A);
        }
        else if (event.id == EventId::GO_TO_STATE_1B) {
            m_stateMachine.transitionTo(state1B);
        }
        else if (event.id == EventId::GO_TO_STATE_3) {
            m_stateMachine.transitionTo(state3);
        }
        else if (event.id == EventId::GO_TO_STATE_4) {
            m_stateMachine.transitionTo(state4);
        }
        else if (event.id == EventId::GO_TO_STATE_5) {
            m_stateMachine.transitionTo(state5);
        }
        else if (event.id == EventId::GO_TO_STATE_6) {
            m_stateMachine.transitionTo(state6);
        }
        else if (event.id == EventId::GO_TO_STATE_7) {
            m_stateMachine.transitionTo(state7);
        }
        else if (event.id == EventId::GO_TO_STATE_8) {
            m_stateMachine.transitionTo(state8);
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

    virtual void state1a_event(const Event& event) {
        std::cout << "state1a_event" << std::endl;

        if (event.id == EventId::GO_TO_STATE_2) {
            m_stateMachine.transitionTo(state2);
        }
        else if (event.id == EventId::EVERYONE_HANDLES_THIS) {
            m_stateMachine.eventHandled();
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
        m_stateMachine.transitionTo(state1C);
    }

    virtual void state1b_event(const Event& event) {
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

    virtual void state1c_event(const Event& event) {
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

    virtual void state2_event(const Event& event) {
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
        m_stateMachine.transitionTo(state1);
    }

    virtual void state3_event(const Event& event) {
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
        m_stateMachine.transitionTo(state4A);
    }

    virtual void state4_event(const Event& event) {
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

    virtual void state4a_event(const Event& event) {
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
        m_stateMachine.transitionTo(state5A);
    }

    virtual void state5_event(const Event& event) {
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
        m_stateMachine.transitionTo(state5A1);
    }

    virtual void state5a_event(const Event& event) {
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
        m_stateMachine.transitionTo(state1);
    }

    virtual void state5a1_event(const Event& event) {
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

    virtual void state6_event(const Event& event) {
        std::cout << "state6_event" << std::endl;
        state6EventCallCount++;

        if (event.id == EventId::GO_TO_STATE_1) {
            m_stateMachine.transitionTo(state1);
        }
    }

    virtual void state6_exit() {
        std::cout << "state6_exit" << std::endl;
        state6ExitCallCount++;
        m_stateMachine.transitionTo(state6A);
    }

    //========================================================================//
    // state6/state6a
    //========================================================================//

    virtual void state6a_entry() {
        std::cout << "state6a_entry" << std::endl;
        state6aEntryCallCount++;
    }

    virtual void state6a_event(const Event& event) {
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

    virtual void state7_event(const Event& event) {
        std::cout << "state7_event" << std::endl;
        state7EventCallCount++;
        if (event.id == EventId::GO_TO_STATE_1) {
            m_stateMachine.transitionTo(state1);
        }
    }

    virtual void state7_exit() {
        std::cout << "state7_exit" << std::endl;
        state7ExitCallCount++;
        // Override transition and go to state2 instead
        m_stateMachine.transitionTo(state2);
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
            m_stateMachine.transitionTo(state8);
        }
    }

    virtual void state8_event(const Event& event) {
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
    hsm.initialTransitionTo(hsm.state1);

    // Make sure we are in state1
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1);
    EXPECT_EQ(hsm.state1EntryCallCount, 1);

    // Send event to transition to state1A
    {
        Event event(EventId::GO_TO_STATE_1A);
        hsm.handleEvent(event);
    }

    // Make sure we are in state1A
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1A);
    EXPECT_EQ(hsm.state1ExitCallCount, 0); // Should not have exited state1
    EXPECT_EQ(hsm.state1aEntryCallCount, 1);

    // Send event to transition to state2
    {
        Event event(EventId::GO_TO_STATE_2);
        hsm.handleEvent(event);
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
    hsm.initialTransitionTo(hsm.state1);

    // Make sure we are in state1
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1);
    EXPECT_EQ(hsm.state1EntryCallCount, 1);

    // Send event to transition to same state
    {
        Event event(EventId::GO_TO_STATE_1);
        hsm.handleEvent(event);
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
    hsm.initialTransitionTo(hsm.state1A);

    // Make sure we are in state1A
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1A);
    EXPECT_EQ(hsm.state1EntryCallCount, 1);
    EXPECT_EQ(hsm.state1aEntryCallCount, 1);

    // Send event that no state handles
    {
        Event event(EventId::NO_ONE_HANDLES_THIS);
        hsm.handleEvent(event);
    }

    // Make sure that the event was bubbled up to the parent state
    EXPECT_EQ(hsm.state1aEventCallCount, 1); 
    EXPECT_EQ(hsm.state1EventCallCount, 1);
}

TEST(HsmTests, EventHandledStopsBubbleUp) {
    TestHsm hsm;

    hsm.initialTransitionTo(hsm.state1A);

    // Make sure we are in state1A
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1A);
    EXPECT_EQ(hsm.state1EntryCallCount, 1);
    EXPECT_EQ(hsm.state1aEntryCallCount, 1);

    // Send event that we be handled
    {
        Event event(EventId::EVERYONE_HANDLES_THIS);
        hsm.handleEvent(event);
    }

    // Make sure that the event was NOT bubbled up to the parent state
    EXPECT_EQ(hsm.state1aEventCallCount, 1); 
    EXPECT_EQ(hsm.state1EventCallCount, 0);
}

TEST(HsmTests, TransitionToStopsBubbleUp) {
    TestHsm hsm;

    hsm.initialTransitionTo(hsm.state1A);

    // Make sure we are in state1A
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1A);
    EXPECT_EQ(hsm.state1EntryCallCount, 1);
    EXPECT_EQ(hsm.state1aEntryCallCount, 1);

    // Transition to state2
    {
        Event event(EventId::GO_TO_STATE_2);
        hsm.handleEvent(event);
    }

    // Make sure that the event was NOT bubbled up to the parent state
    EXPECT_EQ(hsm.state1aEventCallCount, 1); 
    EXPECT_EQ(hsm.state1EventCallCount, 0);
}

TEST(HsmTests, EntryGuardsWorkWithTopLevelStates) {
    TestHsm hsm;

    hsm.initialTransitionTo(hsm.state1);

    // Make sure we are in state1
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1);
    EXPECT_EQ(hsm.state1EntryCallCount, 1);

    // Send event to transition to state3, which has am entry guard
    // that always transitions to state1
    {
        Event event(EventId::GO_TO_STATE_3);
        hsm.handleEvent(event);
    }

    // Make sure we are back in state1, not state3
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1);
    EXPECT_EQ(hsm.state1EntryCallCount, 2);
    EXPECT_EQ(hsm.state3EntryCallCount, 1);
}

TEST(HsmTests, EntryGuardsWorkWithChildStates) {
    TestHsm hsm;

    hsm.initialTransitionTo(hsm.state1);

    // Make sure we are in state1
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1);
    EXPECT_EQ(hsm.state1EntryCallCount, 1);

    // Send event to transition to state1B, which has an entry guard
    // that always transitions to state1C
    {
        Event event(EventId::GO_TO_STATE_1B);
        hsm.handleEvent(event);
    }

    // Make sure we are in state1C
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1C);
    EXPECT_EQ(hsm.state1EntryCallCount, 1);
    EXPECT_EQ(hsm.state1bEntryCallCount, 1);
    EXPECT_EQ(hsm.state1cEntryCallCount, 1);
}

TEST(HsmTests, CanTransitionToChildStateFromParentEntry) {
    TestHsm hsm;

    hsm.initialTransitionTo(hsm.state1);

    // Make sure we are in state1
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1);
    EXPECT_EQ(hsm.state1EntryCallCount, 1);

    // Send event to transition to state4, which has an entry guard
    // that always transitions to state4A
    {
        Event event(EventId::GO_TO_STATE_4);
        hsm.handleEvent(event);
    }

    // Make sure we are in state4A
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state4A);
    EXPECT_EQ(hsm.state4EntryCallCount, 1);
    EXPECT_EQ(hsm.state4ExitCallCount, 0);
    EXPECT_EQ(hsm.state4aEntryCallCount, 1);
}

TEST(HsmTests, ChainedTransitionToInEntryFunctions) {
    TestHsm hsm;

    hsm.initialTransitionTo(hsm.state1);

    // Make sure we are in state1
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1);
    EXPECT_EQ(hsm.state1EntryCallCount, 1);

    // Send event to transition to state5, which will:
    // 1. Transition to state5A from 5's entry function
    // 2. Transition to state5A1 from 5A's entry function
    // 3. Transition to state1 from 5A1's entry function
    {
        Event event(EventId::GO_TO_STATE_5);
        hsm.handleEvent(event);
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

    hsm.initialTransitionTo(hsm.state1);

    // Make sure we are in state1
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1);
    EXPECT_EQ(hsm.state1EntryCallCount, 1);

    // Send event to transition to state6
    {
        Event event(EventId::GO_TO_STATE_6);
        hsm.handleEvent(event);
    }

    // Make sure we are in state6
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state6);
    EXPECT_EQ(hsm.state1ExitCallCount, 1);
    EXPECT_EQ(hsm.state6EntryCallCount, 1);

    // Now try and exit from state6, which will call transitionTo() to state 6a
    // in it's exit function
    {
        Event event(EventId::GO_TO_STATE_1);
        hsm.handleEvent(event);
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

    hsm.initialTransitionTo(hsm.state1);

    // Make sure we are in state1
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state1);
    EXPECT_EQ(hsm.state1EntryCallCount, 1);

    // Send event to transition to state7
    {
        Event event(EventId::GO_TO_STATE_7);
        hsm.handleEvent(event);
    }

    // Make sure we are in state7
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state7);
    EXPECT_EQ(hsm.state1ExitCallCount, 1);
    EXPECT_EQ(hsm.state7EntryCallCount, 1);

    // Send event to transition to state1, but state7's exit function
    // overrides this and transitions to state2 instead
    {
        Event event(EventId::GO_TO_STATE_1);
        hsm.handleEvent(event);
    }

    // Make sure we are in state2
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state2);
    EXPECT_EQ(hsm.state7EventCallCount, 1);
    EXPECT_EQ(hsm.state7ExitCallCount, 1);
    EXPECT_EQ(hsm.state2EntryCallCount, 1);
}

TEST(HsmTests, CanTransitionToSelfFromEntry) {
    TestHsm hsm;

    hsm.initialTransitionTo(hsm.state1);

    // Send event to transition to state8. This should cause a single
    // re-entry of state8 upon entry (i.e. 2x entry and 1x exit calls).
    {
        Event event(EventId::GO_TO_STATE_8);
        hsm.handleEvent(event);
    }

    // Make sure we are in state8
    EXPECT_EQ(hsm.getCurrentState(), &hsm.state8);
    EXPECT_EQ(hsm.state8EntryCallCount, 2);
    EXPECT_EQ(hsm.state8ExitCallCount, 1);
}

//============================================================================================//
// makeState() helper
//============================================================================================//

/**
 * A minimal HSM whose states are constructed with the makeState() helper instead of spelling
 * out the ETL delegate ::create<> expressions by hand. Used to verify the helper produces a
 * State that wires up entry/event/exit/parent identically to the verbose form.
 */
class MakeStateHsm {
public:
    MakeStateHsm() :
      parent(makeState<Event,
        &MakeStateHsm::parent_entry,
        &MakeStateHsm::parent_event,
        &MakeStateHsm::parent_exit>("Parent", *this)),
      child(makeState<Event,
        &MakeStateHsm::child_entry,
        &MakeStateHsm::child_event,
        &MakeStateHsm::child_exit>("Child", *this, &parent)),
      m_stateMachine() {}

    void initialTransitionTo(State<Event>& state) { m_stateMachine.initialTransitionTo(state); }
    void handleEvent(const Event& event) { m_stateMachine.handleEvent(event); }
    const State<Event>* getCurrentState() { return m_stateMachine.getCurrentState(); }

    void parent_entry() { parentEntryCallCount++; }
    void parent_event(const Event& event) {
        if (event.id == EventId::GO_TO_STATE_1A) {
            m_stateMachine.transitionTo(child);
        }
        parentEventCallCount++;
    }
    void parent_exit() { parentExitCallCount++; }

    void child_entry() { childEntryCallCount++; }
    void child_event(const Event& event) { childEventCallCount++; }
    void child_exit() { childExitCallCount++; }

    State<Event> parent;
    State<Event> child;
    StateMachine<Event> m_stateMachine;

    uint32_t parentEntryCallCount = 0;
    uint32_t parentEventCallCount = 0;
    uint32_t parentExitCallCount = 0;
    uint32_t childEntryCallCount = 0;
    uint32_t childEventCallCount = 0;
    uint32_t childExitCallCount = 0;
};

TEST(MakeStateTests, BuildsUsableStates) {
    MakeStateHsm hsm;

    // Names and parent pointer should be wired up by makeState().
    EXPECT_STREQ(hsm.parent.name, "Parent");
    EXPECT_STREQ(hsm.child.name, "Child");
    EXPECT_EQ(hsm.parent.parent, nullptr);
    EXPECT_EQ(hsm.child.parent, &hsm.parent);

    hsm.initialTransitionTo(hsm.parent);
    EXPECT_EQ(hsm.getCurrentState(), &hsm.parent);
    EXPECT_EQ(hsm.parentEntryCallCount, 1);

    // The event handler delegate should fire and trigger the transition to the child state.
    {
        Event event(EventId::GO_TO_STATE_1A);
        hsm.handleEvent(event);
    }
    EXPECT_EQ(hsm.getCurrentState(), &hsm.child);
    EXPECT_EQ(hsm.parentEventCallCount, 1);
    EXPECT_EQ(hsm.childEntryCallCount, 1);

    // Unhandled-by-child events should bubble to the parent's event handler.
    {
        Event event(EventId::NO_ONE_HANDLES_THIS);
        hsm.handleEvent(event);
    }
    EXPECT_EQ(hsm.childEventCallCount, 1);
    EXPECT_EQ(hsm.parentEventCallCount, 2);
}

//============================================================================================//
// makeState() with omitted (nullptr) handlers
//============================================================================================//

/**
 * An HSM whose states omit some handlers by passing nullptr to makeState(), instead of writing
 * empty stub methods. Layout:
 *
 *   parent      (entry + event + exit)
 *     |-- child (no event(): events should bubble up to parent)
 *   onlyEvent   (no entry()/exit(): a leaf that just reacts to events)
 *   bare        (no handlers at all)
 */
class PartialHandlerHsm {
public:
    PartialHandlerHsm() :
      parent(makeState<Event,
        &PartialHandlerHsm::parent_entry,
        &PartialHandlerHsm::parent_event,
        &PartialHandlerHsm::parent_exit>("Parent", *this)),
      child(makeState<Event,
        &PartialHandlerHsm::child_entry,
        nullptr,
        &PartialHandlerHsm::child_exit>("Child", *this, &parent)),
      onlyEvent(makeState<Event,
        nullptr,
        &PartialHandlerHsm::onlyEvent_event,
        nullptr>("OnlyEvent", *this)),
      bare(makeState<Event,
        nullptr,
        nullptr,
        nullptr>("Bare", *this)),
      m_stateMachine() {}

    void initialTransitionTo(State<Event>& state) { m_stateMachine.initialTransitionTo(state); }
    void handleEvent(const Event& event) { m_stateMachine.handleEvent(event); }
    const State<Event>* getCurrentState() { return m_stateMachine.getCurrentState(); }

    void parent_entry() { parentEntryCallCount++; }
    void parent_event(const Event& event) {
        if (event.id == EventId::GO_TO_STATE_1A) {
            m_stateMachine.transitionTo(child);
        }
        parentEventCallCount++;
    }
    void parent_exit() { parentExitCallCount++; }

    void child_entry() { childEntryCallCount++; }
    void child_exit() { childExitCallCount++; }

    void onlyEvent_event(const Event& event) { onlyEventEventCallCount++; }

    State<Event> parent;
    State<Event> child;
    State<Event> onlyEvent;
    State<Event> bare;
    StateMachine<Event> m_stateMachine;

    uint32_t parentEntryCallCount = 0;
    uint32_t parentEventCallCount = 0;
    uint32_t parentExitCallCount = 0;
    uint32_t childEntryCallCount = 0;
    uint32_t childExitCallCount = 0;
    uint32_t onlyEventEventCallCount = 0;
};

TEST(MakeStateTests, NullptrHandlerSlotsLeaveDelegatesUnbound) {
    PartialHandlerHsm hsm;

    // Slots passed a member function pointer are bound; nullptr slots are left unbound.
    EXPECT_TRUE(hsm.parent.entry.is_valid());
    EXPECT_TRUE(hsm.parent.event.is_valid());
    EXPECT_TRUE(hsm.parent.exit.is_valid());

    EXPECT_TRUE(hsm.child.entry.is_valid());
    EXPECT_FALSE(hsm.child.event.is_valid());
    EXPECT_TRUE(hsm.child.exit.is_valid());

    EXPECT_FALSE(hsm.onlyEvent.entry.is_valid());
    EXPECT_TRUE(hsm.onlyEvent.event.is_valid());
    EXPECT_FALSE(hsm.onlyEvent.exit.is_valid());

    EXPECT_FALSE(hsm.bare.entry.is_valid());
    EXPECT_FALSE(hsm.bare.event.is_valid());
    EXPECT_FALSE(hsm.bare.exit.is_valid());
}

TEST(MakeStateTests, StateWithoutEntryOrExitCanBeEnteredAndExited) {
    PartialHandlerHsm hsm;

    // Entering a state with no entry() handler must not crash, and the state must become current.
    hsm.initialTransitionTo(hsm.onlyEvent);
    EXPECT_EQ(hsm.getCurrentState(), &hsm.onlyEvent);

    // Its event() handler still fires.
    {
        Event event(EventId::NO_ONE_HANDLES_THIS);
        hsm.handleEvent(event);
    }
    EXPECT_EQ(hsm.onlyEventEventCallCount, 1);

    // Exiting a state with no exit() handler must not crash either.
    hsm.initialTransitionTo(hsm.parent);
    EXPECT_EQ(hsm.getCurrentState(), &hsm.parent);
    EXPECT_EQ(hsm.parentEntryCallCount, 1);
}

TEST(MakeStateTests, EventBubblesPastStateWithNoEventHandler) {
    PartialHandlerHsm hsm;

    hsm.initialTransitionTo(hsm.parent);
    {
        Event event(EventId::GO_TO_STATE_1A);
        hsm.handleEvent(event);
    }
    EXPECT_EQ(hsm.getCurrentState(), &hsm.child);
    EXPECT_EQ(hsm.childEntryCallCount, 1);

    // The child has no event() handler, so the next event should bubble straight up to the parent.
    const uint32_t parentEventsBefore = hsm.parentEventCallCount;
    {
        Event event(EventId::NO_ONE_HANDLES_THIS);
        hsm.handleEvent(event);
    }
    EXPECT_EQ(hsm.parentEventCallCount, parentEventsBefore + 1);
}

TEST(MakeStateTests, StateWithNoHandlersAtAllIsHarmless) {
    PartialHandlerHsm hsm;

    // Entering, handling an event in, and leaving a state with no handlers must all be no-ops
    // rather than crashes.
    hsm.initialTransitionTo(hsm.bare);
    EXPECT_EQ(hsm.getCurrentState(), &hsm.bare);
    {
        Event event(EventId::NO_ONE_HANDLES_THIS);
        hsm.handleEvent(event);
    }
    EXPECT_EQ(hsm.getCurrentState(), &hsm.bare);

    hsm.initialTransitionTo(hsm.onlyEvent);
    EXPECT_EQ(hsm.getCurrentState(), &hsm.onlyEvent);
}

//============================================================================================//
// Observers (transition / unhandled event / error)
//============================================================================================//

/**
 * A small HSM used to exercise the observer hooks. Records every entry/exit, every unhandled
 * event, and every error so the tests can assert on them.
 *
 *   Parent
 *     |-- Child
 *   LoopA   (entry unconditionally transitions to LoopB)
 *   LoopB   (entry unconditionally transitions to LoopA)
 */
class ObserverHsm {
public:
    ObserverHsm() :
      parent(makeState<Event,
        &ObserverHsm::parent_entry,
        &ObserverHsm::parent_event,
        &ObserverHsm::parent_exit>("Parent", *this)),
      child(makeState<Event,
        &ObserverHsm::child_entry,
        &ObserverHsm::child_event,
        &ObserverHsm::child_exit>("Child", *this, &parent)),
      loopA(makeState<Event,
        &ObserverHsm::loopA_entry,
        &ObserverHsm::loopA_event,
        &ObserverHsm::loopA_exit>("LoopA", *this)),
      loopB(makeState<Event,
        &ObserverHsm::loopB_entry,
        &ObserverHsm::loopB_event,
        &ObserverHsm::loopB_exit>("LoopB", *this)),
      m_stateMachine() {
        m_stateMachine.setTransitionObserver(
            StateMachine<Event>::TransitionObserver::create<ObserverHsm, &ObserverHsm::onTransition>(*this));
        m_stateMachine.setUnhandledEventObserver(
            StateMachine<Event>::UnhandledEventObserver::create<ObserverHsm, &ObserverHsm::onUnhandledEvent>(*this));
        m_stateMachine.setErrorObserver(
            StateMachine<Event>::ErrorObserver::create<ObserverHsm, &ObserverHsm::onError>(*this));
    }

    void initialTransitionTo(State<Event>& state) { m_stateMachine.initialTransitionTo(state); }
    void handleEvent(const Event& event) { m_stateMachine.handleEvent(event); }
    const State<Event>* getCurrentState() { return m_stateMachine.getCurrentState(); }

    // Observer callbacks.
    void onTransition(const State<Event>& state, TransitionAction action) {
        transitions.push_back(std::string(state.name) + (action == TransitionAction::Entry ? ":entry" : ":exit"));
    }
    void onUnhandledEvent(const Event& event) { unhandledEventCount++; }
    void onError(Error error) {
        errorCount++;
        lastError = error;
    }

    // State handlers.
    void parent_entry() {}
    void parent_event(const Event& event) {
        if (event.id == EventId::GO_TO_STATE_1A) {
            m_stateMachine.transitionTo(child);
        }
    }
    void parent_exit() {}

    void child_entry() {}
    void child_event(const Event& event) {} // Deliberately handles nothing so events bubble.
    void child_exit() {}

    void loopA_entry() { m_stateMachine.transitionTo(loopB); }
    void loopA_event(const Event& event) {}
    void loopA_exit() {}

    void loopB_entry() { m_stateMachine.transitionTo(loopA); }
    void loopB_event(const Event& event) {}
    void loopB_exit() {}

    State<Event> parent;
    State<Event> child;
    State<Event> loopA;
    State<Event> loopB;
    StateMachine<Event> m_stateMachine;

    std::vector<std::string> transitions;
    int unhandledEventCount = 0;
    int errorCount = 0;
    Error lastError = Error::MaxRecursionDepthExceeded;
};

TEST(ObserverTests, TransitionObserverFiresOnEntryAndExit) {
    ObserverHsm hsm;

    hsm.initialTransitionTo(hsm.parent);
    EXPECT_THAT(hsm.transitions, ::testing::ElementsAre("Parent:entry"));

    // Parent handles the event by transitioning down into its child, so only the child is entered.
    {
        Event event(EventId::GO_TO_STATE_1A);
        hsm.handleEvent(event);
    }
    EXPECT_THAT(hsm.transitions, ::testing::ElementsAre("Parent:entry", "Child:entry"));

    // Child does not handle the event; it bubbles to the parent which transitions to the child
    // again. Because the destination equals the current state this is a self-transition, exiting
    // and re-entering the child.
    {
        Event event(EventId::GO_TO_STATE_1A);
        hsm.handleEvent(event);
    }
    EXPECT_THAT(hsm.transitions,
        ::testing::ElementsAre("Parent:entry", "Child:entry", "Child:exit", "Child:entry"));
}

TEST(ObserverTests, UnhandledEventObserverFiresOnlyWhenEventBubblesPastTop) {
    ObserverHsm hsm;
    hsm.initialTransitionTo(hsm.parent);

    // Nobody handles this event, so it should bubble past the top and notify the observer.
    {
        Event event(EventId::NO_ONE_HANDLES_THIS);
        hsm.handleEvent(event);
    }
    EXPECT_EQ(hsm.unhandledEventCount, 1);

    // This event causes a transition, so it is considered handled and must NOT notify.
    {
        Event event(EventId::GO_TO_STATE_1A);
        hsm.handleEvent(event);
    }
    EXPECT_EQ(hsm.unhandledEventCount, 1);
}

TEST(ObserverTests, ErrorObserverFiresOnMaxRecursionDepthExceeded) {
    ObserverHsm hsm;

    // LoopA's entry transitions to LoopB whose entry transitions back to LoopA, recursing until
    // the max depth guard trips.
    hsm.initialTransitionTo(hsm.loopA);

    EXPECT_GE(hsm.errorCount, 1);
    EXPECT_EQ(hsm.lastError, Error::MaxRecursionDepthExceeded);
}

TEST(ObserverTests, CanRecoverByTransitioningToAKnownStateAfterMaxRecursionDepthExceeded) {
    ObserverHsm hsm;

    // Trip the max recursion depth guard. This leaves the current state indeterminate, but the
    // internal recursion counter is reset once the outermost transitionTo() unwinds.
    hsm.initialTransitionTo(hsm.loopA);
    EXPECT_GE(hsm.errorCount, 1);

    const int errorsAfterTrip = hsm.errorCount;

    // The documented recovery: transition to a known-good state. This must succeed (no further
    // error) and leave the state machine in that state.
    hsm.initialTransitionTo(hsm.parent);
    EXPECT_EQ(hsm.getCurrentState(), &hsm.parent);
    EXPECT_EQ(hsm.errorCount, errorsAfterTrip);
}
