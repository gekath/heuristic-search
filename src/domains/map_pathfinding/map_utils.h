/*
 * map_utils.h
 *
 * LICENSE HERE
 *
 *  Created on: 2016-09-30
 *      Author: Rick Valenzano
 */
/**
 * Utilities for maps.
 *
 * @file map_utils.h
 */

#ifndef MAP_UTILS_H_
#define MAP_UTILS_H_

#include <string>
#include <vector>
#include "map_location.h"

/**
 * Reads in the pathfinding problems from the given file.
 *
 * The input format is assumed to be as follows. Each line represents a different problem, and each
 * is defined by 4 integers. The first two are the x and y coordinates of the start state. The last
 * two are the x and y coordinates of the goal state.
 *
 * @todo Add more extensive error checking.
 *
 * @param file_name The file name to read from.
 * @param starts The vector in which to store the start states.
 * @param goals The vector in which to store the goal states.
 * @return If the read was successful or not.
 */
bool read_in_pathfinding_probs(std::string file_name, std::vector<MapLocation> &starts, std::vector<MapLocation> &goals);

#endif /* MAP_UTILS_H_ */
