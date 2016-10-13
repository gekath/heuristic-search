/*
 * string_utils.cpp
 *
 * LICENSE HERE
 *
 *  Created on: 2016-08-29
 *      Author: Rick Valenzano
 */

#include "string_utils.h"

using std::string;
using std::vector;
using std::stringstream;
using std::getline;

string int_to_string(int i)
{
    stringstream s;
    s << i;
    return s.str();
}

string double_to_string(double i)
{
    stringstream s;
    s << i;
    return s.str();
}

void split(const string &s, std::vector<std::string> &tokens, char delim)
{
    stringstream ss(s);
    string item;
    while(getline(ss, item, delim)) {
        tokens.push_back(item);
    }
}
