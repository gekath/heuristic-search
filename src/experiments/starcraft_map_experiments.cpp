/**
 * The file you can use to do your experiments for pathfinding in the given Starcraft map.

 * @file starcraft_map_experiments.cpp
 */

#include <stdio.h>
#include <vector>
#include <cstdint>

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
    AStar<MapLocation, MapDir> a_star;
    WeightedAStar<MapLocation, MapDir> weighted_a;
    GBFS<MapLocation, MapDir> gbfs;

    MapPathfindingTransitions map_ops;
    a_star.setTransitionSystem(&map_ops);
    weighted_a.setTransitionSystem(&map_ops);
    gbfs.setTransitionSystem(&map_ops);

    SingleGoalTest<MapLocation> goal_test(MapLocation(0, 0));
    a_star.setGoalTest(&goal_test);
    weighted_a.setGoalTest(&goal_test);
    gbfs.setGoalTest(&goal_test);

    MapLocHashFunction map_hash;
    a_star.setHashFunction(&map_hash);
    weighted_a.setHashFunction(&map_hash);
    gbfs.setHashFunction(&map_hash);

    MapManhattanDistance manhattan;

    vector<MapLocation> starts;
    vector<MapLocation> goals;
    vector<MapDir> solution;

    // Starcraft map tests
    map_ops.loadMap("../src/domains/map_pathfinding/map_files/starcraft_bgh.map");
    map_ops.set4Connected();
    map_hash.setMapDimensions(map_ops);

    a_star.setHeuristic(&manhattan);
    weighted_a.setHeuristic(&manhattan);
    gbfs.setHeuristic(&manhattan);


    starts.clear();
    goals.clear();
    read_in_pathfinding_probs("../src/domains/map_pathfinding/map_files/starcraft_bgh.probs", starts, goals);
    assert(starts.size() == goals.size());

    vector<int> nodes_expanded(starts.size());

    for(unsigned i = 0; i < starts.size(); i++) {
        goal_test.setGoal(goals[i]);
        manhattan.setGoal(goals[i]);

        a_star.getPlan(starts[i], solution);

        goal_test_count = a_star.getGoalTestCount();

        nodes_expanded[i] = goal_test_count;

        // prints stats (using goal test count as measure of number of expansions)
        cout << a_star.getLastPlanCost() << "\t" << goal_test_count << "\t" << a_star.getUniqueGoalTests()
                << endl;
    }

    cout << nodes_expanded.size() << endl;

    return 0;
}
