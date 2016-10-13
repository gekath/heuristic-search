/*
 * tile_puzzle_state.h
 *
 * LICENSE HERE
 *
 *  Created on: 2016-08-11
 *      Author: Rick Valenzano
 */

#ifndef TILEPUZZLESTATE_H_
#define TILEPUZZLESTATE_H_

#include <vector>
#include <iostream>

/**
 * Defines a tile puzzle state.
 *
 * @todo Is both equality and inequality actually needed?
 *
 * @class TilePuzzleState
 */
class TilePuzzleState
{
public:
    /**
     * Creates an empty tile puzzle with zero in all dimensions.
     */
    TilePuzzleState();

    /**
     * Destructor for a sliding tile puzzle state. Does nothing.
     */
    virtual ~TilePuzzleState();

    /**
     * Creates a tile puzzle of the given dimensions.
     *
     * @param rows The number of rows in the puzzle.
     * @param cols The number of columns in the puzzle.
     */
    TilePuzzleState(const unsigned rows, const unsigned cols);

    /**
     * Creates a tile puzzle from the given permutation with the given dimensions.
     *
     * @param perm The permutation as the basis of the puzzle.
     * @param rows The number of rows in the puzzle.
     * @param cols The number of columns in the puzzle.
     */
    TilePuzzleState(const std::vector<unsigned> &perm, const unsigned rows, const unsigned cols);

    std::vector<unsigned> permutation; ///< The permutation representation of the state.

    unsigned num_rows; ///< Number of rows in the state.
    unsigned num_cols; ///< Number of columns in the state.

    unsigned blank_loc; ///< Location of the blank (or 0)
};

/**
 * Outputs a string representation of the sliding tile puzzle state to the given output stream.
 *
 * @param out The output stream.
 * @param s The puzzle to output.
 * @return The output stream.
 */
std::ostream& operator <<(std::ostream & out, const TilePuzzleState &s);

/**
 * Defines equality of two puzzle states.
 *
 * @param s1 The first state to test.
 * @param s2 The second state to test.
 * @return If the states are equal or not.
 */
bool operator ==(const TilePuzzleState &s1, const TilePuzzleState &s2);

/**
 * Defines inequality for puzzle states.
 *
 * @param s1 The first state to compare.
 * @param s2 The second state to compare.
 * @return If the states are not equal.
 */
bool operator !=(const TilePuzzleState &s1, const TilePuzzleState &s2);

#endif /* TILEPUZZLESTATE_H_ */
