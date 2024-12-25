#pragma once

#include "State.hpp"

namespace NinjaHsm {

template <typename Event>
class StateMachine {
public:
    StateMachine() {
        states.fill(nullptr);
    }

    void addState(State state) {
        std::cout << "Adding state at index: " << numStates << std::endl;
        states[numStates] = &state;
        numStates++;
    }


    void handleEvent(Event event);

private:

    std::array<State*, 10> states;

    std::size_t numStates = 0;

    State* currentState = nullptr;

    /**
     * @brief Trigger a transition to a state.
     * @param state The state to transition to.
     */
    void transitionTo(State state) {
        // If current state is null, this means this is the initial transition.
        if (currentState == nullptr) {
            currentState = state;
            currentState->entry();
        }
    }
}; // class StateMachine

}; // namespace NinjaHsm