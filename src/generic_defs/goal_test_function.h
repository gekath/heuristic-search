/*
 * goal_test_function.h
 *
 * LICENSE HERE
 *
 *  Created on: 2016-08-18
 *      Author: Rick Valenzano
 */

#ifndef GOALTESTFUNCTION_H_
#define GOALTESTFUNCTION_H_

/**
 * Abstract class for defining a goal test function.
 *
 * @class GoalTestFunction
 */
template<class state_t>
class GoalTestFunction
{
public:
    /**
     * Constructor for a abstract goal test class. Abstract constructor is empty.
     */
    GoalTestFunction();

    /**
     * Destructor for the abstract goal test class. Abstract destructor is empty.
     */
    virtual ~GoalTestFunction();

    /**
     * Performs the goal test on the given state.
     *
     * @param state The state to test.
     * @return If the given state is a goal state.
     */
    virtual bool isGoal(const state_t &state) const = 0;
};

template<class state_t>
GoalTestFunction<state_t>::GoalTestFunction()
{
}

template<class state_t>
GoalTestFunction<state_t>::~GoalTestFunction()
{
}

#endif /* GOALTESTFUNCTION_H_ */
