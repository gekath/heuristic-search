/*
 * permutation_hash_function.h
 *
 * LICENSE HERE
 *
 *  Created on: 2016-09-23
 *      Author: Rick Valenzano
 */

#ifndef PERMUTATION_HASH_FUNCTION_H_
#define PERMUTATION_HASH_FUNCTION_H_

#include "state_hash_function.h"
#include "../utils/combinatorics.h"
#include <vector>

template<class state_t>
class PermutationHashFunction: public StateHashFunction<state_t>
{
public:
    PermutationHashFunction();
    virtual ~PermutationHashFunction();

    virtual StateHash getStateHash(const state_t &state) const;
};

template<class state_t>
inline PermutationHashFunction<state_t>::PermutationHashFunction()
{
}

template<class state_t>
inline PermutationHashFunction<state_t>::~PermutationHashFunction()
{
}

template<class state_t>
StateHash PermutationHashFunction<state_t>::getStateHash(const state_t& state) const
{
    std::vector<unsigned> permutation = state.permutation;

    StateHash hash_value = 0;
    unsigned num_left = permutation.size();
    for(unsigned i = 0; i < permutation.size(); i++) {
        hash_value += permutation[i] * get_64_bit_factorial(num_left - 1);
        num_left--;
        for(unsigned j = i + 1; j < permutation.size(); j++) {
            if(permutation[j] > permutation[i])
                permutation[j]--;
        }
    }
    return hash_value;
}

#endif /* PERMUTATION_HASH_FUNCTION_H_ */
