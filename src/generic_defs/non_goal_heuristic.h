/*
 * non_goal_heuristic.h
 *
 * LICENSE HERE
 *
 *  Created on: 2016-08-31
 *      Author: Rick Valenzano
 */

#ifndef NON_GOAL_HEURISTIC_H_
#define NON_GOAL_HEURISTIC_H_

#include "heuristic.h"
#include "goal_test_function.h"

// TODO Add debugging for invalid minimum action costs

/**
 * Defines a heuristic that returns 0 for any goal state, and the minimum action cost for any other state.
 *
 * @class NonGoalHeuristic
 */
template<class state_t>
class NonGoalHeuristic: public Heuristic<state_t>
{
public:
    /**
     * Constructs a non-goal heuristic for the given goal test function and minimum action cost.
     * @param g The goal test function. Default is set to none.
     * @param min_cost The minimum action cost.
     */
    NonGoalHeuristic(const GoalTestFunction<state_t> *g = 0, double min_cost = 0);

    /**
     * Destructor for a non-goal heuristic. Destructor is empty.
     */
    virtual ~NonGoalHeuristic();

    /**
     * Sets the goal test function to the new given function.
     *
     * @param g The new goal test function.
     */
    void setGoalTestFunction(const GoalTestFunction<state_t> *g);

    /**
     * Sets the minimum action cost to the new minimum cost.
     *
     * @param min_cost The new minimum cost.
     */
    void setMinimumActionCost(double min_cost);

protected:
    virtual double computeHValue(const state_t &state) const;

    const GoalTestFunction<state_t> *goal_test; ///< The stored goal test function.
    double min_action_cost; ///< The minimum cost of any action.
};

/**
 * Creates a blind heuristic function that returns 0 for all states.
 *
 * Note that the blind heuristic is an instance of a NonGoalHeuristic.
 *
 * @return A blind heuristic function.
 */
template<class state_t>
Heuristic<state_t> * getBlindHeuristic();

template<class state_t>
NonGoalHeuristic<state_t>::NonGoalHeuristic(const GoalTestFunction<state_t>* g, double min_cost)
        : goal_test(g), min_action_cost(min_cost)
{
}

template<class state_t>
NonGoalHeuristic<state_t>::~NonGoalHeuristic()
{
}

template<class state_t>
inline void NonGoalHeuristic<state_t>::setGoalTestFunction(const GoalTestFunction<state_t>* g)
{
    goal_test = g;
}

template<class state_t>
void NonGoalHeuristic<state_t>::setMinimumActionCost(double min_cost)
{
    min_action_cost = min_cost;
}

template<class state_t>
double NonGoalHeuristic<state_t>::computeHValue(const state_t& state) const
{
    if(goal_test && goal_test->isGoal(state))
        return 0.0;
    return min_action_cost;
}

template<class state_t>
Heuristic<state_t> * getBlindHeuristic()
{
    Heuristic<state_t> * h = new NonGoalHeuristic<state_t>();
    return h;
}

#endif /* NON_GOAL_HEURISTIC_H_ */
