/**
 * The file you can use to do your experiments on an empty grid pathfinding map.

 * @file empty_grid_experiments.cpp
 */

#include <stdio.h>
#include <vector>
#include <cstdint>

#include "../domains/map_pathfinding/map_pathfinding_transitions.h"
#include "../domains/map_pathfinding/map_manhattan_distance.h"
#include "../domains/map_pathfinding/map_loc_hash_function.h"
#include "../generic_defs/non_goal_heuristic.h"
#include "../generic_defs/single_goal_test.h"
#include "../algorithms/best_first_search/a_star.h"
#include "../utils/string_utils.h"

using namespace std;

int main(int argc, char **argv)
{
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

    map_ops.set4Connected();
    map_hash.setMapDimensions(map_ops);

    a_star.setHeuristic(&manhattan);

    return 0;
}
