// Minimal translation unit that instantiates the public NinjaHSM API. It is used by CI to verify
// that the headers compile for embedded targets: cross-compiled for ARM, and with C++ exceptions
// and RTTI disabled (as embedded builds typically do). It deliberately avoids <iostream>, printf
// and dynamic allocation so it is representative of a bare-metal build.
#include <NinjaHSM/NinjaHSM.hpp>

using namespace NinjaHSM;

namespace {

struct Event {
    int id;
};

class Machine {
public:
    Machine() :
        m_idle(makeState<Event,
            &Machine::idle_entry, &Machine::idle_event, &Machine::idle_exit>("Idle", *this)),
        m_running(makeState<Event,
            &Machine::running_entry, &Machine::running_event, &Machine::running_exit>("Running", *this, &m_idle)),
        // A state that omits its entry()/exit() handlers by passing nullptr (only reacts to events).
        m_paused(makeState<Event,
            nullptr, &Machine::paused_event, nullptr>("Paused", *this, &m_idle)),
        m_sm() {
        m_sm.setTransitionObserver(
            StateMachine<Event>::TransitionObserver::create<Machine, &Machine::onTransition>(*this));
        m_sm.setUnhandledEventObserver(
            StateMachine<Event>::UnhandledEventObserver::create<Machine, &Machine::onUnhandledEvent>(*this));
        m_sm.setErrorObserver(
            StateMachine<Event>::ErrorObserver::create<Machine, &Machine::onError>(*this));
        m_sm.initialTransitionTo(m_idle);
    }

    void step(const Event& event) { m_sm.handleEvent(event); }

private:
    void idle_entry() {}
    void idle_event(const Event& event) {
        if (event.id == 1) {
            m_sm.transitionTo(m_running);
        }
    }
    void idle_exit() {}

    void running_entry() {}
    void running_event(const Event& event) {
        if (event.id == 2) {
            m_sm.transitionTo(m_paused);
        }
    }
    void running_exit() {}

    void paused_event(const Event& event) {}

    void onTransition(const State<Event>& state, TransitionAction action) {}
    void onUnhandledEvent(const Event& event) {}
    void onError(Error error) {}

    State<Event> m_idle;
    State<Event> m_running;
    State<Event> m_paused;
    StateMachine<Event> m_sm;
};

} // namespace

// Exported (non-internal-linkage) entry point so the translation unit produces a symbol and the
// instantiated code above is not discarded before it is compiled.
void ninjahsm_compile_check() {
    Machine machine;
    Event event{1};
    machine.step(event);
}
