/*
 * map_pathfinding_transitions.h
 *
 * LICENSE HERE
 *
 *  Created on: 2016-09-23
 *      Author: Rick Valenzano
 */

#ifndef MAP_PATHFINDING_TRANSITIONS_H_
#define MAP_PATHFINDING_TRANSITIONS_H_

#include "map_location.h"
#include "../../generic_defs/transition_system.h"

/**
 * Defines the actions possible in the 2D map pathfinding domain.
 *
 * @class MapDir
 */
enum class MapDir
{
    north, northeast, east, southeast, south, southwest, west, northwest, dummy
};

/**
 * Defines the transitions for map pathfinding.
 *
 * Currently sets the origin (0,0) as the upper left corner in keeping with benchmark standard.
 * Locations are either empty or have an obstacle. If the transition system is set as 4-connected,
 * then movement is only possible in the 4 cardinal directions (North, East, South, and West). If
 * it is not 4-connected, it is 8-connected, which means it is possible to move in the 4 cardinal
 * directions as well as Northeast, Southeast, Southwest, and Northwest.
 *
 * @todo Set the bottom left corner to be (0, 0)?
 * @todo Allow for different terrain types.
 * @todo Allow for different terrain heights.
 *
 * @class MapPathfindingTransitions
 */
class MapPathfindingTransitions: public TransitionSystem<MapLocation, MapDir>
{
public:
    /**
     * A constructor for a map pathfinding transition system. Constructs a completely empty map.
     *
     * @param four_dirs Whether the system is 4-connected. Set as true by default.
     */
    MapPathfindingTransitions(bool four_dirs = true);

    /**
     * A constructor for a map pathfinding transition system. Loads in the map from the file from the given file name.
     *
     * @param file_name The name of the file to load the map from.
     * @param four_dirs Whether the system is 4-connected. Set as true by default.
     */
    MapPathfindingTransitions(std::string file_name, bool four_dirs = true);

    /**
     * A destructor for a map pathfinding transition system. Does nothing.
     */
    virtual ~MapPathfindingTransitions();

    // Overloaded methods.
    virtual bool isApplicable(const MapLocation &state, const MapDir &action) const;
    virtual double getActionCost(const MapLocation &state, const MapDir &action) const;
    virtual void applyAction(MapLocation &state, const MapDir &action) const;
    virtual void getActions(const MapLocation &state, std::vector<MapDir> &actions) const;
    virtual bool isInvertible(const MapLocation &state, const MapDir &action) const;
    virtual MapDir getInverse(const MapLocation &state, const MapDir &action) const;
    MapDir getDummyAction() const;
    bool isDummyAction(const MapDir &action) const;

    /**
     * Loads the map from the file with the given file name.
     *
     * If reading fails, the map is cleared and false is returned.
     *
     * The file format is as follows. There must be a line that contains only the word "map", which indicates
     * that the remainder of the file is a representation of the map. An empty location is indicated by any
     * of the symbols '.' 'G' or 'S'. An obstacle is indicated by any of the symbols '@', 'O', 'W', or 'T.'
     * Any other symbol will cause an error.
     *
     * Prior to the line that says map, there must be at least two lines, one of which is of the form "width n"
     * and the other of which is "height m", where n and m are positive integers. These indicate the size of the
     * map. The reading will fail if these are not included.
     *
     * All other lines before the "map" line will be discarded.
     *
     * @param file_name The name of the file to read.
     * @return If the map loading succeeding or not.
     */
    bool loadMap(std::string file_name);

    /**
     * Returns the width of the map.
     *
     * @return The width of the map.
     */
    unsigned getMapWidth() const;

    /**
     * Returns the height of the map.
     *
     * @return The height of the map.
     */
    unsigned getMapHeight() const;

    /**
     * Sets the map to be 4-connected.
     */
    void set4Connected();

    /**
     * Sets the map to be 8-connected.
     */
    void set8Connected();

    /**
     * Sets the cost of diagonal move (ie. northeast, southeast, southwest, or northwest). By default, set
     * as square root two, as defined in floating_point_utils.h.
     *
     * Ignores the given value if it is non-positive.
     *
     * @param d_cost The cost of a diagonal move.
     * @return If the diagonal value was set properly.
     */
    bool setDiagonalCost(double d_cost);

    /**
     * Checks if the given map is 4-connected.
     *
     * @return If the map is set as 4-connected or 8-connected.
     */
    bool is4Connected() const;

    /**
     * Checks if the given map is 8-connected.
     *
     * @return If the map is set as 8-connected or not.
     */
    bool is8Connected() const;

    /**
     * Clears the current map.
     */
    void clearMap();

protected:

    /**
     * Returns true of it is possible to move north from the current location.
     *
     * @param state The state currently located at.
     * @return If it is possible to move north from the given location.
     */
    bool canNorth(const MapLocation &state) const;

    /**
     * Returns true of it is possible to move east from the current location.
     *
     * @param state The state currently located at.
     * @return If it is possible to move east from the given location.
     */
    bool canEast(const MapLocation &state) const;

    /**
     * Returns true of it is possible to move south from the current location.
     *
     * @param state The state currently located at.
     * @return If it is possible to move south from the given location.
     */
    bool canSouth(const MapLocation &state) const;

    /**
     * Returns true of it is possible to move west from the current location.
     *
     * @param state The state currently located at.
     * @return If it is possible to move west from the given location.
     */
    bool canWest(const MapLocation &state) const;

    /**
     * Checks if the location northeast of the given location is empty.
     *
     * @param state The state currently located at.
     * @return If the location northeast of the given location is empty.
     */
    bool isNEEmpty(const MapLocation &state) const;

    /**
     * Checks if the location southeast of the given location is empty.
     *
     * @param state The state currently located at.
     * @return If the location southeast of the given location is empty.
     */
    bool isSEEmpty(const MapLocation &state) const;

    /**
     * Checks if the location southwest of the given location is empty.
     *
     * @param state The state currently located at.
     * @return If the location southwest of the given location is empty.
     */
    bool isSWEmpty(const MapLocation &state) const;

    /**
     * Checks if the location northwest of the given location is empty.
     *
     * @param state The state currently located at.
     * @return If the location northwest of the given location is empty.
     */
    bool isNWEmpty(const MapLocation &state) const;

    unsigned map_width; ///< The map width.
    unsigned map_height; ///< The map height.

    bool four_connected; ///< If the map is 4-connected or 8-connected.
    double diag_cost; ///< The cost of a diagonal move.
    std::vector<std::vector<bool> > grid_empty; ///< Indexed by location, indicates if it is empty or an obstacle.
};

/**
 * Defines an output representation map pathfinding actions.
 *
 * @param out The output stream.
 * @param dir The action to output.
 * @return A string representation of the given action.
 */
std::ostream& operator <<(std::ostream & out, const MapDir &dir);

#endif /* MAP_PATHFINDING_TRANSITIONS_H_ */
