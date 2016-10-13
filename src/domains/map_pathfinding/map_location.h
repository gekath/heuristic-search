/*
 * map_location.h
 *
 * LICENSE HERE
 *
 *  Created on: 2016-09-23
 *      Author: Rick Valenzano
 */

#ifndef MAP_LOCATION_H_
#define MAP_LOCATION_H_

#include <cstdint>
#include <iostream>

/**
 * Defines a state for 2D map pathfinding. Each state is a location in the map given as a set of coordinates.
 *
 * @class MapLocation
 */
class MapLocation
{
public:
    /**
     * Default constructor for a map location. Sets the coordinates to (0,0).
     */
    MapLocation();

    /**
     * Constructor for the map location that sets the coordinates to the given values.
     * @param x_loc The x coordinate for the state.
     * @param y_loc The y coordinate for the state.
     */
    MapLocation(uint16_t x_loc, uint16_t y_loc);

    /**
     * Destructor for the map location. Does nothing.
     */
    virtual ~MapLocation();

    uint16_t x; ///< The x coordinate of the state.
    uint16_t y; ///< The y coordinate of the state.
};

/**
 * Outputs a string representation of a map location to the given output stream.
 *
 * @param out The output stream.
 * @param loc The map location to output.
 * @return The output stream.
 */
std::ostream& operator <<(std::ostream & out, const MapLocation &loc);

/**
 * Defines equality of two map locations.
 *
 * @param loc1 The first location to test.
 * @param loc2 The second location to test.
 * @return If the locations are equal or not.
 */
bool operator ==(const MapLocation &loc1, const MapLocation &loc2);

/**
 * Defines inequality of two map locations.
 *
 * @param loc1 The first location to test.
 * @param loc2 The second location to test.
 * @return If the locations are not equal.
 */
bool operator !=(const MapLocation &loc1, const MapLocation &loc2);

#endif /* MAP_LOCATION_H_ */
