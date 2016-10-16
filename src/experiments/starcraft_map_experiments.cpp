/**
 * The file you can use to do your experiments for pathfinding in the given Starcraft map.

 * @file starcraft_map_experiments.cpp
 */

#include <stdio.h>
#include <vector>
#include <cstdint>
#include <algorithm>

#include "../domains/map_pathfinding/map_pathfinding_transitions.h"
#include "../domains/map_pathfinding/map_manhattan_distance.h"
#include "../domains/map_pathfinding/map_loc_hash_function.h"
#include "../domains/map_pathfinding/map_utils.h"
#include "../generic_defs/non_goal_heuristic.h"
#include "../generic_defs/single_goal_test.h"
#include "../algorithms/best_first_search/a_star.h"
#include "../algorithms/best_first_search/weighted_a.h"
#include "../algorithms/best_first_search/gbfs.h"
#include "../utils/string_utils.h"
#include "../utils/vector_ops.h"

using namespace std;

double compute_median(vector<int> scores);
double compute_average(vector<int> scores);

double compute_median(vector<int> scores) {

    double median;
    size_t size = scores.size();
    sort(scores.begin(), scores.end());

    if (size % 2 == 0) {
        median = (scores[size / 2 - 1] + scores[size / 2]) / 2;
    } else {
        median = scores[size / 2];
    }

    return median;

}

double compute_average(vector<int> scores) {

    int sum = 0 ;
    unsigned int i;
    size_t size = scores.size();

    for (i = 0; i < size; i ++) {
        sum = sum + scores[i];
    }

    return sum / size;

}


int main(int argc, char **argv)
{
    AStar<MapLocation, MapDir> a_star;
    AStar<MapLocation, MapDir> a_low_g;
    AStar<MapLocation, MapDir> a_high_g;

    // WeightedAStar<MapLocation, MapDir> weighted_a;
    // GBFS<MapLocation, MapDir> gbfs;

    MapPathfindingTransitions map_ops;
    a_star.setTransitionSystem(&map_ops);
    a_low_g.setTransitionSystem(&map_ops);
    a_high_g.setTransitionSystem(&map_ops);
    
    // weighted_a.setTransitionSystem(&map_ops);
    // gbfs.setTransitionSystem(&map_ops);

    SingleGoalTest<MapLocation> goal_test(MapLocation(0, 0));
    a_star.setGoalTest(&goal_test);
    a_low_g.setGoalTest(&goal_test);
    a_high_g.setGoalTest(&goal_test);

    // weighted_a.setGoalTest(&goal_test);
    // gbfs.setGoalTest(&goal_test);

    MapLocHashFunction map_hash;
    a_star.setHashFunction(&map_hash);
    a_low_g.setHashFunction(&map_hash);
    a_high_g.setHashFunction(&map_hash);

    // weighted_a.setHashFunction(&map_hash);
    // gbfs.setHashFunction(&map_hash);

    MapManhattanDistance manhattan;

    vector<MapLocation> starts;
    vector<MapLocation> goals;
    vector<MapDir> solution;

    // Starcraft map tests
    map_ops.loadMap("../src/domains/map_pathfinding/map_files/starcraft_bgh.map");
    map_ops.set4Connected();
    map_hash.setMapDimensions(map_ops);

    a_star.setHeuristic(&manhattan);
    a_low_g.setHeuristic(&manhattan);
    a_high_g.setHeuristic(&manhattan);


    a_star.setTieBreaker(0);
    a_low_g.setTieBreaker(1);
    a_high_g.setTieBreaker(2);

    // weighted_a.setHeuristic(&manhattan);
    // gbfs.setHeuristic(&manhattan);

    starts.clear();
    goals.clear();
    read_in_pathfinding_probs("../src/domains/map_pathfinding/map_files/starcraft_bgh.probs", starts, goals);
    assert(starts.size() == goals.size());

    vector<int> default_nodes(starts.size());

    for(unsigned i = 0; i < starts.size(); i++) {
        goal_test.setGoal(goals[i]);
        manhattan.setGoal(goals[i]);

        a_star.getPlan(starts[i], solution);

        int goal_test_count = a_star.getGoalTestCount();

        default_nodes[i] = goal_test_count;

        // prints stats (using goal test count as measure of number of expansions)
        cout << a_star.getLastPlanCost() << "\t" << goal_test_count << "\t" << a_star.getUniqueGoalTests()
                << endl;
    }

    vector<int> low_g_nodes(starts.size());

    for(unsigned i = 0; i < starts.size(); i++) {
        goal_test.setGoal(goals[i]);
        manhattan.setGoal(goals[i]);

        a_low_g.getPlan(starts[i], solution);

        int goal_test_count = a_low_g.getGoalTestCount();

        low_g_nodes[i] = goal_test_count;

        // prints stats (using goal test count as measure of number of expansions)
        cout << a_low_g.getLastPlanCost() << "\t" << goal_test_count << "\t" << a_low_g.getUniqueGoalTests()
                << endl;
    }

    vector<int> high_g_nodes(starts.size());

    for(unsigned i = 0; i < starts.size(); i++) {
        goal_test.setGoal(goals[i]);
        manhattan.setGoal(goals[i]);

        a_high_g.getPlan(starts[i], solution);

        int goal_test_count = a_high_g.getGoalTestCount();

        high_g_nodes[i] = goal_test_count;

        // prints stats (using goal test count as measure of number of expansions)
        cout << a_high_g.getLastPlanCost() << "\t" << goal_test_count << "\t" << a_high_g.getUniqueGoalTests()
                << endl;
    }

    double default_median = compute_median(default_nodes);
    double low_g_median = compute_median(low_g_nodes);
    double high_g_median = compute_median(high_g_nodes);


    double default_avg = compute_average(default_nodes);
    double low_g_avg = compute_average(low_g_nodes);
    double high_g_avg = compute_average(high_g_nodes);

    cout << default_median << "\t" << low_g_median << "\t" << high_g_median << endl;
    cout << default_avg << "\t" << low_g_avg << "\t" << high_g_avg << endl;

    unsigned int i;
    int default_count;
    int low_g_count;
    int high_g_count;
    for (i = 0; i < default_nodes.size(); i++) {

        int min_val = min({default_nodes[i], low_g_nodes[i], high_g_nodes[i]});

        if (default_nodes[i] == min_val) {
            default_count++;
        } else if (low_g_nodes[i] == min_val) {
            low_g_count++;
        } else if (high_g_nodes[i] == min_val) {
            high_g_count++;
        }         

       cout << default_nodes[i] << "\t" << low_g_nodes[i] << "\t" << high_g_nodes[i] << endl; 

    }

    cout << "Number of problems: " << default_nodes.size() << endl;
    cout << "High g count: " << high_g_count << endl;
    cout << default_count << "\t" << low_g_count << "\t" <<  high_g_count << endl;



    return 0;
}
