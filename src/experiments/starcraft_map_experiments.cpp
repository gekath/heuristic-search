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

    int tiebreaker = 0;
    double weight = 1;

    if (argc > 1) {
        tiebreaker = atoi(argv[1]);
    } if (argc > 2) {
        weight = atoi(argv[2]);
    }


    // AStar<MapLocation, MapDir> a_star;
    // AStar<MapLocation, MapDir> a_low_g;
    // AStar<MapLocation, MapDir> a_high_g;

    WeightedAStar<MapLocation, MapDir> a_star;

    MapPathfindingTransitions map_ops;
    a_star.setTransitionSystem(&map_ops);
    // a_low_g.setTransitionSystem(&map_ops);
    // a_high_g.setTransitionSystem(&map_ops);

    SingleGoalTest<MapLocation> goal_test(MapLocation(0, 0));
    a_star.setGoalTest(&goal_test);
    // a_low_g.setGoalTest(&goal_test);
    // a_high_g.setGoalTest(&goal_test);

    MapLocHashFunction map_hash;
    a_star.setHashFunction(&map_hash);
    // a_low_g.setHashFunction(&map_hash);
    // a_high_g.setHashFunction(&map_hash);

    MapManhattanDistance manhattan;

    vector<MapLocation> starts;
    vector<MapLocation> goals;
    vector<MapDir> solution;

    // Starcraft map tests
    map_ops.loadMap("../src/domains/map_pathfinding/map_files/starcraft_bgh.map");
    map_ops.set4Connected();
    map_hash.setMapDimensions(map_ops);

    a_star.setHeuristic(&manhattan);
    // a_low_g.setHeuristic(&manhattan);
    // a_high_g.setHeuristic(&manhattan);

    a_star.setWeights(weight);
    a_star.setTieBreaker(tiebreaker);

    starts.clear();
    goals.clear();
    read_in_pathfinding_probs("../src/domains/map_pathfinding/map_files/starcraft_bgh.probs", starts, goals);
    assert(starts.size() == goals.size());

    vector<int> node_count(starts.size());
    vector<int> unique_count(starts.size());

    for(unsigned i = 0; i < starts.size(); i++) {
        goal_test.setGoal(goals[i]);
        manhattan.setGoal(goals[i]);

        a_star.getPlan(starts[i], solution);

        node_count[i] = a_star.getGoalTestCount();
        unique_count[i] = a_star.getUniqueGoalTests();

    }

    double median_nodes = compute_median(node_count);
    double average_nodes = compute_average(node_count);
    double median_unique = compute_median(unique_count);
    double average_unique = compute_average(unique_count);


    cout << "Weighted A Star, weight = " << weight << endl;
    cout << "Median nodes: " << median_nodes << endl;
    cout << "Average nodes: " << average_nodes << endl;
    cout << "Median unique cost: " << median_unique << endl;
    cout << "Average unique cost: " << average_unique << endl;

    // double default_median = compute_median(default_nodes);
    // double low_g_median = compute_median(low_g_nodes);
    // double high_g_median = compute_median(high_g_nodes);


    // double default_avg = compute_average(default_nodes);
    // double low_g_avg = compute_average(low_g_nodes);
    // double high_g_avg = compute_average(high_g_nodes);

    // cout << default_median << "\t" << low_g_median << "\t" << high_g_median << endl;
    // cout << default_avg << "\t" << low_g_avg << "\t" << high_g_avg << endl;

    // unsigned int i;
    // double default_count = 0;
    // double low_g_count = 0;
    // double high_g_count = 0;
    // for (i = 0; i < default_nodes.size(); i++) {

    //     int min_val = min({default_nodes[i], low_g_nodes[i], high_g_nodes[i]});

    //     if (default_nodes[i] == min_val) {
    //         default_count++;
    //     } else if (low_g_nodes[i] == min_val) {
    //         low_g_count++;
    //     } else if (high_g_nodes[i] == min_val) {
    //         high_g_count++;
    //     }         

    //    cout << default_nodes[i] << "\t" << low_g_nodes[i] << "\t" << high_g_nodes[i] << endl; 

    // }

    // cout << "Number of problems: " << default_nodes.size() << endl;
    // cout << "High g count: " << high_g_count << endl;
    // cout << default_count << "\t" << low_g_count << "\t" <<  high_g_count << endl;



    return 0;
}
