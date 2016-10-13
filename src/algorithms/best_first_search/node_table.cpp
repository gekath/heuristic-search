/*
 * node_table.cpp
 *
 * LICENSE HERE
 *
 *  Created on: 2016-09-12
 *      Author: Rick Valenzano
 */

#include "node_table.h"

std::size_t NodeKeyHash::operator ()(StateHash hash_value) const
{
    return (std::size_t) (hash_value);
}
