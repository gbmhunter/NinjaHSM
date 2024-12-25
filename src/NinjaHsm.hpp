namespace NinjaHsm {

class State {

};

class StateMachine {
public:
    StateMachine();
    ~StateMachine();

    void addState(State state);


    void transitionTo(State state);
};

};

