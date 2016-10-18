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
double compute_variance(vector<int> scores, double mean);
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

double compute_variance(vector<int> scores, double mean) {

    double variance = 0;

    for (unsigned i = 0; i < scores.size(); i++) {

        variance += (scores[i] - mean) * (scores[i] - mean);
    }

    return variance / scores.size();


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
        } if (a1_nodes_low[i] == min_val) {
            low_g_count++;
        } if (a1_nodes_high[i] == min_val) {
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
    double weight = 1;

    if (argc > 1) { 
        tiebreaker = atoi(argv[1]);
    } if (argc > 2) {
        weight = atoi(argv[2]);
    }

    WeightedAStar<TilePuzzleState, BlankSlide> a_1;
    // WeightedAStar<TilePuzzleState, BlankSlide> a_low;
    // WeightedAStar<TilePuzzleState, BlankSlide> a_high;

    TilePuzzleTransitions tile_ops(3, 4);
    a_1.setTransitionSystem(&tile_ops);
    // a_low.setTransitionSystem(&tile_ops);
    // a_high.setTransitionSystem(&tile_ops);

    TilePuzzleState goal_state(3, 4);

    SingleGoalTest<TilePuzzleState> goal_test(goal_state);
    a_1.setGoalTest(&goal_test);
    // a_low.setGoalTest(&goal_test);
    // a_high.setGoalTest(&goal_test);

    PermutationHashFunction<TilePuzzleState> tile_hash;
    a_1.setHashFunction(&tile_hash);
    // a_low.setHashFunction(&tile_hash);
    // a_high.setHashFunction(&tile_hash);

    TileManhattanDistance manhattan(goal_state, tile_ops);
    a_1.setHeuristic(&manhattan);
    // a_low.setHeuristic(&manhattan);
    // a_high.setHeuristic(&manhattan);

    a_1.setTieBreaker(tiebreaker);
    // a_low.setTieBreaker(1);
    // a_high.setTieBreaker(2);

    a_1.setWeights(weight);
    // a_low.setWeights(weight);
    // a_high.setWeights(weight);

    vector<BlankSlide> solution;
    vector<vector<unsigned> > starts;

    read_in_permutations("../src/domains/tile_puzzle/tile_files/3x4_puzzle.probs", starts);

    // vector<int> node_count(starts.size());
    // vector<int> cost_count(starts.size());

    vector<int> node_count(100);
    vector<int> cost_count(100);

    for (unsigned i = 0; i < 100; i++) {        

        TilePuzzleState start_state(starts[27], 3, 4);

        a_1.getPlan(start_state, solution);

        node_count[i] = a_1.getGoalTestCount();

        cost_count[i] = a_1.getLastPlanCost();

    }

    double median_nodes = compute_median(node_count);
    double average_nodes = compute_average(node_count);
    double variance_nodes = compute_variance(node_count);
    // double median_cost = compute_median(cost_count);
    // double average_cost = compute_average(cost_count);


    cout << "Weighted A Star, weight = " << weight << endl;
    cout << "Median nodes: " << median_nodes << endl;
    cout << "Average nodes: " << average_nodes << endl;
    cout << "Variance nodes: " << variance_nodes << endl;
    // cout << "Median cost: " << median_cost << endl;
    // cout << "Average cost: " << average_cost << endl;

    // read_in_permutations("../src/domains/tile_puzzle/tile_files/3x4_puzzle.probs", starts_low);

    // vector<int> a1_nodes_low(starts_low.size());
    // vector<int> a1_cost_low(starts_low.size());
    // // vector<int> gbfs_nodes_low(starts_low.size());
    // // vector<int> gbfs_cost_low(starts_low.size());




    // for(unsigned i = 0; i < starts_low.size(); i++) {
    //     TilePuzzleState start_state_low(starts_low[i], 3, 4);

    //     a_low.getPlan(start_state_low, solution);
    //     // gbfs_low.getPlan(start_state_low, solution);

    //     a1_nodes_low[i] = a_low.getGoalTestCount();
    //     // gbfs_nodes_low[i] = gbfs_low.getGoalTestCount();

    //     int cur_cost = a_low.getLastPlanCost();
    //     a1_cost_low[i] = cur_cost;
    //     // gbfs_cost_low[i] = gbfs_low.getLastPlanCost();



    // }



    // double a1_median_nodes_low = compute_median(a1_nodes_low);
    // // double gbfs_median_nodes_low = compute_median(gbfs_nodes_low);

    // double a1_average_nodes_low = compute_average(a1_nodes_low);
    // // double gbfs_average_nodes_low = compute_average(gbfs_nodes_low);

    // double a1_median_cost_low = compute_median(a1_cost_low);
    // // double gbfs_median_cost_low = compute_median(gbfs_cost_low);

    // double a1_average_cost_low = compute_average(a1_cost_low);
    // // double gbfs_average_cost_low = compute_average(gbfs_cost_low);

    // cout << "==Low-G==" << endl;
    // cout << "Median nodes: " << a1_median_nodes_low << endl;
    // cout << "Average nodes: " << a1_average_nodes_low << endl;
    // cout << "Median cost: " << a1_median_cost_low << endl;
    // cout << "Average cost: " << a1_average_cost_low << endl;
    

    // read_in_permutations("../src/domains/tile_puzzle/tile_files/3x4_puzzle.probs", starts_high);
    // vector<int> a1_nodes_high(starts_high.size());
    // vector<int> a1_cost_high(starts_high.size());
    // // vector<int> gbfs_nodes_high(starts_high.size());
    // // vector<int> gbfs_cost_high(starts_high.size());


    // for(unsigned i = 0; i < starts_high.size(); i++) {
    //     TilePuzzleState start_state_high(starts_high[i], 3, 4);

    //     a_high.getPlan(start_state_high, solution);
    //     // gbfs_high.getPlan(start_state_high, solution);

    //     a1_nodes_high[i] = a_high.getGoalTestCount();
    //     // gbfs_nodes_high[i] = gbfs_high.getGoalTestCount();

    //     a1_cost_high[i] = a_high.getLastPlanCost();
    //     // gbfs_cost_high[i] = gbfs_high.getLastPlanCost();

    // }

    // double a1_median_nodes_high = compute_median(a1_nodes_high);
    // // double gbfs_median_nodes_high = compute_median(gbfs_nodes_high);

    // double a1_average_nodes_high = compute_average(a1_nodes_high);
    // // double gbfs_average_nodes_high = compute_average(gbfs_nodes_high);

    // double a1_median_cost_high = compute_median(a1_cost_high);
    // // double gbfs_median_cost_high = compute_median(gbfs_cost_high);

    // double a1_average_cost_high = compute_average(a1_cost_high);
    // // double gbfs_average_cost_high = compute_average(gbfs_cost_high);

    // cout << "==High-G==" << endl;
    // cout << "Median nodes: " << a1_median_nodes_high << endl;
    // cout << "Average nodes: " << a1_average_nodes_high << endl;
    // cout << "Median cost: " << a1_median_cost_high << endl;
    // cout << "Average cost: " << a1_average_cost_high << endl;

    // vector<double> nodes = compute_percent(a1_nodes, a1_nodes_low, a1_nodes_high);
    // vector<double> cost = compute_percent(a1_cost, a1_cost_low, a1_cost_high);

    // // vector<double> gbfs_percent_nodes = compute_percent(gbfs_nodes, gbfs_nodes_low, gbfs_nodes_high);
    // // vector<double> gbfs_percent_cost = compute_percent(gbfs_cost, gbfs_cost_low, gbfs_cost_high);

    // cout << "Percent Node expansions" << endl;
    // cout << "Default Low-G High-G" << endl;
    // cout << nodes[0] << "\t" << nodes[1] << "\t" <<  nodes[2] << endl;

    // cout << "Percent Cost" << endl;
    // cout << "Default Low-G High-G" << endl;
    // cout << cost[0] << "\t" << cost[1] << "\t" << cost[2] << endl;

    // cout << "===========" << endl;
    // cout << "GBFS" << endl;

    // cout << "==Default==" << endl;
    // cout << "Median nodes: " << gbfs_median_nodes << endl;
    // cout << "Average nodes: " << gbfs_average_nodes << endl;
    // cout << "Median cost: " << gbfs_median_cost << endl;
    // cout << "Average cost: " << gbfs_average_cost << endl;

    // cout << "==Low-G==" << endl;
    // cout << "Median nodes: " << gbfs_median_nodes_low << endl;
    // cout << "Average nodes: " << gbfs_average_nodes_low << endl;
    // cout << "Median cost: " << gbfs_median_cost_low << endl;
    // cout << "Average cost: " << gbfs_average_cost_low << endl;

    // cout << "==High-G==" << endl;
    // cout << "Median nodes: " << gbfs_median_nodes_high << endl;
    // cout << "Average nodes: " << gbfs_average_nodes_high << endl;
    // cout << "Median cost: " << gbfs_median_cost_high << endl;
    // cout << "Average cost: " << gbfs_average_cost_high << endl;
    
    // cout << "Percent Node expansions" << endl;
    // cout << "Default Low-G High-G" << endl;
    // cout << gbfs_percent_nodes[0] << "\t" << gbfs_percent_nodes[1] << "\t" <<  gbfs_percent_nodes[2] << endl;

    // cout << "Percent Cost" << endl;
    // cout << "Default Low-G High-G" << endl;
    // cout << gbfs_percent_cost[0] << "\t" << gbfs_percent_cost[1] << "\t" << gbfs_percent_cost[2] << endl;



    return 0;
}
