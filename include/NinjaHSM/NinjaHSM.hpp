#pragma once

#include <functional>

enum class NinjaHSMRc : int32_t {
    SUCCESS = 0,
    MAX_STATES_REACHED = -1,
};

#include "State.hpp"
#include "StateMachine.hpp"
