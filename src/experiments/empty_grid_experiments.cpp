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
#include "../utils/string_utils.h"

using namespace std;

int main(int argc, char **argv)
{

    // default 0, low g 1, high g 2
    int tieBreaker = 0;
    if (argc > 2) {
        tieBreaker = atoi(argv[2]);
    }

    cout << argc << endl;
    cout << tieBreaker << endl;

    AStar<MapLocation, MapDir> a_star;

    MapPathfindingTransitions map_ops;
    a_star.setTransitionSystem(&map_ops);

    SingleGoalTest<MapLocation> goal_test(MapLocation(0, 0));
    a_star.setGoalTest(&goal_test);

    MapLocHashFunction map_hash;
    a_star.setHashFunction(&map_hash);

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

    starts.clear();
    goals.clear();
    read_in_pathfinding_probs("../src/domains/map_pathfinding/map_files/empty_grid.probs", starts, goals);
    assert(starts.size() == goals.size());

    for(unsigned i = 0; i < starts.size(); i++) {
        goal_test.setGoal(goals[i]);
        manhattan.setGoal(goals[i]);

        a_star.getPlan(starts[i], solution);

        // prints stats (using goal test count as measure of number of expansions)
        cout << a_star.getLastPlanCost() << "\t" << a_star.getGoalTestCount() << "\t" << a_star.getUniqueGoalTests()
                << endl;
    }

    return 0;
}
