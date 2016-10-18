/*
 * best_first_search.h
 *
 * LICENSE HERE
 *
 *  Created on: 2016-09-11
 *      Author: Rick Valenzano
 */

#ifndef BEST_FIRST_SEARCH_H_
#define BEST_FIRST_SEARCH_H_

#include "../../utils/combinatorics.h"
#include "../../generic_defs/search_engine.h"
#include "open_closed_list.h"
#include "../../generic_defs/heuristic.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

/**
 * Defines the possible results of expanding a single node.
 *
 * Below is the meaning of each of the values:
 *
 * goal_found - a goal has been found.\n
 * no_solution - no solution was found.\n
 * res_limit - a resource limit has been hit.\n
 * empty_open - the open list is empty.\n
 *
 * @class BfsExpansionResult
 */
enum class BfsExpansionResult
{
    goal_found, no_solution, res_limit, empty_open
};

/**
 *
 * An abstract template for best-first search.
 *
 * @todo Add branch and bound option
 * @todo Add pathmax
 * @todo Make it a bit more general
 * @todo Are these the return types that we want for BfsExpansionResult
 *
 * @class BestFirstSearch
 */
template<class state_t, class action_t>
class BestFirstSearch: public SearchEngine<state_t, action_t>
{
    using SearchEngine<state_t, action_t>::op_system;
    using SearchEngine<state_t, action_t>::goal_test;
    using SearchEngine<state_t, action_t>::incumbent_plan;
    using SearchEngine<state_t, action_t>::incumbent_cost;

    using SearchEngine<state_t, action_t>::hitSuccFuncLimit;
    using SearchEngine<state_t, action_t>::hitGoalTestLimit;
    using SearchEngine<state_t, action_t>::hitHCompLimit;
    using SearchEngine<state_t, action_t>::increaseActionGenCount;
    using SearchEngine<state_t, action_t>::incrementGoalTestCount;
    using SearchEngine<state_t, action_t>::incrementHCompCount;
    using SearchEngine<state_t, action_t>::incrementStateGenCount;
    using SearchEngine<state_t, action_t>::incrementSuccFuccCalls;

public:
    BestFirstSearch();
    virtual ~BestFirstSearch();

    int reopen;

    /**
     * Sets the hash function used by the search.
     *
     * @param hash A pointer to the hash function.
     */
    void setHashFunction(const StateHashFunction<state_t> *hash);

    /**
     * Sets the heuristic function used by the search.
     *
     * @param heur A pointer to the heuristic function to use.
     */
    void setHeuristic(Heuristic<state_t> *heur);

    void setReopen(int is_reopen);

    void setTieBreaker(int tiebreaker);

    // void setTieBreaker(NodeID<state_t, action_t, StateHash, double, double, double, NodeID> *tiebreaker);

    /**
     * Returns the number of unique goal tests.
     *
     * @return The number of unique goal tests.
     */
    uint64_t getUniqueGoalTests() const;

protected:
    // Overloaded functions
    virtual SearchTermType searchForPlan(const state_t &init_state);
    virtual void resetStatistics();
    virtual bool isConfigured() const;
    virtual void resetEngine();

    /**
     * Calculates the evaluation of the given node.
     *
     * @param state The state to evaluate.
     * @param g_cost The g-cost of the current node.
     * @param h_cost The h-cost of the current node.
     * @return The evaluation of the current node.
     */
    virtual double nodeEval(const state_t &state, double g_cost, double h_cost) = 0;

    /**
     * Expands a single node and returns the result of the expansion (regarding if a solution as found or not).
     *
     * @return The result of the node expansion.
     */
    virtual BfsExpansionResult nodeExpansion();

    /**
     * Extracts the path that ends at the node for the given id and stores it as the incumbent plan.
     *
     * @param path_end_id The node at the end of the path.
     */
    void extractSolutionPath(NodeID path_end_id);

    Heuristic<state_t> *heur_func; ///< The heuristic function.
    const StateHashFunction<state_t> *hash_func; ///< The hash function.
    int tiebreaker;

    OpenClosedList<state_t, action_t> open_closed_list; ///< The open and closed list.

    uint64_t unique_goal_tests; ///< The number of unique goal tests performed.

    std::vector<action_t> app_actions; ///< A vector to store the set of applicable actions.
};

template<class state_t, class action_t>
BestFirstSearch<state_t, action_t>::BestFirstSearch()
        : heur_func(0), hash_func(0), unique_goal_tests(0)
{
}

template<class state_t, class action_t>
BestFirstSearch<state_t, action_t>::~BestFirstSearch()
{
}

template<class state_t, class action_t>
inline void BestFirstSearch<state_t, action_t>::setHashFunction(const StateHashFunction<state_t>* hash)
{
    hash_func = hash;
}

template<class state_t, class action_t>
inline void BestFirstSearch<state_t, action_t>::setHeuristic(Heuristic<state_t>* heur)
{
    heur_func = heur;
}

template<class state_t, class action_t>
inline void BestFirstSearch<state_t, action_t>::setReopen(int is_reopen)
{
    reopen = is_reopen;
}

template<class state_t, class action_t>
inline void BestFirstSearch<state_t, action_t>::setTieBreaker(int tiebreaker_type)
{
    tiebreaker = tiebreaker_type;
}

template<class state_t, class action_t>
SearchTermType BestFirstSearch<state_t, action_t>::searchForPlan(const state_t& init_state)
{
    BfsExpansionResult exp_result = BfsExpansionResult::no_solution;

    heur_func->prepareToCompute();
    double init_h = heur_func->getHValue(init_state);

    incrementHCompCount();

    double init_eval = nodeEval(init_state, 0.0, init_h);

    open_closed_list.addInitialNodeToOpen(init_state, op_system->getDummyAction(), hash_func->getStateHash(init_state),
            init_h, init_eval);

    while(exp_result == BfsExpansionResult::no_solution)
        exp_result = nodeExpansion();

    if(exp_result == BfsExpansionResult::res_limit)
        return SearchTermType::res_limit;
    return SearchTermType::completed;
}

template<class state_t, class action_t>
void BestFirstSearch<state_t, action_t>::resetStatistics()
{
    SearchEngine<state_t, action_t>::resetStatistics();
    unique_goal_tests = 0;
}

template<class state_t, class action_t>
BfsExpansionResult BestFirstSearch<state_t, action_t>::nodeExpansion()
{
    if(open_closed_list.isOpenEmpty())
        return BfsExpansionResult::empty_open;

    NodeID to_expand_id;
    if (tiebreaker == 0) {
        to_expand_id = open_closed_list.getBestNodeAndClose();
    } else if (tiebreaker == 1) {
        to_expand_id = open_closed_list.getLowGAndClose();
    } else if (tiebreaker == 2) {
        to_expand_id = open_closed_list.getHighGAndClose();
    }

    // NodeID to_expand_id = open_closed_list.getHighGAndClose();

    BFSNode<state_t, action_t> to_expand_node = open_closed_list.getNode(to_expand_id);
    //std::cout << "Expanding " << to_expand_id << " - " << to_expand_node.state << "," << to_expand_node.gen_action << std::endl;

    if(hitGoalTestLimit())
        return BfsExpansionResult::res_limit;

    incrementGoalTestCount();
    if(!to_expand_node.reopened)
        unique_goal_tests++; // change this to a function

    if(goal_test->isGoal(to_expand_node.state)) { // checks for goal
        extractSolutionPath(to_expand_id);
        return BfsExpansionResult::goal_found;
    }

    double parent_g = to_expand_node.g_cost;

    if(hitSuccFuncLimit())
        return BfsExpansionResult::res_limit;

    incrementSuccFuccCalls();

    app_actions.clear();
    op_system->getActions(to_expand_node.state, app_actions);
    increaseActionGenCount(app_actions.size());

    for(unsigned i = 0; i < app_actions.size(); i++) {

        double edge_cost = op_system->getActionCost(to_expand_node.state, app_actions[i]);
        double child_g = parent_g + edge_cost;

        state_t child_state = to_expand_node.state;
        op_system->applyAction(child_state, app_actions[i]);
        incrementStateGenCount();

        StateHash child_hash = hash_func->getStateHash(child_state);
        NodeID child_id;
        StateLocation child_loc = open_closed_list.getStateLocation(child_state, child_hash, child_id);

        if(child_loc == StateLocation::open || child_loc == StateLocation::closed) {
            if(fp_less(child_g, open_closed_list.getNode(child_id).g_cost)) {
                open_closed_list.getNode(child_id).g_cost = child_g;
                open_closed_list.getNode(child_id).eval = nodeEval(child_state, child_g,
                        open_closed_list.getNode(child_id).h_value);
                open_closed_list.getNode(child_id).parent_id = to_expand_id;
                open_closed_list.getNode(child_id).gen_action = app_actions[i];

                if(child_loc == StateLocation::open)
                    open_closed_list.openNodeEvalChanged(child_id);
                else
                    if (reopen == 1) {
                        open_closed_list.reopenNode(child_id);

                    }
                    
                
            }
        } else {

            if(hitHCompLimit())
                return BfsExpansionResult::res_limit;

            incrementHCompCount();
            heur_func->prepareToCompute();
            double child_h = heur_func->getHValue(child_state);
            double child_eval = nodeEval(child_state, child_g, child_h);

            //std::cout << "New Child " << child_state << " eval " << child_eval << std::endl;
            open_closed_list.addNewNodeToOpen(child_state, app_actions[i], child_hash, child_g, child_h, child_eval,
                    to_expand_id);
        }
    }

    return BfsExpansionResult::no_solution;
}

template<class state_t, class action_t>
bool BestFirstSearch<state_t, action_t>::isConfigured() const
{
    if(!heur_func || !hash_func)
        return false;
    return SearchEngine<state_t, action_t>::isConfigured();
}

template<class state_t, class action_t>
inline uint64_t BestFirstSearch<state_t, action_t>::getUniqueGoalTests() const
{
    return unique_goal_tests;
}

template<class state_t, class action_t>
void BestFirstSearch<state_t, action_t>::resetEngine()
{
    open_closed_list.clear();
    SearchEngine<state_t, action_t>::resetEngine();
}

template<class state_t, class action_t>
void BestFirstSearch<state_t, action_t>::extractSolutionPath(NodeID path_end_id)
{
    incumbent_cost = 0.0;
    incumbent_plan.clear();

    NodeID id = path_end_id;
    while(open_closed_list.getNode(id).gen_action != op_system->getDummyAction()) {
        incumbent_plan.push_back(open_closed_list.getNode(id).gen_action);
        id = open_closed_list.getNode(id).parent_id;
        incumbent_cost += op_system->getActionCost(open_closed_list.getNode(id).state, incumbent_plan.back());
    }

    assert(!fp_greater(incumbent_cost, open_closed_list.getNode(path_end_id).g_cost));
}

#endif /* BEST_FIRST_SEARCH_H_ */
