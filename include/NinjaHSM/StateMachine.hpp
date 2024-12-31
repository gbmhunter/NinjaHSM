#pragma once

#include <array>
#include <iostream>

#include "State.hpp"


namespace NinjaHSM {

constexpr uint32_t MAX_RECURSION_COUNT = 10;
constexpr uint32_t MAX_STATES = 100;

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
            std::cout << "Max recursion count reached. Aborting transition." << std::endl;
            return;
        }
        uint32_t ourRecursionCount = maxRecursionCount;
        std::cout << "transitionTo() called. Our recursion count is: " << ourRecursionCount << std::endl;
        if (currentState == nullptr) {
            std::cout << "Current state is null." << std::endl;
        }
        std::cout << "Destination state: " << state->name << std::endl;

        // Rename just for readability below
        State<Event>* destinationState = state;


        // If the new destination state is a child of the previous entry() function,
        // we don't want to re-call the entry() function (we assume the state was entered).
        if (calledEntryState != nullptr && isChildOf(calledEntryState, destinationState)) {
            std::cout << "Just called entry state is a parent of the destination state.Assuming state was entered." << std::endl;
            currentState = calledEntryState;
            calledEntryState = nullptr; // Clear flag
        }

        if (calledExitState != nullptr && !isChildOf(calledExitState, destinationState)) {
            std::cout << "Just called exit state is not a child of the destination state. Assuming state was exited." << std::endl;
            currentState = calledExitState->parent;
            calledExitState = nullptr; // Clear flag
        }

        if (currentState == destinationState) {
            std::cout << "Current state is the same as the destination state. Exiting and entering again." << std::endl;
            currentState->exit();
            if (ourRecursionCount != maxRecursionCount) {
                std::cout << "Recursion detected. Aborting transition." << std::endl;
                goto END;
            }
            currentState = currentState->parent;
        }

        // This loop handles one entry or exit per iteration.
        while (currentState != destinationState) {
            if (currentState == nullptr) {
                std::cout << "Current state is null." << std::endl;
            } else {
                std::cout << "Current state: " << currentState->name << std::endl;
            }
            std::cout << "Destination state: " << destinationState->name << std::endl;
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
                std::cout << "Found current state in destination branch. Moving down one state to: " << stateInDestinationBranch->name << std::endl;
                calledEntryState = stateInDestinationBranch;
                stateInDestinationBranch->entry();
                calledEntryState = nullptr;
                if (ourRecursionCount != maxRecursionCount) {
                    std::cout << "Recursion detected. Aborting transition." << std::endl;
                    break;
                }
                std::cout << "Setting current state to: " << stateInDestinationBranch->name << std::endl;
                currentState = stateInDestinationBranch;
                continue;
            }

            // If we get here, we need to exit the current state.
            std::cout << "Need to exit current state." << std::endl;
            // Transition to the top most parent of the destination state.
            calledExitState = currentState;
            currentState->exit();
            calledExitState = nullptr; // Clear flag
            if (ourRecursionCount != maxRecursionCount) {
                std::cout << "Recursion detected. Aborting transition." << std::endl;
                break;
            }
            currentState = currentState->parent; // This might be nullptr
        }

        END:
        std::cout << "transitionTo() finished. Our recursion index is: " << ourRecursionCount << ", current state is: " << currentState->name << std::endl;

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
