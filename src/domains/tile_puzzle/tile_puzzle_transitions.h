/*
 * tile_puzzle_transitions.h
 *
 * LICENSE HERE
 *
 *  Created on: 2016-08-16
 *      Author: Rick Valenzano
 */

#ifndef TILEPUZZLETRANSITIONS_H_
#define TILEPUZZLETRANSITIONS_H_

#include <iostream>
#include "../../generic_defs/transition_system.h"
#include "tile_puzzle_state.h"

/**
 * Defines the applicable actions in the sliding tile puzzle domain.
 *
 * Note that left corresponds to "sliding the blank left." The action listed
 * as "dummy" is the dummy action.
 *
 * @class BlankSlide
 */
enum class BlankSlide
{
    left, up, down, right, dummy
};

/**
 * Defines the possible cost types for sliding tiles in the sliding tile puzzle domain.
 *
 * Below is the meaning of each of the values:
 *
 * unit - all actions have a cost of 1.\n
 * heavy - moving tile i has a cost of i.\n
 * inverse - moving tile i has a cost of 1/i.\n
 *
 * @class TileCostType
 */
enum class TileCostType
{
    unit, heavy, inverse
};

/**
 * Defines the transition system for the sliding tile puzzle.
 *
 * @todo Add operator ordering options.
 * @todo Check what the default operator ordering should be.
 *
 * @class TilePuzzleTransitions
 */
class TilePuzzleTransitions: public TransitionSystem<TilePuzzleState, BlankSlide>
{
public:
    /**
     * Builds a tile puzzle transition system with the given number of rows, columns, and cost type.
     *
     * @param rows The number of rows in the puzzle.
     * @param cols The number of columns in the puzzle.
     * @param cost The action cost type.
     */
    TilePuzzleTransitions(unsigned rows, unsigned cols, TileCostType cost = TileCostType::unit);

    /**
     * Builds a tile puzzle transition system with the given number of rows, columns, and custom tile cost type.
     *
     * The custom tile cost type is defined as list of numbers, where the value at index i is the cost of moving tile i.
     * The value at index 0 is ignored.
     *
     * @param rows The number of rows in the puzzle.
     * @param cols The number of columns in the puzzle.
     * @param tile_costs A list defining the custom tile cost.
     */
    TilePuzzleTransitions(unsigned rows, unsigned cols, const std::vector<double> &tile_costs);

    /**
     * Destructor for tile puzzle transition system. Destructor is empty.
     */
    virtual ~TilePuzzleTransitions();

    // Overloaded methods.
    virtual bool isApplicable(const TilePuzzleState &state, const BlankSlide &action) const;
    virtual double getActionCost(const TilePuzzleState &state, const BlankSlide &action) const;
    virtual void applyAction(TilePuzzleState &state, const BlankSlide &action) const;
    virtual void getActions(const TilePuzzleState &state, std::vector<BlankSlide> &actions) const;
    virtual bool isInvertible(const TilePuzzleState &state, const BlankSlide &action) const;
    virtual BlankSlide getInverse(const TilePuzzleState &state, const BlankSlide &action) const;
    BlankSlide getDummyAction() const;
    bool isDummyAction(const BlankSlide &action) const;

    /**
     * Sets the tile costs to one of the standard types.
     *
     * @param cost_type The cost type for the transitions.
     */
    void setStandardTileCostType(const TileCostType cost_type);

    /**
     * Sets the tile_costs to a custom cost type.
     *
     * The function takes in a list of double values, such that the value at
     * index i is the cost of moving tile i.
     *
     * @param tile_costs A list of the tile costs.
     */
    void setCustomTileCostType(const std::vector<double> &tile_costs);

    /**
     * Sets the properties of the problem, including a cost type selected from the standard options.
     *
     * @param rows The number of rows.
     * @param cols The number of columns.
     * @param cost_type The action cost type.
     */
    void setProperties(unsigned rows, unsigned cols, TileCostType cost_type = TileCostType::unit);

    /**
     * Sets the properties of the problem, including a custom tile cost type.
     *
     * @param rows The number of rows.
     * @param cols The number of columns.
     * @param tile_costs A list of numbers defining the cost of moving each tile.
     */
    void setProperties(unsigned rows, unsigned cols, const std::vector<double> &tile_costs);

    /**
     * Returns the cost of moving the specified tile.
     *
     * @param tile The tile to move.
     * @return The cost of moving the given tile.
     */
    double getTileMoveCost(unsigned tile) const;

protected:
    /**
     * Caches the set of actions applicable for each location of the blank.
     *
     * Actions are generated according to the operator ordering currently in use.
     *
     * @param rows The number of rows in the puzzle.
     * @param cols The number of columns in the puzzle.
     */
    void setActionList(unsigned rows, unsigned cols);

    /**
     * Checks if the given action is applicable given the current location of the blank.
     *
     * @param action The action to check.
     * @param blank_loc The location of the blank.
     * @return If the given action is applicable given where the blank currently is.
     */
    bool isApplicableInLocation(const BlankSlide &action, unsigned blank_loc) const;

    /**
     * Gets the identity of the tile being moved by the given action.
     *
     * Assumes the action is applicable in the given state.
     *
     * @param state The state in which the move is being applied.
     * @param action The action to apply.
     * @returns The identity of the tile being moved.
     */
    unsigned getMovingTile(const TilePuzzleState &state, const BlankSlide &action) const;

    unsigned num_rows; ///< Number of rows in the puzzle.
    unsigned num_cols; ///< Number of columns in the puzzle.

    std::vector<std::vector<BlankSlide> > loc_actions; ///< Caches the actions applicable for each blank location.
    std::vector<BlankSlide> ops_in_order; ///< An ordering of all the possible actions.
    std::vector<double> tile_move_cost; ///< Caches the cost of moving each tile.
};

/**
 * Defines an output representation tile puzzle actions.
 *
 * @param out The output stream.
 * @param dir The action to output.
 * @return A string representation of the given action.
 */
std::ostream& operator <<(std::ostream & out, const BlankSlide &dir);

#endif /* TILEPUZZLETRANSITIONS_H_ */
