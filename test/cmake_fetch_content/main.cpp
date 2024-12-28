#include "NinjaHSM/NinjaHSM.hpp"

using namespace NinjaHSM;

class Event {

};

class TestHsm : public NinjaHSM::StateMachine<Event> {

};

int main() {
    TestHsm hsm;
    return 0;
}