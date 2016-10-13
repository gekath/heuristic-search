/*
 * tile_puzzle_state.cpp
 *
 *  Created on: 2016-08-11
 *      Author: rick
 */

#include "tile_puzzle_state.h"

using std::cerr;
using std::vector;
using std::endl;
using std::ostream;

TilePuzzleState::TilePuzzleState() : num_rows(0), num_cols(0), blank_loc(0)
{
}

TilePuzzleState::~TilePuzzleState()
{
}


TilePuzzleState::TilePuzzleState(const unsigned rows, const unsigned cols)
        : permutation(rows * cols)
{
    num_rows = rows;
    num_cols = cols;

    for(unsigned i = 0; i < permutation.size(); i++) {
        permutation[i] = i;
    }

    blank_loc = 0;
}

TilePuzzleState::TilePuzzleState(const std::vector<unsigned>& perm, const unsigned rows, const unsigned cols)
        : permutation(perm), num_rows(rows), num_cols(cols), blank_loc(perm.size())
{
    if(permutation.size() != num_rows * num_cols) {
        cerr << "Error in SlidingTileState constructor.";
        cerr << "Permutation isn't the right size for given parameters";
        cerr << endl;
    }

    for(unsigned i = 0; i < permutation.size(); i++) {
        if(permutation[i] == 0) {
            blank_loc = i;
        }
    }
}

ostream& operator <<(std::ostream& out, const TilePuzzleState& s)
{
    out << "(" << s.num_rows << "x" << s.num_cols << ")-[";
    for(unsigned i = 0; i < s.permutation.size(); i++) {
        out << s.permutation[i];
        if(i > 1 && i < s.permutation.size() - 1 && (i + 1) % s.num_cols == 0)
            out << ",";
        if(i < s.permutation.size() - 1)
            out << " ";
    }
    out << "]";
    return out;
}

bool operator ==(const TilePuzzleState& s1, const TilePuzzleState& s2)
{
    if((s1.permutation.size() != s2.permutation.size()) || (s1.blank_loc != s2.blank_loc))
        return false;

    for(unsigned i = 0; i < s1.permutation.size(); i++) {
        if(s1.permutation[i] != s2.permutation[i])
            return false;
    }
    return true;
}

bool operator !=(const TilePuzzleState& s1, const TilePuzzleState& s2)
{
    return !(s1 == s2);
}
