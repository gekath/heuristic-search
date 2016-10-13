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
#include "../algorithms/best_first_search/a_star.h"
#include "../generic_defs/permutation_hash_function.h"
#include "../utils/combinatorics.h"

#include <cstdint>

using namespace std;

int main(int argc, char **argv)
{
    AStar<TilePuzzleState, BlankSlide> a_star;

    TilePuzzleTransitions tile_ops(3, 4);
    a_star.setTransitionSystem(&tile_ops);

    TilePuzzleState goal_state(3, 4);

    SingleGoalTest<TilePuzzleState> goal_test(goal_state);
    a_star.setGoalTest(&goal_test);

    PermutationHashFunction<TilePuzzleState> tile_hash;
    a_star.setHashFunction(&tile_hash);

    TileManhattanDistance manhattan(goal_state, tile_ops);
    a_star.setHeuristic(&manhattan);

    vector<BlankSlide> solution;

    vector<vector<unsigned> > starts;

    read_in_permutations("../src/domains/tile_puzzle/tile_files/3x4_puzzle.probs", starts);


    for(unsigned i = 0; i < starts.size(); i++) {
        TilePuzzleState start_state(starts[i], 3, 4);

        a_star.getPlan(start_state, solution);

        // prints stats (using goal test count as measure of number of expansions)
        cout << a_star.getLastPlanCost() << "\t" << a_star.getGoalTestCount() << "\t" << a_star.getUniqueGoalTests()
                << endl;
    }

    return 0;
}
