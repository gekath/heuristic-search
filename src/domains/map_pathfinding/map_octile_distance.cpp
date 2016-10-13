/*
 * map_octile_distance.cpp
 *
 * LICENSE HERE
 *
 *  Created on: 2016-09-29
 *      Author: Rick Valenzano
 */

#include "map_octile_distance.h"
#include "../../utils/floating_point_utils.h"

MapOctileDistance::MapOctileDistance()
        : diag_cost(ROOT_TWO)
{
}

MapOctileDistance::~MapOctileDistance()
{
}

void MapOctileDistance::setGoal(const MapLocation &state)
{
    setGoal(state.x, state.y);
}

void MapOctileDistance::setGoal(uint16_t x_loc, uint16_t y_loc)
{
    goal.x = x_loc;
    goal.y = y_loc;
}

bool MapOctileDistance::setDiagonalCost(double d_cost)
{
    if(!fp_greater(d_cost, 0.0)) {
        //TODO Add error message
        return false;
    }
    diag_cost = d_cost;
    return true;
}

double MapOctileDistance::computeHValue(const MapLocation& state) const
{
    if(!fp_less(diag_cost, 2.0))
        return abs(goal.x - state.x) + abs(goal.y - state.y);

    double delta_x = abs(goal.x - state.x);
    double delta_y = abs(goal.y - state.y);

    if(fp_less(delta_x, delta_y))
        return delta_x*diag_cost + delta_y - delta_x;
    return delta_y*diag_cost + delta_x - delta_y;
}
