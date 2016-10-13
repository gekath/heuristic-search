/*
 * string_utils.h
 *
 * LICENSE HERE
 *
 *  Created on: 2016-08-29
 *      Author: Rick Valenzano
 */
/**
 * Utilities for string manipulation.
 *
 * @file string_utils.h
 */

#ifndef STRING_UTILS_H_
#define STRING_UTILS_H_

#include <vector>
#include <sstream>
#include <iostream>
#include <stdlib.h>

/**
 * Converts the given integer to a string.
 *
 * @param i The integer to convert.
 * @return A string representation of the given integer.
 */
std::string int_to_string(int i);

/**
 * Converts the given double to a string.
 *
 * @param d The double to convert.
 * @return The string representation of the double.
 */
std::string double_to_string(double d);

/**
 * Splits the given string according into different tokens according to the given delimiter, and appends them to the given list.
 *
 * @param s The string to split.
 * @param tokens The list of tokens to store the split in.
 * @param delim The delimiter.
 */
void split(const std::string &s, std::vector<std::string> &tokens, char delim);

/**
 * Returns a string representation of the given vector.
 *
 * Assumes that the "<<" operator can be applied to the vector elements.
 * @param vec A vector of elements.
 * @return A string representation of the vector.
 */
template<class T>
std::string vec_to_string(const std::vector<T> &vec);

// Templated function implementations
template<class T>
std::string vec_to_string(const std::vector<T>& vec)
{
    std::stringstream s;
    s << "[";
    for(unsigned i = 0; i < vec.size(); i++) {
        s << vec[i];
        if(i != vec.size() - 1)
            s << " ";
    }
    s << "]";
    return s.str();
}

#endif /* STRING_UTILS_H_ */
