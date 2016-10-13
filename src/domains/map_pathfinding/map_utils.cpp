/*
 * map_utils.cpp
 *
 * LICENSE HERE
 *
 *  Created on: 2016-09-30
 *      Author: Rick Valenzano
 */

#include <sstream>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "map_utils.h"
#include "../../utils/string_utils.h"

using std::string;
using std::vector;
using std::stringstream;
using std::ifstream;
using std::cout;
using std::cerr;
using std::endl;

bool read_in_pathfinding_probs(string file_name, vector<MapLocation> &starts, vector<MapLocation> &goals)
{
    ifstream in_stream(file_name, std::ios::in);

    if(in_stream.fail()) {
        cerr << "Could not read in map problem file " << file_name << ".\n";
        return false;
    }

    string new_line;
    vector<string> tokens;

    while(getline(in_stream, new_line)) {
        tokens.clear();
        split(new_line, tokens, ' ');

        uint16_t start_x = atoi(tokens[0].c_str());
        uint16_t start_y = atoi(tokens[1].c_str());

        uint16_t goal_x = atoi(tokens[2].c_str());
        uint16_t goal_y = atoi(tokens[3].c_str());

        starts.push_back(MapLocation(start_x, start_y));
        goals.push_back(MapLocation(goal_x, goal_y));
    }

    return true;
}
