/*
 * combinatorics.cpp
 *
 * LICENSE HERE
 *
 *  Created on: 2016-08-29
 *      Author: Rick Valenzano
 */

#include "combinatorics.h"
#include "string_utils.h"
#include <cassert>
#include <sstream>
#include <fstream>
#include <iostream>

using std::vector;
using std::string;
using std::cerr;
using std::ifstream;

// Helper Functions
/**
 * Helper function for get_next combo.
 *
 * Given a combination, a position, and whether we are at a reset, finds the next combo if one
 * exists. This means incrementing the element at the given position if possible, and then resetting
 * the remaining values to the smallest values left. If its not possible, it returns false.
 *
 * @param combo The current combo which is updated to get the next combo.
 * @param max_num The maximum element value.
 * @param pos The current position.
 * @param reset_rest Whether the combo should be reset starting from the given position.
 * @return If we successfully found the next combination.
 */
bool get_next_combo_helper(vector<unsigned> &combo, unsigned max_num, unsigned pos, bool reset_rest);

static uint64_t factorial_table[21] = {1ll, 1ll, 2ll, 6ll, 24ll, 120ll, 720ll, 5040ll, 40320ll, 362880ll, 3628800ll,
        39916800ll, 479001600ll, 6227020800ll, 87178291200ll, 1307674368000ll, 20922789888000ll, 355687428096000ll,
        6402373705728000ll, 121645100408832000ll, 2432902008176640000ll};

// Function implementations
bool get_next_combo(vector<unsigned> &combo, unsigned k, unsigned n, bool first_combo)
{
    assert(n >= k);

    if(first_combo || combo.size() != k) { // need to initialize first combo
        combo.resize(k);

        for(unsigned i = 0; i < k; i++) {
            combo[i] = i;
        }
        return true;
    }
    assert(n >= combo.size() - 1);

    return get_next_combo_helper(combo, n - 1, 0, false); // call helper function to get next combo
}

bool get_next_combo_helper(vector<unsigned> &combo, unsigned max_num, unsigned pos, bool reset_rest)
{
    assert(max_num >= combo.size() - 1);

    if(reset_rest) { // need to reset rest of the combo to smallest values possible starting from current position
        if(pos == 0) { // if this is the first position, reset the whole combo
            combo[0] = 0;
            return get_next_combo_helper(combo, max_num, pos + 1, true);
        }

        assert(combo[pos - 1] + combo.size() - pos <= max_num); // make sure enough positions for remaining elements

        combo[pos] = combo[pos - 1] + 1; // assign the smallest remaining element

        if(pos == combo.size() - 1) // if finished the combo, return it was successful
            return true;

        return get_next_combo_helper(combo, max_num, pos + 1, true); // move to resetting next value
    }

    // if have hit a number that is too high, must backtrack and replace
    if(combo[pos] + combo.size() - pos - 1 == max_num) { // have run out of elements, and must backtrack
        return false;
    } else if(pos == combo.size() - 1) { // at last position and larger elements remaining
        combo[pos]++;
        return true;
    }

    if(!get_next_combo_helper(combo, max_num, pos + 1, false)) { // try to increase elements in remainder of combo
        combo[pos]++; // if can't, have to update the current one first
        return get_next_combo_helper(combo, max_num, pos + 1, true); // reset remainder accordingly
    }

    return true;
}

void get_rand_permutation(vector<unsigned>& perm, unsigned perm_size)
{
    perm.resize(perm_size);

    for(unsigned i = 0; i < perm_size; i++)
        perm[i] = 0;

    randomly_reorder<unsigned>(perm);
}

uint64_t get_64_bit_factorial(unsigned n)
{
    if(n > 20)
        return (uint64_t) -1;
    return factorial_table[n];
}

uint64_t get64BitnUpperk(unsigned n, unsigned k)
{
    if(n <= k)
        return 1;
    if(n <= 20)
        return factorial_table[n] / factorial_table[k];

    uint64_t value = 1;
    assert(n >= 0 && k >= 0);

    for(unsigned i = n; i > k; i--) {
        value *= (uint64_t) i;
    }

    return value;
}

bool read_in_permutations(string file_name, vector<vector<unsigned> >& perms)
{
    ifstream in_stream(file_name, std::ios::in);

    if(in_stream.fail()) {
        cerr << "Could not read in map problem file " << file_name << ".\n";
        return false;
    }

    string new_line;
    vector<string> tokens;
    vector<unsigned> perm;

    while(getline(in_stream, new_line)) {
        tokens.clear();
        split(new_line, tokens, ' ');

        perm.clear();
        for(unsigned i = 0; i < tokens.size(); i++) {
            unsigned value = atoi(tokens[i].c_str());
            perm.push_back(value);
        }
        //TODO Add permutation checking here

        perms.push_back(perm);
    }
    return true;
}
