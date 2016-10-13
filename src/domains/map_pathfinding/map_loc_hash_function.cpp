/*
 * map_loc_hash_function.cpp
 *
 * LICENSE HERE
 *
 *  Created on: 2016-09-29
 *      Author: Rick Valenzano
 */

#include "map_loc_hash_function.h"

MapLocHashFunction::MapLocHashFunction() : map_width(0), map_height(0)
{
}

MapLocHashFunction::~MapLocHashFunction()
{
}

StateHash MapLocHashFunction::getStateHash(const MapLocation& state) const
{
    //TODO Error checking
    //TODO Take advantage of 16bit
    return map_width*((uint64_t)state.x) + ((uint64_t)state.y);
}

void MapLocHashFunction::setMapDimensions(const MapPathfindingTransitions& ops)
{
    setMapDimensions(ops.getMapWidth(), ops.getMapHeight());
}

void MapLocHashFunction::setMapDimensions(unsigned width, unsigned height)
{
    map_width = width;
    map_height = height;
}
