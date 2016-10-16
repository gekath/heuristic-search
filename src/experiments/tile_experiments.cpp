/**
 * The file you can use to do your experiments in the sliding tile puzzle.

 * @file tile_experiments.cpp
 */

#include <stdio.h>
#include <iostream>
#include <vector>

#include "../domains/tile_puzzle/tile_puzzle_transitions.h"
#include "../domains/tile_puzzle/tile_manhattan_distance.h"
#include "../generic_defs/single_goal_test.h"
#include "../algorithms/best_first_search/weighted_a.h"
#include "../algorithms/best_first_search/gbfs.h"
#include "../generic_defs/permutation_hash_function.h"
#include "../utils/combinatorics.h"

#include <cstdint>

using namespace std;

int main(int argc, char **argv)
{

     // default 0, low g 1, high g 2
    int tiebreaker = 0;

    if (argc > 1) {
        tiebreaker = atoi(argv[1]);
    } 

    WeightedAStar<TilePuzzlestate, BlankSlide> a_1;
    WeightedAStar<TilePuzzlestate, BlankSlide> a_2;
    WeightedAStar<TilePuzzlestate, BlankSlide> a_5;
    WeightedAStar<TilePuzzlestate, BlankSlide> a_10;
    WeightedAStar<TilePuzzlestate, BlankSlide> a_100;
    GBFS<TilePuzzlestate, BlankSlide> gbfs;

    TilePuzzleTransitions tile_ops(3, 4);
    a_1.setTransitionSystem(&tile_ops);
    a_2.setTransitionSystem(&tile_ops);
    a_5.setTransitionSystem(&tile_ops);
    a_10.setTransitionSystem(&tile_ops);
    a_100.setTransitionSystem(&tile_ops);
    gbfs.setTransitionSystem(&tile_ops);

    TilePuzzleState goal_state(3, 4);

    SingleGoalTest<TilePuzzleState> goal_test(goal_state);
    a_1.setGoalTest(&goal_test);
    a_2.setGoalTest(&goal_test);
    a_5.setGoalTest(&goal_test);
    a_10.setGoalTest(&goal_test);
    a_100.setGoalTest(&goal_test);
    gbfs.setGoalTest(&goal_test);

    PermutationHashFunction<TilePuzzleState> tile_hash;
    a_1.setHashFunction(&tile_hash);
    a_2.setHashFunction(&tile_hash);
    a_5.setHashFunction(&tile_hash);
    a_10.setHashFunction(&tile_hash);
    a_100.setHashFunction(&tile_hash);
    gbfs.setHashFunction(&tile_hash);


    TileManhattanDistance manhattan(goal_state, tile_ops);
    a_1.setHeuristic(&manhattan);
    a_2.setHeuristic(&manhattan);
    a_5.setHeuristic(&manhattan);
    a_10.setHeuristic(&manhattan);
    a_100.setHeuristic(&manhattan);
    gbfs.setHeuristic(&manhattan);

    a_1.setTieBreaker(tiebreaker);
    a_2.setTieBreaker(tiebreaker);
    a_5.setTieBreaker(tiebreaker);
    a_10.setTieBreaker(tiebreaker);
    a_100.setTieBreaker(tiebreaker);
    gbfs.setTieBreaker(tiebreaker);

    a_1.setWeights(1);
    a_2.setWeights(2);
    a_5.setWeights(5);
    a_10.setWeights(10);
    a_100.setWeights(100);

    vector<BlankSlide> solution;

    vector<vector<unsigned> > starts;

    read_in_permutations("../src/domains/tile_puzzle/tile_files/3x4_puzzle.probs", starts);

    for(unsigned i = 0; i < starts.size(); i++) {
        TilePuzzleState start_state(starts[i], 3, 4);

        a_1.getPlan(start_state, solution);
        a_2.getPlan(start_state, solution);
        a_5.getPlan(start_state, solution);
        a_10.getPlan(start_state, solution);
        a_100.getPlan(start_state, solution);
        gbfs.getPlan(start_state, solution);

        // prints stats (using goal test count as measure of number of expansions)
        
        cout << "Weighted A Star, weight = 1" << endl;
        cout << a_1.getLastPlanCost() << "\t" << a_1.getGoalTestCount() << "\t" << a_1.getUniqueGoalTests()
                << endl;

        cout << "Weighted A Star, weight = 2" << endl;
        cout << a_2.getLastPlanCost() << "\t" << a_2.getGoalTestCount() << "\t" << a_2.getUniqueGoalTests()
                << endl;

        cout << "Weighted A Star, weight = 5" << endl;
        cout << a_5.getLastPlanCost() << "\t" << a_5.getGoalTestCount() << "\t" << a_5.getUniqueGoalTests()
                << endl;

        cout << "Weighted A Star, weight = 10" << endl;
        cout << a_10.getLastPlanCost() << "\t" << a_10.getGoalTestCount() << "\t" << a_10.getUniqueGoalTests()
                << endl;

        cout << "Weighted A Star, weight = 100" << endl;
        cout << a_100.getLastPlanCost() << "\t" << a_100.getGoalTestCount() << "\t" << a_100.getUniqueGoalTests()
                << endl;

        cout << "GBFS" << endl;
        cout << gbfs.getLastPlanCost() << "\t" << gbfs.getGoalTestCount() << "\t" << gbfs.getUniqueGoalTests()
                << endl;

    }

    return 0;
}
