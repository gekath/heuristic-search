/*
 * state_hash_function.h
 *
 * LICENSE HERE
 *
 *  Created on: 2016-09-12
 *      Author: Rick Valenzano
 */

#ifndef STATE_HASH_FUNCTION_H_
#define STATE_HASH_FUNCTION_H_
#include <cstdint>

typedef uint64_t StateHash; ///< The hash value of a state.

/**
 * An abstract class defining a hash function for states.
 *
 * @class StateHashFunction
 */
template<class state_t>
class StateHashFunction
{
public:
    /**
     * Constructor for the abstract hash function. Abstract constructor is empty.
     */
    StateHashFunction();

    /**
     * Destructor for the abstract hash function. Abstract destructor is empty.
     */
    virtual ~StateHashFunction();

    /**
     * Returns the hash value of the given state.
     *
     * @param state The state whose hash value is needed.
     * @return The hash value of the given state.
     */
    virtual StateHash getStateHash(const state_t &state) const = 0;
};

template<class state_t>
StateHashFunction<state_t>::StateHashFunction()
{
}

template<class state_t>
StateHashFunction<state_t>::~StateHashFunction()
{
}

#endif /* STATE_HASH_FUNCTION_H_ */
