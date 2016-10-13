/*
 * combinatorics.h
 *
 * LICENSE HERE
 *
 *  Created on: 2016-08-29
 *      Author: Rick Valenzano
 */
/**
 * A set of combinatorial functions.
 *
 * @todo Figure out proper way to get random numbers.
 * @todo Add something that checks to make sure read permutations are proper permutations.
 * @file combinatorics.h
 */
#ifndef COMBINATORICS_H_
#define COMBINATORICS_H_

#include <vector>
#include <stdlib.h>
#include <cstdint>
#include <string>

/**
 * Given a set (or combination) of natural numbers, generates the next combination in the lexicographic ordering.
 *
 * This function allows the user to iterate through all combinations of sets of k natural numbers. Given such
 * a combination as a vector of sorted numbers, it generates the next combination if one exists and returns
 * true. The combination is also returned as a sorted vector. If no such combination exists, it returns false
 * and does not change the combination.
 *
 * Note that the first vector in the ordering is [0, 1, ..., k - 1] and the last is [n - k, n - 2, n - 1].
 *
 * The first vector in the ordering is generated if the given combination does not have the correct number
 * of elements or the optional flag is set to true. Otherwise, the functions assumes it is given a sorted
 * vector representing a valid combination of k of the first n natural numbers.
 *
 * @param combo The last combination generated.
 * @param k The size of each combination.
 * @param n The size of the set of natural numbers to select from.
 * @param first_combo Whether to generate the first combination.
 * @return If a new combination has been generated, or if it has run out of combinations.
 */
bool get_next_combo(std::vector<unsigned> &combo, unsigned k, unsigned n, bool first_combo = false);

/**
 * Randomly reorders the elements in the given vector.
 *
 * @param vec The vector to randomly reorder.
 */
template<class T>
void randomly_reorder(std::vector<T> &vec);

/**
 * Returns a random permutation of a subset of the natural numbers.
 *
 * The given vector is reset before the permutation is stored there.
 *
 * @param perm The vector to store the random permutation in.
 * @param size The size of the permutation.
 */
void get_rand_permutation(std::vector<unsigned> &perm, unsigned size);

/**
 * Returns n! for the given value of n.
 *
 * If n is too large, it just returns the highest value stored in 64 bits.
 *
 * @param n The value to take the factorial of.
 * @return The factorial value of the given value.
 */
uint64_t get_64_bit_factorial(unsigned n);

/**
 * Returns the value of n!/k! for the given integers.
 *
 * No guarantees on what happens if there is an overflow.
 * @todo Figure out what should happen when an overflow happens.
 *
 * @param n The denominator of the n!/k! term.
 * @param k The numerator of the n!/k! term.
 * @return The value of n!/k! for the given integers.
 */
uint64_t get64BitnUpperk(unsigned n, unsigned k);

/**
 * Reads in the list of permutations from the given file.
 *
 *
 * @param file_name The file name to read the states in from.
 * @param perms The list that the read permutations are appended to.
 * @return If the file reading succeeded or not.
 */
bool read_in_permutations(std::string file_name, std::vector<std::vector<unsigned> > &perms);

// Templated function implementations
template<class T>
void randomly_reorder(std::vector<T>& vec)
{
    unsigned pos = vec.size() - 1;
    // construct new permutation
    while(pos > 0) {
        int r = random();
        int index = r % (pos + 1);

        T temp = vec[index];
        vec[index] = vec[pos];
        vec[pos] = temp;

        pos--;
    }
}

#endif /* COMBINATORICS_H_ */
