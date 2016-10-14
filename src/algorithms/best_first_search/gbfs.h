/*
 * weighted_a.h
 *
 * LICENSE HERE
 *
 *  Created on: 2016-09-22
 *      Author: Rick Valenzano
 */

#ifndef GBFS_H_
#define GBFS_H_

#include "best_first_search.h"
#include "a_star.h"

template<class state_t, class action_t>
class GBFS: public AStar<state_t, action_t>
{
public:
    GBFS();
    virtual ~GBFS();

protected:
    virtual double nodeEval(const state_t &state, double g_cost, double h_cost);
};

template<class state_t, class action_t>
inline GBFS<state_t, action_t>::GBFS()
{
}

template<class state_t, class action_t>
inline GBFS<state_t, action_t>::~GBFS()
{
}

template<class state_t, class action_t>
inline double GBFS<state_t, action_t>::nodeEval(const state_t& state, double g_cost, double h_cost)
{
    return h_cost;
}

#endif /* GBFS_H_ */

