/*
 * transition_system.h
 *
 * LICENSE HERE
 *
 *  Created on: 2016-08-15
 *      Author: Rick Valenzano
 */

#ifndef TRANSITIONSYSTEM_H_
#define TRANSITIONSYSTEM_H_

#include <vector>

/**
 * An abstract class defining the way transition systems are to be applied.
 *
 * @todo Add debug mode?
 *
 * @class TransitionSystem
 */
template<class state_t, class action_t>
class TransitionSystem
{
public:
    /**
     * Constructor for the abstract transition system class. Abstract constructor is empty.
     */
    TransitionSystem();

    /**
     * Destructor for the abstract transition system class. Abstract destructor is empty.
     */
    virtual ~TransitionSystem();

    /**
     * Checks if the action is applicable in the given state.
     *
     * @param state The state in the domain.
     * @param action The action to test for applicability.
     * @return If the given action is applicable in the given state.
     */
    virtual bool isApplicable(const state_t &state, const action_t &action) const;

    /**
     * Checks if the given states are adjacent.
     *
     * @param parent The potential parent state.
     * @param child The potential child state.
     * @return If the given child state is adjacent to the given child state.
     */
    virtual bool isChildState(const state_t &parent, const state_t &child) const;

    /**
     * Returns the cost of applying the given action in the given state.
     *
     * Assumes the action is actually applicable in that state.
     *
     * @param state The state to apply the action in.
     * @param action The action to apply.
     * @return The cost of applying the action in the given state.
     */
    virtual double getActionCost(const state_t &state, const action_t &action) const = 0;

    /**
     * Applies the given action to the given state.
     *
     * Assumes the given action is applicable in the given state.
     *
     * @param state The state to update.
     * @param action The action to apply.
     */
    virtual void applyAction(state_t &state, const action_t &action) const = 0;

    /**
     * Gets the actions applicable in a given state and appends them to the given list.
     *
     * @param state The state to generate actions in.
     * @param actions The list of actions to append to.
     */
    virtual void getActions(const state_t &state, std::vector<action_t> &actions) const = 0;

    /**
     * Gets the successors of a given state and appends them to the given list.
     *
     * By default just generates list of applicable actions and generates the corresponding children.
     *
     * @param state The state to generate successors for.
     * @param children The list to append the children to.
     */
    virtual void getSuccessors(const state_t &state, std::vector<state_t> &children) const;

    /**
     * Checks if the given action is invertible in the given state.
     *
     * Assumes the action is either the dummy action, or is applicable in the given state.
     *
     * @param state The state the action is being applied in.
     * @param action The action to check.
     * @return If the action is invertible.
     */
    virtual bool isInvertible(const state_t &state, const action_t &action) const;

    /**
     * Returns the inverse of a given action.
     *
     * If no inverse exists, returns the dummy action.
     *
     * @param state The state the action is being applied in.
     * @param action The action to invert.
     * @return The inverse of the given action.
     */
    virtual action_t getInverse(const state_t &state, const action_t &action) const;

    /**
     * Returns the dummy action for this domain.
     *
     * The dummy action should never actually be applicable and is not invertible.
     *
     * @return The domain's dummy action.
     */
    virtual action_t getDummyAction() const = 0;

    /**
     * Checks if the given action is the dummy action for this domain.
     *
     * @param action The action to check.
     * @return If the given action is the dummy action.
     */
    virtual bool isDummyAction(const action_t &action) const = 0;

    /**
     * Applies the given sequence of actions to the given state.
     *
     * Returns false if the sequence is not entirely applicable. The state will be left at the point immediately prior to the sequence no longer being applicable.
     *
     * @param state The state to apply the sequence to.
     * @param actions The sequence of actions to apply.
     * @return If all actions in the sequence could be applied.
     */
    bool applyActionSequence(state_t &state, const std::vector<action_t> &actions) const;

    /**
     * Checks if the given sequence of actions is applicable to the given state.
     *
     * @param state The state to try the sequence on.
     * @param actions The sequence of actions to test.
     * @return If the actions can be applied sequentially on the given state.
     */
    bool isApplicableSequence(const state_t &state, const std::vector<action_t> &actions) const;
};

template<class state_t, class action_t>
TransitionSystem<state_t, action_t>::TransitionSystem()
{
}

template<class state_t, class action_t>
TransitionSystem<state_t, action_t>::~TransitionSystem()
{
}

template<class state_t, class action_t>
bool TransitionSystem<state_t, action_t>::isApplicable(const state_t &state, const action_t &action) const
{
    if(action == getDummyAction())
        return false;

    std::vector<action_t> actions;

    getActions(state, actions);

    for(unsigned i = 0; i < actions.size(); i++) {
        if(action == actions[i])
            return true;
    }

    return false;
}

template<class state_t, class action_t>
bool TransitionSystem<state_t, action_t>::isChildState(const state_t &parent, const state_t &child) const
{
    std::vector<state_t> children;

    getSuccessors(parent, children);

    for(unsigned i = 0; i < children.size(); i++) {
        if(child == children[i])
            return true;
    }

    return false;
}

template<class state_t, class action_t>
void TransitionSystem<state_t, action_t>::getSuccessors(const state_t &state, std::vector<state_t> &children) const
{
    std::vector<action_t> actions;

    getActions(state, actions);

    for(unsigned i = 0; i < actions.size(); i++) {
        state_t state_copy(state);
        applyAction(state_copy, actions[i]);

        children.push_back(state_copy);
    }
}

template<class state_t, class action_t>
bool TransitionSystem<state_t, action_t>::isInvertible(const state_t &state, const action_t &action) const {
    return false;
}

template<class state_t, class action_t>
action_t TransitionSystem<state_t, action_t>::getInverse(const state_t &state, const action_t &action) const {
    return getDummyAction();
}


template<class state_t, class action_t>
bool TransitionSystem<state_t, action_t>::applyActionSequence(state_t &state,
        const std::vector<action_t> &actions) const
{
    for(unsigned i = 0; i < actions.size(); i++) {
        if(!isApplicable(state, actions[i]))
            return false;
        applyGivenAction(state, actions[i]);
    }

    return true;
}

template<class state_t, class action_t>
bool TransitionSystem<state_t, action_t>::isApplicableSequence(const state_t &state,
        const std::vector<action_t> &actions) const
{
    state_t state_copy(state);

    if(!applyActionSequence(state, actions))
        return false;

    return true;
}
#endif /* TRANSITIONSYSTEM_H_ */
