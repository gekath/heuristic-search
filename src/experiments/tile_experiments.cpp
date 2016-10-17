/**
 * The file you can use to do your experiments in the sliding tile puzzle.

 * @file tile_experiments.cpp
 */

#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include "../domains/tile_puzzle/tile_puzzle_transitions.h"
#include "../domains/tile_puzzle/tile_manhattan_distance.h"
#include "../generic_defs/single_goal_test.h"
#include "../algorithms/best_first_search/weighted_a.h"
#include "../algorithms/best_first_search/gbfs.h"
#include "../generic_defs/permutation_hash_function.h"
#include "../utils/combinatorics.h"

#include <cstdint>

using namespace std;

double compute_median(vector<int> scores);
double compute_average(vector<int> scores);
vector<double> compute_percent(vector<int> a1_nodes, vector<int> a1_nodes_low, vector<int> a1_nodes_high);

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

vector<double> compute_percent(vector<int> a1_nodes, vector<int> a1_nodes_low, vector<int> a1_nodes_high) {
    
    vector<double> finals(3);

    unsigned int i;
    double default_count = 0;
    double low_g_count = 0;
    double high_g_count = 0;

    for (i = 0; i < a1_nodes.size(); i++) {

        int min_val = min({a1_nodes[i], a1_nodes_low[i], a1_nodes_high[i]});

        if (a1_nodes[i] == min_val) {
            default_count++;
        } else if (a1_nodes_low[i] == min_val) {
            low_g_count++;
        } else if (a1_nodes_high[i] == min_val) {
            high_g_count++;
        }               
        // cout << default_nodes[i] << "\t" << low_g_nodes[i] << "\t" << high_g_nodes[i] << endl; 
    }
    
    finals[0] = default_count;
    finals[1] = low_g_count;
    finals[2] = high_g_count;

    return finals; 
}

int main(int argc, char **argv)
{

     // default 0, low g 1, high g 2
    int tiebreaker = 0;

    if (argc > 1) {
        tiebreaker = atoi(argv[1]);
    } 

    WeightedAStar<TilePuzzleState, BlankSlide> a_1;
    WeightedAStar<TilePuzzleState, BlankSlide> a_2;
    WeightedAStar<TilePuzzleState, BlankSlide> a_5;
    WeightedAStar<TilePuzzleState, BlankSlide> a_10;
    WeightedAStar<TilePuzzleState, BlankSlide> a_100;
    GBFS<TilePuzzleState, BlankSlide> gbfs;

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

    vector<int> a1_nodes(starts.size());
    vector<int> a1_cost(starts.size());
    vector<int> a2_nodes(starts.size());
    vector<int> a2_cost(starts.size());
    vector<int> a5_nodes(starts.size());
    vector<int> a5_cost(starts.size());
    vector<int> a10_nodes(starts.size());
    vector<int> a10_cost(starts.size());
    vector<int> a100_nodes(starts.size());
    vector<int> a100_cost(starts.size());
    vector<int> gbfs_nodes(starts.size());
    vector<int> gbfs_cost(starts.size());

    for(unsigned i = 0; i < starts.size(); i++) {
        TilePuzzleState start_state(starts[i], 3, 4);

        a_1.getPlan(start_state, solution);
        a_2.getPlan(start_state, solution);
        a_5.getPlan(start_state, solution);
        a_10.getPlan(start_state, solution);
        a_100.getPlan(start_state, solution);
        gbfs.getPlan(start_state, solution);

        a1_nodes[i] = a_1.getGoalTestCount();
        a2_nodes[i] = a_2.getGoalTestCount();
        a5_nodes[i] = a_5.getGoalTestCount();
        a10_nodes[i] = a_10.getGoalTestCount();
        a100_nodes[i] = a_100.getGoalTestCount();
        gbfs_nodes[i] = gbfs.getGoalTestCount();

        a1_cost[i] = a_1.getLastPlanCost();
        a2_cost[i] = a_2.getLastPlanCost();
        a5_cost[i] = a_5.getLastPlanCost();
        a10_cost[i] = a_10.getLastPlanCost();
        a100_cost[i] = a_100.getLastPlanCost();
        gbfs_cost[i] = gbfs.getLastPlanCost();

    }

    double a1_median_nodes = compute_median(a1_nodes);
    double a2_median_nodes = compute_median(a2_nodes);
    double a5_median_nodes = compute_median(a5_nodes);
    double a10_median_nodes = compute_median(a10_nodes); 
    double a100_median_nodes = compute_median(a100_nodes);
    double gbfs_median_nodes = compute_median(gbfs_nodes);

    double a1_average_nodes = compute_average(a1_nodes);
    double a2_average_nodes = compute_average(a2_nodes);
    double a5_average_nodes = compute_average(a5_nodes);
    double a10_average_nodes = compute_average(a10_nodes);
    double a100_average_nodes = compute_average(a100_nodes);
    double gbfs_average_nodes = compute_average(gbfs_nodes);

    double a1_median_cost = compute_median(a1_cost);
    double a2_median_cost = compute_median(a2_cost);
    double a5_median_cost = compute_median(a5_cost);
    double a10_median_cost = compute_median(a10_cost);
    double a100_median_cost = compute_median(a100_cost);
    double gbfs_median_cost = compute_median(gbfs_cost);

    double a1_average_cost = compute_average(a1_cost);
    double a2_average_cost = compute_average(a2_cost);
    double a5_average_cost = compute_average(a5_cost);
    double a10_average_cost = compute_average(a10_cost);
    double a100_average_cost = compute_average(a100_cost);
    double gbfs_average_cost = compute_average(gbfs_cost);

    cout << "Median nodes: " << endl;
    cout << a1_median_nodes << "\t" << a2_median_nodes << "\t" << a5_median_nodes << "\t" 
            << a10_median_nodes << "\t" << a100_median_nodes << "\t" << gbfs_median_nodes << endl;

    cout << "Average nodes: " << endl;
    cout << a1_average_nodes << "\t" << a2_average_nodes << "\t" << a5_average_nodes << "\t" 
            << a10_average_nodes << "\t" << a100_average_nodes << "\t" << gbfs_average_nodes << endl;

    cout << "Median Cost: " << endl;
    cout << a1_median_cost << "\t" << a2_median_cost << "\t" << a5_median_cost << "\t" 
            << a10_median_cost << "\t" << a100_median_cost << "\t" << gbfs_median_cost << endl;

    cout << "Average cost: " << endl;
    cout << a1_average_cost << "\t" << a2_average_cost << "\t" << a5_average_cost << "\t" 
            << a10_average_cost << "\t" << a100_average_cost << "\t" << gbfs_average_cost << endl;
            


    return 0;
}
