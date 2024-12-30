#pragma once

#include <array>
#include <iostream>

#include "State.hpp"

constexpr uint32_t MAX_RECURSION_COUNT = 10;

namespace NinjaHSM {

template <typename Event>
class StateMachine {
public:
    StateMachine() {
        states.fill(nullptr);
    }

    void addState(State<Event> * state) {
        std::cout << "Adding state at index: " << numStates << std::endl;
        states[numStates] = state;
        numStates++;
    }

    void initialTransitionTo(State<Event> * state) {
        transitionTo(state);
    }

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

    State<Event>* getCurrentState() {
        return currentState;
    }

    void eventHandled() {
        eventHandledCalled = true;
    }

protected:

    std::array<State<Event>*, 10> states;

    std::size_t numStates = 0;

    State<Event>* currentState = nullptr;

    bool transitionToCalled = false;

    bool eventHandledCalled = false;

    State<Event>* calledEntryState = nullptr;

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

            // If we get here, we didn't find the current state in the destination branch.
            if (currentState == nullptr)  {
                std::cout << "Current state is null or has no parent. Transitioning to top most parent of destination state." << std::endl;
                // Transition to the top most parent of the destination state.
                // No exit of current state because there is no current state!
                stateInDestinationBranch->entry();
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
            currentState->exit();
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
