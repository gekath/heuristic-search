/*
 * weighted_a.h
 *
 * LICENSE HERE
 *
 *  Created on: 2016-09-22
 *      Author: Rick Valenzano
 */

#ifndef WEIGHTED_A_H_
#define WEIGHTED_A_H_

#include "best_first_search.h"
#include "a_star.h"

template<class state_t, class action_t>
class WeightedAStar: public AStar<state_t, action_t>
{
public:
    WeightedAStar();
    virtual ~WeightedAStar();
    void setWeights(double weights);
    double weight;

protected:
    virtual double nodeEval(const state_t &state, double g_cost, double h_cost);

};

template<class state_t, class action_t>
inline WeightedAStar<state_t, action_t>::WeightedAStar()
{
}

template<class state_t, class action_t>
inline WeightedAStar<state_t, action_t>::~WeightedAStar()
{
}

template<class state_t, class action_t>
inline double WeightedAStar<state_t, action_t>::nodeEval(const state_t& state, double g_cost, double h_cost)
{
    return g_cost + weight * h_cost;
}

template<class state_t, class action_t>
void WeightedAStar<state_t, action_t>::setWeights(double new_weight)
{
    weight = new_weight;
}

#endif /* A_STAR_H_ */

