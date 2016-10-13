/*
 * map_location.cpp
 *
 * LICENSE HERE
 *
 *  Created on: 2016-09-23
 *      Author: Rick Valenzano
 */

#include "map_location.h"

MapLocation::MapLocation() : x(0), y(0)
{
}

MapLocation::MapLocation(uint16_t x_loc, uint16_t y_loc) : x(x_loc), y(y_loc)
{
}

MapLocation::~MapLocation()
{
}

std::ostream& operator <<(std::ostream& out, const MapLocation& loc)
{
    out << "(" << loc.x << ", " << loc.y << ")";
    return out;
}

bool operator ==(const MapLocation& loc1, const MapLocation& loc2)
{
    return (loc1.x == loc2.x) && (loc1.y == loc2.y);
}

bool operator !=(const MapLocation& loc1, const MapLocation& loc2)
{
    return !(loc1 == loc2);
}
