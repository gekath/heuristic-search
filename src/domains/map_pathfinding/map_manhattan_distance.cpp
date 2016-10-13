/*
 * map_manhattan_distance.cpp
 *
 * LICENSE HERE
 *
 *  Created on: 2016-09-29
 *      Author: Rick Valenzano
 */

#include "map_manhattan_distance.h"

#include <stdlib.h>

MapManhattanDistance::MapManhattanDistance()
{
}

MapManhattanDistance::~MapManhattanDistance()
{
}

void MapManhattanDistance::setGoal(const MapLocation &state)
{
    setGoal(state.x, state.y);
}

void MapManhattanDistance::setGoal(uint16_t x_loc, uint16_t y_loc)
{
    goal.x = x_loc;
    goal.y = y_loc;
}

double MapManhattanDistance::computeHValue(const MapLocation& state) const
{
    return abs(goal.x - state.x) + abs(goal.y - state.y);
}
