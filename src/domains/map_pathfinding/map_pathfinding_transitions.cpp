/*
 * map_pathfinding_transitions.cpp
 *
 * LICENSE HERE
 *
 *  Created on: 2016-09-23
 *      Author: Rick Valenzano
 */

#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>

#include "map_pathfinding_transitions.h"

#include <cassert>

#include "../../utils/string_utils.h"
#include "../../utils/floating_point_utils.h"

using std::string;
using std::vector;
using std::stringstream;
using std::ifstream;
using std::cout;
using std::cerr;
using std::endl;

MapPathfindingTransitions::MapPathfindingTransitions(bool four_dirs)
        : map_width(0), map_height(0), four_connected(four_dirs), diag_cost(ROOT_TWO)
{
}

MapPathfindingTransitions::MapPathfindingTransitions(std::string file_name, bool four_dirs)
        : MapPathfindingTransitions(four_dirs)
{
    loadMap(file_name);
}

MapPathfindingTransitions::~MapPathfindingTransitions()
{
}

bool MapPathfindingTransitions::loadMap(string file_name)
{
    clearMap();
    ifstream in_stream(file_name, std::ios::in);

    if(in_stream.fail()) {
        cerr << "Could not read in map file " << file_name << ". ";
        cerr << "Map reading failed." << endl;
        return false;
    }

    string new_line;
    vector<string> tokens;

    bool getting_map = false;

    unsigned current_row = 0;

    unsigned line_count = 0;
    while(getline(in_stream, new_line)) {
        line_count++;

        if(!getting_map) {

            if(new_line == "map") {
                if(map_height == 0 || map_width == 0) {
                    cerr << "Map width or height not specified in map file " << file_name << ".";
                    cerr << "Map reading failed." << endl;
                    clearMap();
                    return false;
                }
                getting_map = true;
            } else {
                tokens.clear();
                split(new_line, tokens, ' ');

                if(tokens.size() > 0 && tokens[0] == "width") {
                    if(map_width > 0) {
                        cerr << "Map width entered multiple times in map file " << file_name << ".";
                        cerr << "Map reading failed." << endl;
                        clearMap();
                        return false;
                    } else if(tokens.size() == 2) {

                        int new_width = atoi(tokens[1].c_str());
                        if(new_width <= 0) {
                            cerr << "Invalid width entered in map file " << file_name << ".";
                            cerr << "Map reading failed." << endl;
                            clearMap();
                            return false;
                        }
                        map_width = new_width;
                    } else {
                        cerr << "Improper formatting of width specification in map file " << file_name << ".";
                        cerr << "Map reading failed." << endl;
                        clearMap();
                        return false;
                    }
                } else if(tokens.size() > 0 && tokens[0] == "height") {
                    if(map_height > 0) {
                        cerr << "Map height entered multiple times in map file " << file_name << ".";
                        cerr << "Map reading failed." << endl;
                        clearMap();
                        return false;
                    } else if(tokens.size() == 2) {

                        int new_height = atoi(tokens[1].c_str());
                        if(new_height <= 0) {
                            cerr << "Invalid height entered in map file " << file_name << ".";
                            cerr << "Map reading failed." << endl;
                            clearMap();
                            return false;
                        }
                        map_height = new_height;
                    } else {
                        cerr << "Improper formatting of height specification in map file " << file_name << ".";
                        cerr << "Map reading failed." << endl;
                        clearMap();
                        return false;
                    }
                }
            }
        } else {

            if(grid_empty.size() == 0)
                grid_empty.resize(map_height, vector<bool>(map_width, false));

            if(new_line.size() != map_width) {
                cerr << "Line " << line_count << " of map file " << file_name << " has incorrect length";
                cerr << " for the specified width. Map reading failed." << endl;
                clearMap();
                return false;
            }

            for(unsigned i = 0; i < new_line.size(); i++) {
                if(new_line.at(i) == '.' || new_line.at(i) == 'G' || new_line.at(i) == 'S')
                    grid_empty[i][current_row] = true;
                else if(new_line.at(i) != '@' && new_line.at(i) != 'O' && new_line.at(i) != 'T'
                        && new_line.at(i) != 'W') {
                    cerr << "Invalid map location symbol " << new_line.at(i) << " on line " << line_count;
                    cerr << " of map file " << file_name << ". Map reading failed." << endl;
                    clearMap();
                    return false;
                }
            }
            current_row++;
        }
    }

    if(!getting_map) {
        cerr << "'map' keyword did not appear in map file " << file_name << ".";
        cerr << "Map reading failed." << endl;
        clearMap();
        return false;
    } else if(current_row != map_height) {
        cerr << "Number of map rows in map file " << file_name << " does not match listed height.";
        cerr << "Map reading failed." << endl;
        clearMap();
        return false;
    }
    return true;
}

unsigned MapPathfindingTransitions::getMapWidth() const
{
    return map_width;
}

unsigned MapPathfindingTransitions::getMapHeight() const
{
    return map_height;
}

void MapPathfindingTransitions::set4Connected()
{
    four_connected = true;
}

void MapPathfindingTransitions::set8Connected()
{
    four_connected = false;
}

bool MapPathfindingTransitions::setDiagonalCost(double d_cost)
{
    if(fp_less(d_cost, 0.0)) {
        cerr << "Diagonal cost of " << d_cost << " is less than 0.0 and is thus invalid." << endl;
        return false;
    }
    diag_cost = d_cost;
    return true;
}

bool MapPathfindingTransitions::is4Connected() const
{
    return four_connected;
}

bool MapPathfindingTransitions::is8Connected() const
{
    return !four_connected;
}

bool MapPathfindingTransitions::isApplicable(const MapLocation& state, const MapDir& action) const
{
    switch(action) {
        case MapDir::north:
            return canNorth(state);
        case MapDir::east:
            return canEast(state);
        case MapDir::south:
            return canSouth(state);
        case MapDir::west:
            return canWest(state);
        default:
            break;
    }
    if(!four_connected) {
        switch(action) {
            case MapDir::northeast:
                return canNorth(state) && canEast(state) && isNEEmpty(state);
            case MapDir::southeast:
                return canEast(state) && canSouth(state) && isSEEmpty(state);
            case MapDir::southwest:
                return canSouth(state) && canWest(state) && isSWEmpty(state);
            case MapDir::northwest:
                return canNorth(state) && canWest(state) && isNWEmpty(state);
            default:
                break;
        }
    }
    return false;
}

double MapPathfindingTransitions::getActionCost(const MapLocation& state, const MapDir& action) const
{
    if(action == MapDir::north || action == MapDir::east || action == MapDir::south || action == MapDir::west)
        return 1.0;
    return diag_cost;
}

void MapPathfindingTransitions::applyAction(MapLocation& state, const MapDir& action) const
{
    switch(action) {
        case MapDir::north:
            state.y--;
            break;
        case MapDir::northeast:
            state.x++;
            state.y--;
            break;
        case MapDir::east:
            state.x++;
            break;
        case MapDir::southeast:
            state.x++;
            state.y++;
            break;
        case MapDir::south:
            state.y++;
            break;
        case MapDir::southwest:
            state.x--;
            state.y++;
            break;
        case MapDir::west:
            state.x--;
            break;
        case MapDir::northwest:
            state.x--;
            state.y--;
            break;
        default:
            break;
    }
}

void MapPathfindingTransitions::getActions(const MapLocation& state, std::vector<MapDir>& actions) const
{
    bool north = canNorth(state);
    bool east = canEast(state);
    bool south = canSouth(state);
    bool west = canWest(state);

    if(north)
        actions.push_back(MapDir::north);
    if(!four_connected && north && east && isNEEmpty(state))
        actions.push_back(MapDir::northeast);
    if(east)
        actions.push_back(MapDir::east);
    if(!four_connected && east && south && isSEEmpty(state))
        actions.push_back(MapDir::southeast);
    if(south)
        actions.push_back(MapDir::south);
    if(!four_connected && south && west && isSWEmpty(state))
        actions.push_back(MapDir::southwest);
    if(west)
        actions.push_back(MapDir::west);
    if(!four_connected && north && west && isNWEmpty(state))
        actions.push_back(MapDir::northwest);
}

bool MapPathfindingTransitions::isInvertible(const MapLocation& state, const MapDir& action) const
{
    if(action == MapDir::dummy)
        return false;
    return true;
}

MapDir MapPathfindingTransitions::getInverse(const MapLocation& state, const MapDir& action) const
{
    switch(action) {
        case MapDir::north:
            return MapDir::south;
        case MapDir::northeast:
            return MapDir::southwest;
        case MapDir::east:
            return MapDir::west;
        case MapDir::southeast:
            return MapDir::northwest;
        case MapDir::south:
            return MapDir::north;
        case MapDir::southwest:
            return MapDir::northeast;
        case MapDir::west:
            return MapDir::east;
        case MapDir::northwest:
            return MapDir::southeast;
        default:
            break;
    }
    return MapDir::dummy;
}

MapDir MapPathfindingTransitions::getDummyAction() const
{
    return MapDir::dummy;
}

bool MapPathfindingTransitions::isDummyAction(const MapDir& action) const
{
    return action == MapDir::dummy;
}

void MapPathfindingTransitions::clearMap()
{
    map_width = 0;
    map_height = 0;
    grid_empty.clear();
}

bool MapPathfindingTransitions::canNorth(const MapLocation& state) const
{
    assert(state.x < map_width);
    assert(state.y < map_height);

    if(state.y > 0 && grid_empty[state.x][state.y - 1])
        return true;
    return false;
}

bool MapPathfindingTransitions::canEast(const MapLocation& state) const
{
    if(state.x < map_width - 1 && grid_empty[state.x + 1][state.y])
        return true;
    return false;
}

bool MapPathfindingTransitions::canSouth(const MapLocation& state) const
{
    if(state.y < map_height - 1 && grid_empty[state.x][state.y + 1])
        return true;
    return false;
}

bool MapPathfindingTransitions::canWest(const MapLocation& state) const
{
    if(state.x > 0 && grid_empty[state.x - 1][state.y])
        return true;
    return false;
}

bool MapPathfindingTransitions::isNEEmpty(const MapLocation& state) const
{
    return grid_empty[state.x + 1][state.y - 1];
}

bool MapPathfindingTransitions::isSEEmpty(const MapLocation& state) const
{
    return grid_empty[state.x + 1][state.y + 1];
}

bool MapPathfindingTransitions::isSWEmpty(const MapLocation& state) const
{
    return grid_empty[state.x - 1][state.y + 1];
}

bool MapPathfindingTransitions::isNWEmpty(const MapLocation& state) const
{
    return grid_empty[state.x - 1][state.y - 1];
}

std::ostream& operator <<(std::ostream& out, const MapDir& action)
{
    switch(action) {
        case MapDir::north:
            out << "north";
            break;
        case MapDir::northeast:
            out << "northeast";
            break;
        case MapDir::east:
            out << "east";
            break;
        case MapDir::southeast:
            out << "southeast";
            break;
        case MapDir::south:
            out << "south";
            break;
        case MapDir::southwest:
            out << "southwest";
            break;
        case MapDir::west:
            out << "west";
            break;
        case MapDir::northwest:
            out << "northwest";
            break;
        default:
            out << "dummy";
            break;
    }
    return out;
}
