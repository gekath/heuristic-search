/*
 * map_loc_hash_function.h
 *
 * LICENSE HERE
 *
 *  Created on: 2016-09-29
 *      Author: Rick Valenzano
 */

#ifndef MAP_LOC_HASH_FUNCTION_H_
#define MAP_LOC_HASH_FUNCTION_H_

#include "../../generic_defs/state_hash_function.h"
#include "map_location.h"
#include "map_pathfinding_transitions.h"

/**
 * A class for calculating the hash value of a map location.
 *
 * @class MapLocHashFunction
 */
class MapLocHashFunction: public StateHashFunction<MapLocation>
{
public:
    /**
     * Constructor for a map location hash function. Initializes the map dimensions to zeroes.
     */
    MapLocHashFunction();

    /**
     * Destructor for a map location hash function. Does nothing.
     */
    virtual ~MapLocHashFunction();

    // Overloaded function
    virtual StateHash getStateHash(const MapLocation &state) const;

    /**
     * Sets the map dimensions based on the given map transition function.
     *
     * @param ops The map transition function.
     */
    void setMapDimensions(const MapPathfindingTransitions &ops);

    /**
     * Sets the map dimensions to the given values.
     *
     * @param width The map width.
     * @param height The map height.
     */
    void setMapDimensions(unsigned width, unsigned height);

protected:
    uint64_t map_width; ///< The width of the map. Stored as uint64_t to avoid extra type casting.
    uint64_t map_height; ///< The height of the map. Stored as uint64_t to avoid extra type casting.
};

#endif /* MAP_LOC_HASH_FUNCTION_H_ */
