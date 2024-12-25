#pragma once

#include "State.hpp"

namespace NinjaHsm {

class StateMachine {
public:
    StateMachine();
    ~StateMachine();

    void addState(State state);


    void transitionTo(State state);
};

};