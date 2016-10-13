/*
 * a_star.h
 *
 * LICENSE HERE
 *
 *  Created on: 2016-09-22
 *      Author: Rick Valenzano
 */

#ifndef A_STAR_H_
#define A_STAR_H_

#include "best_first_search.h"

template<class state_t, class action_t>
class AStar: public BestFirstSearch<state_t, action_t>
{
public:
    AStar();
    virtual ~AStar();

protected:
    virtual double nodeEval(const state_t &state, double g_cost, double h_cost);
};

template<class state_t, class action_t>
inline AStar<state_t, action_t>::AStar()
{
}

template<class state_t, class action_t>
inline AStar<state_t, action_t>::~AStar()
{
}

template<class state_t, class action_t>
inline double AStar<state_t, action_t>::nodeEval(const state_t& state, double g_cost, double h_cost)
{
    return g_cost + h_cost;
}

#endif /* A_STAR_H_ */
