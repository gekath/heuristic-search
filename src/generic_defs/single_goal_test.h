/*
 * single_goal_test.h
 *
 * LICENSE HERE
 *
 *  Created on: 2016-08-18
 *      Author: Rick Valenzano
 */

#ifndef SINGLEGOALTEST_H_
#define SINGLEGOALTEST_H_

#include "goal_test_function.h"

/**
 * Defines a goal test function when there is a single goal state.
 *
 * @class SingleGoalTest
 */
template<class state_t>
class SingleGoalTest: public GoalTestFunction<state_t>
{
public:
    /**
     * Constructs a goal test function where there is a given single goal state.
     *
     * @param g The goal state.
     */
    SingleGoalTest(const state_t &g);

    /**
     * Destructor for a single goal test function. Destructor is empty.
     */
    virtual ~SingleGoalTest();

    virtual bool isGoal(const state_t &state) const;

    /**
     * Resets the goal state to a new given state.
     *
     * @param g The new goal state.
     */
    virtual void setGoal(const state_t &g);

    /**
     * Returns the current goal state.
     *
     * @return The current goal state.
     */
    state_t getCurrentGoal() const;

protected:
    state_t goal; ///< The stored goal state.
};

template<class state_t>
SingleGoalTest<state_t>::SingleGoalTest(const state_t& goal_state)
        : goal(goal_state)
{
}

template<class state_t>
SingleGoalTest<state_t>::~SingleGoalTest()
{
}

template<class state_t>
bool SingleGoalTest<state_t>::isGoal(const state_t& state) const
{
    return (goal == state);
}

template<class state_t>
void SingleGoalTest<state_t>::setGoal(const state_t& g)
{
    goal = g;
}

template<class state_t>
state_t SingleGoalTest<state_t>::getCurrentGoal() const
{
    return goal;
}

#endif /* SINGLEGOALTEST_H_ */
