/*
 * map_octile_distance.h
 *
 * LICENSE HERE
 *
 *  Created on: 2016-09-29
 *      Author: Rick Valenzano
 */

#ifndef MAP_OCTILE_DISTANCE_H_
#define MAP_OCTILE_DISTANCE_H_

#include "../../generic_defs/heuristic.h"
#include "map_location.h"

/**
 * A class that defines the octile heuristic function for a map pathfinding domain.
 *
 * @class MapOctileDistance
 */
class MapOctileDistance: public Heuristic<MapLocation>
{
public:
    /**
     * Constructor for a octile heuristic function. Assumes a goal of (0,0) and squarer root 2 diagonal cost.
     */
    MapOctileDistance();

    /**
     * Destructor for octile heuristic function. Does nothing.
     */
    virtual ~MapOctileDistance();

    /**
     * Sets the goal location to the given map location.
     *
     * @param state The new goal.
     */
    void setGoal(const MapLocation &state);

    /**
     * Sets the goal to use the given coordinates.
     *
     * @param x_loc The x location of the goal.
     * @param y_loc The y location of the goal.
     */
    void setGoal(uint16_t x_loc, uint16_t y_loc);

    /**
     * Sets the cost of a diagonal move.
     *
     * Value must be larger than 0.0. False is returned if it is not.
     *
     * @param d_cost The new diagonal move cost.
     * @return If the diagonal move cost was successfully set;
     */
    bool setDiagonalCost(double d_cost);

protected:
    // Overloaded methods
    virtual double computeHValue(const MapLocation &state) const;

    MapLocation goal; ///< The goal location currently being used.
    double diag_cost; ///< The cost of diagonal moves.
};

#endif /* MAP_OCTILE_DISTANCE_H_ */
