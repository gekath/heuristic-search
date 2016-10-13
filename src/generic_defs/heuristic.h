/*
 * heuristic.h
 *
 * LICENSE HERE
 *
 *  Created on: 2016-08-23
 *      Author: Rick Valenzano
 */

#ifndef HEURISTIC_H_
#define HEURISTIC_H_

/**
 * Abstract class for defining a heuristic function.
 *
 * Descendents should overwrite calculateH. Negative heuristic values should be interpreted as
 * indicating that the problem is unsolvable from the current state. The intended way to calculate
 * a heuristic will then be call prepareToCompute, and then computeHeuristic.
 *
 * @class Heuristic
 */
template<class state_t>
class Heuristic
{
public:
    /**
     * Constructor for the abstract heuristic class. Abstract constructor only initializes the last heuristic value to be 0.0.
     */
    Heuristic();

    /**
     * Destructor for the abstract heuristic class. Abstract destructor is empty.
     */
    virtual ~Heuristic();

    /**
     * Prepares the heuristic function for computing the next heuristic value.
     *
     * This function is intended to allow for having multiple heuristics computed
     * simultaneously using the same process.
     */
    virtual void prepareToCompute();

    /**
     * Gets and returns the heuristic value of the given state.
     *
     * To get the heuristic value, calls computeHValue.
     * If the heuristic value is negative, the heuristic function is indicating unsolvability.
     *
     * @param state The state whose heuristic value is to be calculated.
     * @return The heuristic value of the given state.
     */
    double getHValue(const state_t &state);

    /**
     * Returns the last heuristic value computed.
     *
     * If the heuristic value is negative, the heuristic function is indicating unsolvability.
     *
     * @return The last heuristic value computed.
     */
    double getLastHValue() const;

    /**
     * Returns if a heuristic value is currently stored for this heuristic function.
     *
     * @return If a valid heuristic value is currently stored.
     */
    bool isHValueStored() const;

protected:
    /**
     * Calculates the heuristic value of the given state. Does not store the heuristic value.
     *
     * If the heuristic value is negative, the heuristic function is indicating unsolvability.
     *
     * This is the main function to be overloaded.
     *
     * @param state The state whose heuristic value is to be calculated.
     * @return The heuristic value of the given state.
     */
    virtual double computeHValue(const state_t &state) const = 0;

private:
    double last_h; ///< The last heuristic value calculated.

    bool h_stored; ///< Is the heuristic value currently stored.
};

template<class state_t>
Heuristic<state_t>::Heuristic()
        : last_h(0.0), h_stored(false)
{
}

template<class state_t>
Heuristic<state_t>::~Heuristic()
{
}

template<class state_t>
double Heuristic<state_t>::getHValue(const state_t& state)
{
    last_h = computeHValue(state);

    h_stored = true;
    return last_h;
}

template<class state_t>
inline double Heuristic<state_t>::getLastHValue() const
{
    return last_h;
}

template<class state_t>
void Heuristic<state_t>::prepareToCompute()
{
    last_h = -1.0;
    h_stored = false;
}

template<class state_t>
inline bool Heuristic<state_t>::isHValueStored() const
{
    return h_stored;
}

#endif /* HEURISTIC_H_ */
