#pragma once

#include "State.hpp"

namespace NinjaHsm {

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

    /**
     * @brief Trigger a transition to a state.
     * @param state The state to transition to.
     */
    void transitionTo(State<Event> * state) {
        std::cout << "transitionTo() called." << std::endl;
        transitionToCalled = true;
        if (currentState == nullptr) {
            std::cout << "Current state is null. Transitioning to initial state." << std::endl;
        }
        std::cout << "Destination state: " << state->name << std::endl;

        // Rename just for readability below
        State<Event>* destinationState = state;

        if (currentState == destinationState) {
            std::cout << "Current state is the same as the destination state. Exiting and entering again." << std::endl;
            currentState->exit();
            currentState->entry();
        }

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
            while (stateInDestinationBranch->parent != nullptr) {
                if (stateInDestinationBranch->parent == currentState) {
                    // We've found the current state in the destination branch.
                    // Move down one state.
                    std::cout << "Found current state in destination branch. Moving down one state to: " << stateInDestinationBranch->name << std::endl;
                    // currentState->exit();
                    stateInDestinationBranch->entry();
                    currentState = stateInDestinationBranch;
                    foundCurrentStateInDestinationBranch = true;
                    break;
                }
                stateInDestinationBranch = stateInDestinationBranch->parent;
            }

            if (foundCurrentStateInDestinationBranch) {
                break;
            }

            // If we get here, we didn't find the current state in the destination branch.
            if (currentState == nullptr)  {
                std::cout << "Current state is null or has no parent. Transitioning to top most parent of destination state." << std::endl;
                // Transition to the top most parent of the destination state.
                // No exit of current state because there is no current state!
                stateInDestinationBranch->entry();
                currentState = stateInDestinationBranch;
                continue;
            }

            if (currentState->parent == nullptr) {
                std::cout << "Current state has no parent. Transitioning to top most parent of destination state." << std::endl;
                // Transition to the top most parent of the destination state.
                currentState->exit();
                stateInDestinationBranch->entry();
                currentState = stateInDestinationBranch;
                continue;
            }

            // If we get here, we need to transition to the parent of the current state.
            currentState->exit();
            // Don't need call entry(), it's a parent of the current state so we are
            // already in it.
            // currentState->parent->entry();
            currentState = currentState->parent;
        }
        std::cout << "transitionTo() finished. Transitioned to state: " << currentState->name << std::endl;
    } // transitionTo()
}; // class StateMachine

}; // namespace NinjaHsm