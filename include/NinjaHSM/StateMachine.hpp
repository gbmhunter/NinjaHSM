#pragma once

#include <cstdint>

#include "State.hpp"

namespace NinjaHSM {

/**
 * The maximum number of times transitionTo() can be called recursively.
 * This is to prevent infinite recursion in case of a bug. transitionTo()
 * is called recursively if it called within a state's entry() or exit()
 * methods.
 */
constexpr uint32_t MAX_RECURSION_COUNT = 50;

template <typename EventType>
class StateMachine {
public:
    StateMachine() {}

    /**
     * Perform the transition to the provided initial state. This function should be called before
     * calling handleEvent() for the first time.
     * 
     * @param[in] state The initial state to transition to.
     */
    void initialTransitionTo(const State<EventType>& state) {
        transitionTo(state);
    }

    /**
     * Provide an event to the state machine. The state machine will call then current state's
     * onEvent() function.
     * 
     * @param[in] event The event to handle.
     */
    void handleEvent(const EventType& event) {
        // The event handler could call transitionTo() to change the state, and/or
        // call eventHandled() to indicate that the event was handled. If any of these
        // occur, we do not want to propagate the event to the parent state.
        m_transitionToCalled = false;
        m_eventHandledCalled = false;
        const State<EventType>* stateToHandleEvent = m_currentState;
        while (stateToHandleEvent != nullptr) {
            stateToHandleEvent->event(event);
            if (m_transitionToCalled || m_eventHandledCalled) {
                break;
            }
            stateToHandleEvent = stateToHandleEvent->parent;
        }
    }

    /**
     * Get the current state of the state machine. Can be nullptr before initial transition occurs
     * due to transitionTo() being called.
     * 
     * @return A pointer to the current state.
     */
    const State<EventType>* getCurrentState() {
        return m_currentState;
    }

    /**
     * @brief Trigger a transition to a state.
     * @param state The state to transition to.
     */
    void transitionTo(const State<EventType>& state) {
        m_transitionToCalled = true;
        m_maxRecursionCount++;
        if (m_maxRecursionCount > MAX_RECURSION_COUNT) {
            return;
        }
        uint32_t ourRecursionCount = m_maxRecursionCount;

        // Rename just for readability below
        const State<EventType>* destinationState = &state;

        // If the new destination state is a child of the previous entry() function,
        // we don't want to re-call the entry() function (we assume the state was entered).
        if (m_calledEntryState != nullptr && isChildOf(m_calledEntryState, destinationState)) {
            m_currentState = m_calledEntryState;
            m_calledEntryState = nullptr; // Clear flag
        }

        if (m_calledExitState != nullptr && !isChildOf(m_calledExitState, destinationState)) {
            m_currentState = m_calledExitState->parent;
            m_calledExitState = nullptr; // Clear flag
        }

        if (m_currentState == destinationState) {
            m_currentState->exit();
            if (ourRecursionCount != m_maxRecursionCount) {
                goto END;
            }
            m_currentState = m_currentState->parent;
        }

        // This loop handles one entry or exit per iteration.
        while (m_currentState != destinationState) {
            // Logic:
            // Search for the current state in the tree containing the destination
            // state and all of it's parents. If the current state is found,
            // Move down one state. If the current state is not found there, we
            // need to move to the current state's parent
            const State<EventType>* stateInDestinationBranch = destinationState;
            bool foundCurrentStateInDestinationBranch = false;
            while (stateInDestinationBranch != nullptr) {
                if (stateInDestinationBranch->parent == m_currentState) {
                    foundCurrentStateInDestinationBranch = true;
                    break;
                }
                stateInDestinationBranch = stateInDestinationBranch->parent;
                // if (stateInDestinationBranch == nullptr) {
                //     std::cout << "State in destination branch is null. This should never happen." << std::endl;
                //     break;
                // }
            }

            if (foundCurrentStateInDestinationBranch) {
                // We've found the current state in the destination branch.
                // Move down one state.
                m_calledEntryState = stateInDestinationBranch;
                stateInDestinationBranch->entry();
                m_calledEntryState = nullptr;
                if (ourRecursionCount != m_maxRecursionCount) {
                    break;
                }
                m_currentState = stateInDestinationBranch;
                continue;
            }

            // If we get here, we need to exit the current state.
            // Transition to the top most parent of the destination state.
            m_calledExitState = m_currentState;
            m_currentState->exit();
            m_calledExitState = nullptr; // Clear flag
            if (ourRecursionCount != m_maxRecursionCount) {
                break;
            }
            m_currentState = m_currentState->parent; // This might be nullptr
        }

        END:

        // If we are at the top of the recursion, reset the recursion index so it's
        // ready for the next non-recursive transitionTo() call.
        if (ourRecursionCount == 1) {
            m_maxRecursionCount = 0;
        }
    } // transitionTo()

    /**
     * Indicate to the state machine that an event was handled and event bubbling should stop.
     * This function should be called only inside state onEvent() functions.
     * 
     * Calling transitionTo() from within a state's onEvent() function will also stop event bubbling.
     */
    void eventHandled() {
        m_eventHandledCalled = true;
    }

protected:

    const State<EventType>* m_currentState = nullptr;

    bool m_transitionToCalled = false;

    bool m_eventHandledCalled = false;

    /**
     * Set to a valid state pointer just before calling a states entry() function.
     * This is used to avoid re-calling the entry() function is the entry function
     * calls transitionTo() to a CHILD state.
     */
    const State<EventType>* m_calledEntryState = nullptr;

    /**
     * Set to a valid state pointer just before calling a states exit() function.
     * This is used to avoid re-calling the exit() function is the exit function
     * calls transitionTo() to a PARENT state.
     */
    const State<EventType>* m_calledExitState = nullptr;

    /**
     * Keeps track of how many times transitionTo() has been called recursively.
     */
    uint32_t m_maxRecursionCount = 0;

    /**
     * Check if a child state is a child of a parent state.
     * 
     * @param parent The parent state.
     * @param child The potential child state.
     * @return True if the child is a child of the parent, false otherwise.
     */
    bool isChildOf(const State<EventType>* parent, const State<EventType>* child) {
        const State<EventType>* state = child;
        while (state != nullptr) {
            if (state == parent) {
                return true;
            }
            state = state->parent;
        }
        return false;
    }
}; // class StateMachine

}; // namespace NinjaHSM
