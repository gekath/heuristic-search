/*
 * search_engine.h
 *
 * LICENSE HERE
 *
 *  Created on: 2016-08-24
 *      Author: Rick Valenzano
 */

#ifndef SEARCHENGINE_H_
#define SEARCHENGINE_H_

#include <vector>
#include <cstdint>

#include "goal_test_function.h"
#include "transition_system.h"

//TODO Are these enums really the best way to do this? Have classes like an Exception?
//TODO Add time and time limits
//TODO Add debug modes for state-checking?

/**
 * Defines the possible statuses for a search engine.
 *
 * Below is the meaning of each of the values:
 *
 * not_ready - the engine is not ready for search.\n
 * ready - the engine is ready for search.\n
 * active - the engine is currently searching, no solution has been found yet.\n
 * terminated - the engine has finished searching.\n
 *
 * @class SearchStatus
 */
enum class SearchStatus
{
    not_ready, ready, active, terminated
};

/**
 * Defines the possible termination types for a search engine.
 *
 * Below is the meaning of each of the values:
 *
 * completed - the search has terminated without hitting resource limits.\n
 * engine_not_ready - the search terminated because it was initiated before the engive was configured properly.\n
 * res_limit - the search terminated due to some resource limit having been reached.\n
 *
 * @class SearchTermination
 */
enum class SearchTermType
{
    completed, engine_not_ready, res_limit
};

/**
 * Defines a template for a search algorithm.
 *
 * @class SearchEngine
 */
template<class state_t, class action_t>
class SearchEngine
{
public:
    /**
     * Constructor for abstract search engine. Abstract constructor is empty aside from default initializations.
     */
    SearchEngine();

    /**
     * Destructor for abstract search engine. Abstract constructor is empty.
     */
    virtual ~SearchEngine();

    /**
     * Sets the goal test function to be used by the engine.
     *
     * Resets the engine whenever this is called.
     *
     * @param g The new goal test function.
     */
    void setGoalTest(const GoalTestFunction<state_t> *g);

    /**
     * Sets the transition system to be used by the engine.
     *
     * Resets the engine whenever this is called.
     *
     * @param o The new transition function.
     */
    void setTransitionSystem(const TransitionSystem<state_t, action_t> *o);

    /**
     * Gets the current status of the search engine.
     *
     * @return Status of the engine.
     */
    SearchStatus getStatus() const;

    /**
     * Given an initial state, finds a plan if one exists.
     *
     * If no plan exists or was found before a specified resource limit, the returned plan is set to be empty and
     * the reason for the termination is returned. The function also checks to make sure that the engine is ready
     * to search for a plan prior to search beginning.
     *
     * @param init_state The initial state to find a solution from.
     * @param sol_plan A solution plan, if one exists.
     * @return The reason for termination.
     */
    SearchTermType getPlan(const state_t &init_state, std::vector<action_t> &sol_plan);

    /**
     * Returns if an incumbent solution has been found and is stored.
     *
     * @return If an incumbent solution is stored.
     */
    bool foundSolution() const;

    /**
     * Returns the last plan found during the last search run.
     *
     * @return The last plan found.
     */
    std::vector<action_t> getLastPlan();

    /**
     * Returns the cost of the last plan found during the last search run.
     *
     * Negative values should be interpreted as there being no plan.
     *
     * @return The cost of the last plan found.
     */
    double getLastPlanCost();

    /**
     * Sets a limit on the number of states that can be stored at any time.
     *
     * The default value of 0 means no limit.
     *
     * @param limit The limit on state storage.
     */
    void setStorageLimit(uint64_t limit);

    /**
     * Returns true if the limit on number of states being stored has been set and reached.
     *
     * @return If the limit on state storage has been hit.
     */
    bool hitStorageLimit();

    /**
     * Sets a limit on the number of heuristic computations that can be performed.
     *
     * The default value of 0 means no limit.
     *
     * @param limit The limit on the number of heuristic computations performed.
     */
    void setHComputeLimit(uint64_t limit);

    /**
     * Returns true if the limit on the number of heuristic computations allowed has been set and reached.
     *
     * @return If the limit on the heuristic computations has been hit.
     */
    bool hitHCompLimit();

    /**
     * Sets a limit on the number of calls to the successor (or applicable action) function.
     *
     * The default value of 0 means no limit.
     *
     * @param limit The limit on the number of calls to the successor or applicable action function.
     */
    void setSuccCallLimit(uint64_t limit);

    /**
     * Returns true if the limit on the number of successor function calls has been set and reached.
     *
     * @return If the limit on successor function calls has been hit.
     */
    bool hitSuccFuncLimit();

    /**
     * Sets a limit on the number of goal tests.
     *
     * The default value of 0 means no limit.
     *
     * @param limit The limit on the number of goal tests.
     */
    void setGoalTestLimit(uint64_t limit);

    /**
     * Returns true if the limit on the number of goal tests has been hit.
     *
     * @return If the limit on goal tests has been hit.
     */
    bool hitGoalTestLimit();

    /**
     * Returns the number of states stored during the last search.
     *
     * @return The number of states stored.
     */
    uint64_t getStorageCount() const;

    /**
     * Returns the number of heuristic computations during the last search.
     *
     * @return The number of heuristic computations.
     */
    uint64_t getHCompCount() const;

    /**
     * Gets the number of calls to the successor or applicable actions function during the last search.
     *
     * @return The number of calls to the successor or applicable actions function.
     */
    uint64_t getSuccFuncCount() const;

    /**
     * Gets the number of states generated during the last search.
     *
     * @return The number of state generations.
     */
    uint64_t getStateGenCount() const;

    /**
     * Gets the number of actions generated during the last search.
     *
     * @return The number of action generations.
     */
    uint64_t getActionGenCount() const;

    /**
     * Gets the number of goal tests performed during the last search.
     *
     * @return The number of goal tests performed.
     */
    uint64_t getGoalTestCount() const;

    /**
     * Resets the statistics counters.
     */
    virtual void resetStatistics();

    /**
     * Resets the search engine by resetting the statistics and checking if the engine is ready for the next search.
     */
    virtual void resetEngine();

    /**
     * Checks if the engine has all elements necessary for search.
     *
     * @return If the engine has all elements it needs for search.
     */
    virtual bool isConfigured() const;

protected:

    /**
     * Searches for a plan. Assumes search engine is ready and properly configured.
     *
     * @param init_state The initial state to search from.
     * @return The reason for the termination of the algorithm.
     */
    virtual SearchTermType searchForPlan(const state_t &init_state) = 0;

    /**
     * Increments the count of the number of goal tests performed.
     */
    virtual void incrementGoalTestCount();

    /**
     * Increments the count of the number of heuristic computations performed.
     */
    virtual void incrementHCompCount();

    /**
     * Increments the count of the number of successor function calls performed.
     */
    virtual void incrementSuccFuccCalls();

    /**
     * Increments the count of the number of state generations performed.
     */
    virtual void incrementStateGenCount();

    /**
     * Increases the count of the number of actions generated.
     *
     * @param new_gens The number of new action generations.
     */
    virtual void increaseActionGenCount(double new_gens);

    const GoalTestFunction<state_t> *goal_test; ///< The goal test function.
    const TransitionSystem<state_t, action_t> *op_system; ///< The transition system.

    SearchStatus alg_status; ///< The current search status.

    bool have_incumbent; ///< If an incumbent solution is stored or not.
    std::vector<action_t> incumbent_plan; ///< The last solution found.
    double incumbent_cost; ///< The cost of the last solution.

    uint64_t storage_limit; ///< The limit on how many states are stored. 0 for no limit.
    uint64_t storage_count; ///< The number of states currently being stored.

    uint64_t h_comp_limit; ///< The limit on how many heuristic computations can be performed. 0 for no limit.
    uint64_t h_comp_count; ///< The number of heuristic computations during the current search.

    uint64_t succ_func_limit; ///< The limit on how many successor function calls can be performed. 0 for no limit.
    uint64_t succ_func_count; ///< The number of heuristic computations made during the current search.

    uint64_t goal_test_limit; ///< The limit on the number of goal tests. 0 for no limit
    uint64_t goal_test_count; ///< The number of goal tests performed during the current search.

    uint64_t state_gen_count; ///< The number of state generations during the current search.
    uint64_t action_gen_count; ///< The number of state generations during the current search.
};

template<class state_t, class action_t>
SearchEngine<state_t, action_t>::SearchEngine()
        : goal_test(0), op_system(0), alg_status(SearchStatus::not_ready), have_incumbent(false), incumbent_cost(0.0),
                storage_limit(0), storage_count(0), h_comp_limit(0), h_comp_count(0), succ_func_limit(0),
                succ_func_count(0), goal_test_limit(0), goal_test_count(0), state_gen_count(0), action_gen_count(0)
{
}

template<class state_t, class action_t>
SearchEngine<state_t, action_t>::~SearchEngine()
{
}

template<class state_t, class action_t>
void SearchEngine<state_t, action_t>::setGoalTest(const GoalTestFunction<state_t>* g)
{
    goal_test = g;

    resetEngine();
}

template<class state_t, class action_t>
void SearchEngine<state_t, action_t>::setTransitionSystem(const TransitionSystem<state_t, action_t>* o)
{
    op_system = o;

    resetEngine();
}

template<class state_t, class action_t>
inline SearchStatus SearchEngine<state_t, action_t>::getStatus() const
{
    return alg_status;
}

template<class state_t, class action_t>
SearchTermType SearchEngine<state_t, action_t>::getPlan(const state_t& init_state, std::vector<action_t>& sol_plan)
{
    resetEngine();

    if(alg_status == SearchStatus::not_ready)
        return SearchTermType::engine_not_ready;

    alg_status = SearchStatus::active;
    SearchTermType term = searchForPlan(init_state);

    alg_status = SearchStatus::terminated;

    sol_plan.clear();
    if(incumbent_plan.size() > 0) {
        for(unsigned i = 0; i < incumbent_plan.size(); i++) {
            sol_plan.push_back(incumbent_plan[i]);
        }
    }

    return term;
}

template<class state_t, class action_t>
inline void SearchEngine<state_t, action_t>::setStorageLimit(uint64_t limit)
{
    storage_limit = limit;
}

template<class state_t, class action_t>
inline void SearchEngine<state_t, action_t>::setHComputeLimit(uint64_t limit)
{
    h_comp_limit = limit;
}

template<class state_t, class action_t>
inline void SearchEngine<state_t, action_t>::setSuccCallLimit(uint64_t limit)
{
    succ_func_limit = limit;
}

template<class state_t, class action_t>
inline uint64_t SearchEngine<state_t, action_t>::getStorageCount() const
{
    return storage_count;
}

template<class state_t, class action_t>
inline uint64_t SearchEngine<state_t, action_t>::getHCompCount() const
{
    return h_comp_count;
}

template<class state_t, class action_t>
inline uint64_t SearchEngine<state_t, action_t>::getSuccFuncCount() const
{
    return succ_func_count;
}

template<class state_t, class action_t>
inline uint64_t SearchEngine<state_t, action_t>::getStateGenCount() const
{
    return state_gen_count;
}

template<class state_t, class action_t>
inline uint64_t SearchEngine<state_t, action_t>::getActionGenCount() const
{
    return action_gen_count;
}

template<class state_t, class action_t>
void SearchEngine<state_t, action_t>::resetStatistics()
{
    storage_count = 0;
    h_comp_count = 0;
    succ_func_count = 0;
    state_gen_count = 0;
    action_gen_count = 0;
    goal_test_count = 0;
}

template<class state_t, class action_t>
void SearchEngine<state_t, action_t>::resetEngine()
{
    resetStatistics();

    have_incumbent = false;
    incumbent_cost = -1.0;
    incumbent_plan.clear();

    alg_status = SearchStatus::not_ready;
    if(isConfigured())
        alg_status = SearchStatus::ready;
}

template<class state_t, class action_t>
inline uint64_t SearchEngine<state_t, action_t>::getGoalTestCount() const
{
    return goal_test_count;
}

template<class state_t, class action_t>
bool SearchEngine<state_t, action_t>::isConfigured() const
{
    if(!goal_test || !op_system) {
        return false;
    }
    return true;
}

template<class state_t, class action_t>
bool SearchEngine<state_t, action_t>::hitStorageLimit()
{
    if(storage_limit > 0 && storage_count >= storage_limit)
        return true;
    return false;
}

template<class state_t, class action_t>
bool SearchEngine<state_t, action_t>::hitHCompLimit()
{
    if(h_comp_limit > 0 && h_comp_count >= h_comp_limit)
        return true;
    return false;
}

template<class state_t, class action_t>
inline std::vector<action_t> SearchEngine<state_t, action_t>::getLastPlan()
{
    return incumbent_plan;
}

template<class state_t, class action_t>
inline double SearchEngine<state_t, action_t>::getLastPlanCost()
{
    return incumbent_cost;
}

template<class state_t, class action_t>
inline bool SearchEngine<state_t, action_t>::foundSolution() const
{
    return have_incumbent;
}

template<class state_t, class action_t>
bool SearchEngine<state_t, action_t>::hitSuccFuncLimit()
{
    if(succ_func_limit > 0 && succ_func_count >= succ_func_limit)
        return true;
    return false;
}

template<class state_t, class action_t>
inline void SearchEngine<state_t, action_t>::setGoalTestLimit(uint64_t limit)
{
    goal_test_limit = limit;
}

template<class state_t, class action_t>
bool SearchEngine<state_t, action_t>::hitGoalTestLimit()
{
    if(goal_test_limit > 0 && goal_test_count >= goal_test_limit)
        return true;
    return false;
}

template<class state_t, class action_t>
void SearchEngine<state_t, action_t>::increaseActionGenCount(double new_gens)
{
    action_gen_count += new_gens;
}

template<class state_t, class action_t>
void SearchEngine<state_t, action_t>::incrementStateGenCount()
{
    state_gen_count++;
}

template<class state_t, class action_t>
void SearchEngine<state_t, action_t>::incrementGoalTestCount()
{
    goal_test_count++;
}

template<class state_t, class action_t>
void SearchEngine<state_t, action_t>::incrementSuccFuccCalls()
{
    succ_func_count++;
}

template<class state_t, class action_t>
void SearchEngine<state_t, action_t>::incrementHCompCount()
{
    h_comp_count++;
}

#endif /* SEARCHENGINE_H_ */
