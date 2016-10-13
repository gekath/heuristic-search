/*
 * tile_puzzle_transitions.cpp
 *
 * LICENSE HERE
 *
 *  Created on: 2016-08-16
 *      Author: Rick Valenzano
 */

#include "tile_puzzle_transitions.h"

using std::vector;

TilePuzzleTransitions::TilePuzzleTransitions(unsigned rows, unsigned cols, TileCostType cost_type)
{
    setProperties(rows, cols, cost_type);
}

TilePuzzleTransitions::TilePuzzleTransitions(unsigned rows, unsigned cols, const std::vector<double> &tile_costs)
{
    setProperties(rows, cols, tile_costs);
}

TilePuzzleTransitions::~TilePuzzleTransitions()
{
}

bool TilePuzzleTransitions::isApplicable(const TilePuzzleState& state, const BlankSlide& action) const
{
    if(action == BlankSlide::dummy)
        return false;
    return isApplicableInLocation(action, state.blank_loc);
}

double TilePuzzleTransitions::getActionCost(const TilePuzzleState& state, const BlankSlide& action) const
{
    return tile_move_cost[getMovingTile(state, action)];
}

void TilePuzzleTransitions::applyAction(TilePuzzleState& state, const BlankSlide& action) const
{
    if(action == BlankSlide::up) {
        state.permutation[state.blank_loc] = state.permutation[state.blank_loc - num_cols];
        state.blank_loc -= num_cols;
    } else if(action == BlankSlide::right) {
        state.permutation[state.blank_loc] = state.permutation[state.blank_loc + 1];
        state.blank_loc++;
    } else if(action == BlankSlide::down) {
        state.permutation[state.blank_loc] = state.permutation[state.blank_loc + num_cols];
        state.blank_loc += num_cols;
    } else if(action == BlankSlide::left) {
        state.permutation[state.blank_loc] = state.permutation[state.blank_loc - 1];
        state.blank_loc--;
    }

    state.permutation[state.blank_loc] = 0;
}

void TilePuzzleTransitions::getActions(const TilePuzzleState& state, vector<BlankSlide>& actions) const
{
    for(unsigned i = 0; i < loc_actions[state.blank_loc].size(); i++)
        actions.push_back(loc_actions[state.blank_loc][i]);
}

bool TilePuzzleTransitions::isApplicableInLocation(const BlankSlide& action, unsigned blank_loc) const
{
    if(action == BlankSlide::up) {
        if(blank_loc >= num_cols)
            return true;
    } else if(action == BlankSlide::right) {
        if(blank_loc % num_cols < num_cols - 1)
            return true;
    } else if(action == BlankSlide::down) {
        if(blank_loc < (num_rows - 1) * num_cols)
            return true;
    } else if(action == BlankSlide::left) {
        if(blank_loc % num_cols > 0)
            return true;
    }
    return false;
}

void TilePuzzleTransitions::setStandardTileCostType(const TileCostType cost_type)
{
    tile_move_cost[0] = 0;
    for(unsigned i = 1; i < num_rows * num_cols; i++) {
        if(cost_type == TileCostType::unit)
            tile_move_cost[i] = 1.0;
        else if(cost_type == TileCostType::heavy)
            tile_move_cost[i] = i;
        else if(cost_type == TileCostType::inverse)
            tile_move_cost[i] = 1 / ((double) i);
    }
}

void TilePuzzleTransitions::setCustomTileCostType(const std::vector<double>& costs)
{
    tile_move_cost[0] = 0;
    for(unsigned i = 1; i < num_rows * num_cols; i++) {
        tile_move_cost[i] = costs[i];
    }
}

void TilePuzzleTransitions::setActionList(unsigned rows, unsigned cols)
{

    num_rows = rows;
    num_cols = cols;

    tile_move_cost.resize(num_rows * num_cols);

    ops_in_order.push_back(BlankSlide::up);
    ops_in_order.push_back(BlankSlide::right);
    ops_in_order.push_back(BlankSlide::down);
    ops_in_order.push_back(BlankSlide::left);

    loc_actions.clear();

    vector<BlankSlide> blank_actions(4);
    for(unsigned i = 0; i < num_rows * num_cols; i++) {
        blank_actions.clear();
        for(unsigned j = 0; j < ops_in_order.size(); j++) {
            if(isApplicableInLocation(ops_in_order[j], i))
                blank_actions.push_back(ops_in_order[j]);
        }
        loc_actions.push_back(blank_actions);
    }
}

void TilePuzzleTransitions::setProperties(unsigned rows, unsigned cols, TileCostType cost_type)
{
    setActionList(rows, cols);
    setStandardTileCostType(cost_type);
}

void TilePuzzleTransitions::setProperties(unsigned rows, unsigned cols, const std::vector<double>& tile_costs)
{
    setActionList(rows, cols);
    setCustomTileCostType(tile_costs);
}

double TilePuzzleTransitions::getTileMoveCost(unsigned tile) const
{
    return tile_move_cost[tile];
}

BlankSlide TilePuzzleTransitions::getDummyAction() const
{
    return BlankSlide::dummy;
}

bool TilePuzzleTransitions::isDummyAction(const BlankSlide& action) const
{
    return (action == BlankSlide::dummy);
}

unsigned TilePuzzleTransitions::getMovingTile(const TilePuzzleState& state, const BlankSlide& action) const
{
    if(action == BlankSlide::up)
        return state.permutation[state.blank_loc - num_cols];
    else if(action == BlankSlide::right)
        return state.permutation[state.blank_loc + 1];
    else if(action == BlankSlide::down)
        return state.permutation[state.blank_loc + num_cols];
    else if(action == BlankSlide::left)
        return state.permutation[state.blank_loc - 1];
    return 0;
}

std::ostream& operator <<(std::ostream& out, const BlankSlide& action)
{
    if(action == BlankSlide::up)
        out << "up";
    else if(action == BlankSlide::right)
        out << "right";
    else if(action == BlankSlide::down)
        out << "down";
    else if(action == BlankSlide::left)
        out << "left";
    else if(action == BlankSlide::dummy)
        out << "dummy";
    return out;
}

bool TilePuzzleTransitions::isInvertible(const TilePuzzleState &state, const BlankSlide& action) const
{
    if(action == BlankSlide::dummy)
        return false;
    return true;
}

BlankSlide TilePuzzleTransitions::getInverse(const TilePuzzleState &state, const BlankSlide& action) const
{
    if(action == BlankSlide::up)
        return BlankSlide::down;
    else if(action == BlankSlide::right)
        return BlankSlide::left;
    else if(action == BlankSlide::down)
        return BlankSlide::up;
    else if(action == BlankSlide::left)
        return BlankSlide::right;

    return BlankSlide::dummy;
}
