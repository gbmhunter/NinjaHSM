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

/**
 * Describes whether a state is being entered or exited. Passed to a transition observer
 * (see StateMachine::setTransitionObserver()).
 */
enum class TransitionAction {
    Entry,
    Exit,
};

/**
 * Errors the state machine can report to an error observer (see StateMachine::setErrorObserver()).
 */
enum class Error {
    /**
     * transitionTo() recursed deeper than MAX_RECURSION_COUNT (almost always an unconditional
     * transitionTo() in an entry()/exit() method). The transition is abandoned and the state
     * machine may be left in an indeterminate state.
     */
    MaxRecursionDepthExceeded,
};

template <typename EventType>
class StateMachine {
public:
    /**
     * Observer called immediately after a state's entry() or exit() method runs. Useful for
     * logging/tracing transitions without having to instrument every state. The first argument
     * is the state that was entered/exited, the second whether it was an entry or an exit.
     */
    using TransitionObserver = etl::delegate<void(const State<EventType>&, TransitionAction)>;

    /**
     * Observer called when an event is not handled by the current state or any of its parents
     * (i.e. it bubbled past the top of the hierarchy without any state calling transitionTo()
     * or eventHandled()).
     */
    using UnhandledEventObserver = etl::delegate<void(const EventType&)>;

    /**
     * Observer called when the state machine encounters an internal error (see Error).
     */
    using ErrorObserver = etl::delegate<void(Error)>;

    StateMachine() {}

    /**
     * Set an observer to be notified each time a state is entered or exited. Pass a default
     * constructed (unbound) delegate to remove a previously set observer. When no observer is
     * set there is zero runtime cost beyond a single is_valid() check per entry/exit.
     *
     * @param[in] observer The observer to call, or an unbound delegate to clear.
     */
    void setTransitionObserver(TransitionObserver observer) {
        m_transitionObserver = observer;
    }

    /**
     * Set an observer to be notified when an event bubbles past the top of the state hierarchy
     * without being handled. Pass a default constructed (unbound) delegate to clear.
     *
     * @param[in] observer The observer to call, or an unbound delegate to clear.
     */
    void setUnhandledEventObserver(UnhandledEventObserver observer) {
        m_unhandledEventObserver = observer;
    }

    /**
     * Set an observer to be notified when the state machine encounters an internal error.
     * Pass a default constructed (unbound) delegate to clear.
     *
     * @param[in] observer The observer to call, or an unbound delegate to clear.
     */
    void setErrorObserver(ErrorObserver observer) {
        m_errorObserver = observer;
    }

    /**
     * Perform the transition to the provided initial state. This function should be called before
     * calling handleEvent() for the first time.
     * 
     * @param[in] state The initial state to transition to.
     */
    void initialTransitionTo(const State<EventType>& state) {
        transitionTo(state);
    }

    /**
     * Provide an event to the state machine. The state machine will call the current state's
     * event() function.
     * 
     * @param[in] event The event to handle.
     */
    void handleEvent(const EventType& event) {
        // The event handler could call transitionTo() to change the state, and/or
        // call eventHandled() to indicate that the event was handled. If any of these
        // occur, we do not want to propagate the event to the parent state.
        m_transitionToCalled = false;
        m_eventHandledCalled = false;
        const State<EventType>* stateToHandleEvent = m_currentState;
        while (stateToHandleEvent != nullptr) {
            // A state may have no event() handler (an unbound delegate); skip it so the event
            // bubbles up to the parent.
            if (stateToHandleEvent->event.is_valid()) {
                stateToHandleEvent->event(event);
            }
            if (m_transitionToCalled || m_eventHandledCalled) {
                break;
            }
            stateToHandleEvent = stateToHandleEvent->parent;
        }
        // If no state transitioned or claimed the event, it bubbled past the top of the
        // hierarchy unhandled. Let any observer know.
        if (!m_transitionToCalled && !m_eventHandledCalled && m_unhandledEventObserver.is_valid()) {
            m_unhandledEventObserver(event);
        }
    }

    /**
     * Get the current state of the state machine. Can be nullptr before initial transition occurs
     * due to transitionTo() being called.
     * 
     * @return A pointer to the current state.
     */
    const State<EventType>* getCurrentState() const {
        return m_currentState;
    }

    /**
     * @brief Trigger a transition to a state.
     * @param state The state to transition to.
     */
    void transitionTo(const State<EventType>& state) {
        m_transitionToCalled = true;
        m_recursionDepth++;
        if (m_recursionDepth > MAX_RECURSION_COUNT) {
            if (m_errorObserver.is_valid()) {
                m_errorObserver(Error::MaxRecursionDepthExceeded);
            }
            return;
        }
        uint32_t ourRecursionDepth = m_recursionDepth;

        // Rename just for readability below
        const State<EventType>* destinationState = &state;

        // If the new destination state is a child of the previous entry() function,
        // we don't want to re-call the entry() function (we assume the state was entered).
        if (m_calledEntryState != nullptr && isChildOf(m_calledEntryState, destinationState)) {
            m_currentState = m_calledEntryState;
            m_calledEntryState = nullptr; // Clear flag
        }

        if (m_calledExitState != nullptr && !isChildOf(m_calledExitState, destinationState)) {
            m_currentState = m_calledExitState->parent;
            m_calledExitState = nullptr; // Clear flag
        }

        if (m_currentState == destinationState) {
            exitState(m_currentState);
            if (ourRecursionDepth != m_recursionDepth) {
                goto END;
            }
            m_currentState = m_currentState->parent;
        }

        // This loop handles one entry or exit per iteration.
        while (m_currentState != destinationState) {
            // Logic:
            // Search for the current state in the tree containing the destination
            // state and all of it's parents. If the current state is found,
            // Move down one state. If the current state is not found there, we
            // need to move to the current state's parent
            const State<EventType>* stateInDestinationBranch = destinationState;
            bool foundCurrentStateInDestinationBranch = false;
            while (stateInDestinationBranch != nullptr) {
                if (stateInDestinationBranch->parent == m_currentState) {
                    foundCurrentStateInDestinationBranch = true;
                    break;
                }
                stateInDestinationBranch = stateInDestinationBranch->parent;
            }

            if (foundCurrentStateInDestinationBranch) {
                // We've found the current state in the destination branch.
                // Move down one state.
                m_calledEntryState = stateInDestinationBranch;
                enterState(stateInDestinationBranch);
                m_calledEntryState = nullptr;
                if (ourRecursionDepth != m_recursionDepth) {
                    break;
                }
                m_currentState = stateInDestinationBranch;
                continue;
            }

            // If we get here, we need to exit the current state.
            // Transition to the top most parent of the destination state.
            m_calledExitState = m_currentState;
            exitState(m_currentState);
            m_calledExitState = nullptr; // Clear flag
            if (ourRecursionDepth != m_recursionDepth) {
                break;
            }
            m_currentState = m_currentState->parent; // This might be nullptr
        }

        END:

        // If we are at the top of the recursion, reset the recursion index so it's
        // ready for the next non-recursive transitionTo() call.
        if (ourRecursionDepth == 1) {
            m_recursionDepth = 0;
        }
    } // transitionTo()

    /**
     * Indicate to the state machine that an event was handled and event bubbling should stop.
     * This function should be called only inside state event() functions.
     *
     * Calling transitionTo() from within a state's event() function will also stop event bubbling.
     */
    void eventHandled() {
        m_eventHandledCalled = true;
    }

protected:

    /**
     * Call a state's entry() method and then notify the transition observer (if set).
     *
     * @param[in] state The state to enter.
     */
    void enterState(const State<EventType>* state) {
        // The state may have no entry() handler (an unbound delegate).
        if (state->entry.is_valid()) {
            state->entry();
        }
        if (m_transitionObserver.is_valid()) {
            m_transitionObserver(*state, TransitionAction::Entry);
        }
    }

    /**
     * Call a state's exit() method and then notify the transition observer (if set).
     *
     * @param[in] state The state to exit.
     */
    void exitState(const State<EventType>* state) {
        // The state may have no exit() handler (an unbound delegate).
        if (state->exit.is_valid()) {
            state->exit();
        }
        if (m_transitionObserver.is_valid()) {
            m_transitionObserver(*state, TransitionAction::Exit);
        }
    }

    const State<EventType>* m_currentState = nullptr;

    bool m_transitionToCalled = false;

    bool m_eventHandledCalled = false;

    /**
     * Set to a valid state pointer just before calling a states entry() function.
     * This is used to avoid re-calling the entry() function is the entry function
     * calls transitionTo() to a CHILD state.
     */
    const State<EventType>* m_calledEntryState = nullptr;

    /**
     * Set to a valid state pointer just before calling a states exit() function.
     * This is used to avoid re-calling the exit() function is the exit function
     * calls transitionTo() to a PARENT state.
     */
    const State<EventType>* m_calledExitState = nullptr;

    /**
     * Keeps track of how many times transitionTo() has been called recursively.
     */
    uint32_t m_recursionDepth = 0;

    /**
     * Observers. Default constructed (unbound) until set via the corresponding setter. Unbound
     * delegates are never called (guarded by is_valid()).
     */
    TransitionObserver m_transitionObserver;
    UnhandledEventObserver m_unhandledEventObserver;
    ErrorObserver m_errorObserver;

    /**
     * Check if a child state is a child of a parent state.
     * 
     * @param parent The parent state.
     * @param child The potential child state.
     * @return True if the child is a child of the parent, false otherwise.
     */
    bool isChildOf(const State<EventType>* parent, const State<EventType>* child) {
        const State<EventType>* state = child;
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
