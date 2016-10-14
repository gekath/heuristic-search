/**
 * The file you can use to do your experiments on an empty grid pathfinding map.

 * @file empty_grid_experiments.cpp
 */

#include <stdio.h>
#include <vector>
#include <cstdint>
#include <stdlib.h>

#include "../domains/map_pathfinding/map_pathfinding_transitions.h"
#include "../domains/map_pathfinding/map_manhattan_distance.h"
#include "../domains/map_pathfinding/map_loc_hash_function.h"
#include "../domains/map_pathfinding/map_utils.h"
#include "../generic_defs/non_goal_heuristic.h"
#include "../generic_defs/single_goal_test.h"
#include "../algorithms/best_first_search/a_star.h"
#include "../algorithms/best_first_search/weighted_a.h"
#include "../utils/string_utils.h"


using namespace std;

int main(int argc, char **argv)
{

    // default 0, low g 1, high g 2
    int tieBreaker = 0;
    if (argc > 1) {
        tieBreaker = atoi(argv[1]);
    }

    // cout << argc << endl;
    // cout << tieBreaker << endl;

    int weights = 2;

    AStar<MapLocation, MapDir> a_star;
    WeightedAStar<MapLocation, MapDir> weighted_a;

    MapPathfindingTransitions map_ops;
    a_star.setTransitionSystem(&map_ops);
    weighted_a.setTransitionSystem(&map_ops);

    SingleGoalTest<MapLocation> goal_test(MapLocation(0, 0));
    a_star.setGoalTest(&goal_test);
    weighted_a.setGoalTest(&goal_test);

    MapLocHashFunction map_hash;
    a_star.setHashFunction(&map_hash);
    weighted_a.setHashFunction(&map_hash);

    MapManhattanDistance manhattan;

    vector<MapLocation> starts;
    vector<MapLocation> goals;
    vector<MapDir> solution;

    // Empty Grid Tests
    if(!map_ops.loadMap("../src/domains/map_pathfinding/map_files/empty_grid.map")) {
        cerr << "Need to define empty_grid.map" << endl;
        return 1;
    }

    cout << "Loaded empty grid" << endl;

    map_ops.set4Connected();
    map_hash.setMapDimensions(map_ops);

    a_star.setHeuristic(&manhattan);
    // a_star.setTieBreaker(&default);
    a_star.setTieBreaker(tieBreaker);

    weighted_a.setHeuristic(&manhattan);
    weighted_a.setTieBreaker(tieBreaker);
    weighted_a.setWeights(weights);

    starts.clear();
    goals.clear();
    read_in_pathfinding_probs("../src/domains/map_pathfinding/map_files/empty_grid.probs", starts, goals);
    assert(starts.size() == goals.size());

    for(unsigned i = 0; i < starts.size(); i++) {
        goal_test.setGoal(goals[i]);
        manhattan.setGoal(goals[i]);

        a_star.getPlan(starts[i], solution);

        // prints stats (using goal test count as measure of number of expansions)
        // cout << a_star.getLastPlanCost() << "\t" << a_star.getGoalTestCount() << "\t" << a_star.getUniqueGoalTests()
                // << endl;

        cout << weighted_a.getLastPlanCost() << "\t" << weighted_a.getGoalTestCount() << "\t" << weighted_a.getUniqueGoalTests()
                << endl;

    }

    return 0;
}
