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

template <typename Event>
class StateMachine {
public:
    StateMachine() {}

    /**
     * Perform the transition to the provided initial state. This function should be called before
     * calling handleEvent() for the first time.
     * 
     * @param[in] state The initial state to transition to.
     */
    void initialTransitionTo(State<Event> * state) {
        transitionTo(state);
    }

    /**
     * Provide an event to the state machine. The state machine will call then current state's
     * onEvent() function.
     * 
     * @param[in] event The event to handle.
     */
    void handleEvent(Event * event) {
        // The event handler could call transitionTo() to change the state, and/or
        // call eventHandled() to indicate that the event was handled. If any of these
        // occur, we do not want to propagate the event to the parent state.
        transitionToCalled = false;
        eventHandledCalled = false;
        State<Event>* stateToHandleEvent = currentState;
        while (stateToHandleEvent != nullptr) {
            stateToHandleEvent->event(event);
            if (transitionToCalled || eventHandledCalled) {
                break;
            }
            stateToHandleEvent = stateToHandleEvent->parent;
        }
    }

    /**
     * Get the current state of the state machine.
     * 
     * @return A pointer to the current state.
     */
    State<Event>* getCurrentState() {
        return currentState;
    }

    /**
     * Indicate to the state machine that an event was handled and event bubbling should stop.
     * This function should be called only inside state onEvent() functions.
     * 
     * Calling transitionTo() from within a state's onEvent() function will also stop event bubbling.
     */
    void eventHandled() {
        eventHandledCalled = true;
    }

protected:

    State<Event>* currentState = nullptr;

    bool transitionToCalled = false;

    bool eventHandledCalled = false;

    /**
     * Set to a valid state pointer just before calling a states entry() function.
     * This is used to avoid re-calling the entry() function is the entry function
     * calls transitionTo() to a CHILD state.
     */
    State<Event>* calledEntryState = nullptr;

    /**
     * Set to a valid state pointer just before calling a states exit() function.
     * This is used to avoid re-calling the exit() function is the exit function
     * calls transitionTo() to a PARENT state.
     */
    State<Event>* calledExitState = nullptr;

    /**
     * Keeps track of how many times transitionTo() has been called recursively.
     */
    uint32_t maxRecursionCount = 0;

    /**
     * @brief Trigger a transition to a state.
     * @param state The state to transition to.
     */
    void transitionTo(State<Event> * state) {
        transitionToCalled = true;
        maxRecursionCount++;
        if (maxRecursionCount > MAX_RECURSION_COUNT) {
            return;
        }
        uint32_t ourRecursionCount = maxRecursionCount;

        // Rename just for readability below
        State<Event>* destinationState = state;


        // If the new destination state is a child of the previous entry() function,
        // we don't want to re-call the entry() function (we assume the state was entered).
        if (calledEntryState != nullptr && isChildOf(calledEntryState, destinationState)) {
            currentState = calledEntryState;
            calledEntryState = nullptr; // Clear flag
        }

        if (calledExitState != nullptr && !isChildOf(calledExitState, destinationState)) {
            currentState = calledExitState->parent;
            calledExitState = nullptr; // Clear flag
        }

        if (currentState == destinationState) {
            currentState->exit();
            if (ourRecursionCount != maxRecursionCount) {
                goto END;
            }
            currentState = currentState->parent;
        }

        // This loop handles one entry or exit per iteration.
        while (currentState != destinationState) {
            // Logic:
            // Search for the current state in the tree containing the destination
            // state and all of it's parents. If the current state is found,
            // Move down one state. If the current state is not found there, we
            // need to move to the current state's parent
            State<Event>* stateInDestinationBranch = destinationState;
            bool foundCurrentStateInDestinationBranch = false;
            while (stateInDestinationBranch != nullptr) {
                if (stateInDestinationBranch->parent == currentState) {
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
                calledEntryState = stateInDestinationBranch;
                stateInDestinationBranch->entry();
                calledEntryState = nullptr;
                if (ourRecursionCount != maxRecursionCount) {
                    break;
                }
                currentState = stateInDestinationBranch;
                continue;
            }

            // If we get here, we need to exit the current state.
            // Transition to the top most parent of the destination state.
            calledExitState = currentState;
            currentState->exit();
            calledExitState = nullptr; // Clear flag
            if (ourRecursionCount != maxRecursionCount) {
                break;
            }
            currentState = currentState->parent; // This might be nullptr
        }

        END:

        // If we are at the top of the recursion, reset the recursion index so it's
        // ready for the next non-recursive transitionTo() call.
        if (ourRecursionCount == 1) {
            maxRecursionCount = 0;
        }
    } // transitionTo()

    bool isChildOf(State<Event>* parent, State<Event>* child) {
        State<Event>* state = child;
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
