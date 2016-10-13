/*
 * floating_point_compare.h
 *
 * LICENSE HERE
 *
 *  Created on: 2016-08-29
 *      Author: Rick Valenzano
 */

/**
 * Code for doing proper floating point comparisons.
 *
 * @file floating_point_utils.h
 */

#ifndef FLOATING_POINT_COMPARE_H_
#define FLOATING_POINT_COMPARE_H_

static const double TOLERANCE = 0.000001; ///< The amount of tolerance to allow for floating point comparisons.

//TODO Why not just make this
static const double ROOT_TWO = 1.414213562; ///< Precomputed static value for square root 2.

/**
 * Returns if a is less than b, given some tolerance due to floating point arithmetic.
 *
 * @param a The left-hand operator of "<"
 * @param b The right-hand operator of "<"
 * @return If the first number is less than the second.
 */
bool fp_less(double a, double b);

/**
 * Returns if a is greater than b, given some tolerance due to floating point arithmetic.
 *
 * @param a The left-hand operator of ">"
 * @param b The right-hand operator of ">"
 * @return If the first number is greater than the second.
 */
bool fp_greater(double a, double b);

/**
 * Returns if a is equal to b, given some tolerance due to floating point arithmetic.
 *
 * @param a The left-hand operator of "=="
 * @param b The right-hand operator of "=="
 * @return If the first number is equal to the second.
 */
bool fp_equal(double a, double b);

/**
 * Returns the minimum of a and b, given some tolerance due to floating point arithmetic.
 *
 * @param a The first value to take the minimum over.
 * @param b The second value to take the minimum over.
 * @return The minimum of the two numbers
 */
double fp_min(double a, double b);

/**
 * Returns the maximum of a and b, given some tolerance due to floating point arithmetic.
 *
 * @param a The first value to take the maximum over.
 * @param b The second value to take the maximum over.
 * @return The maximum of the two numbers
 */
double fp_max(double a, double b);

#endif /* FLOATING_POINT_COMPARE_H_ */
