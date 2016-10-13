/*
 * open_closed_list.h
 *
 * LICENSE HERE
 *
 *  Created on: 2016-09-12
 *      Author: Rick Valenzano
 */

#ifndef OPEN_CLOSED_LIST_H_
#define OPEN_CLOSED_LIST_H_

#include "../../utils/floating_point_utils.h"
#include "node_table.h"
#include <stdio.h>
#include <iostream>

typedef unsigned BFSOpenLocation; ///< The location of a node in the open list heap.

/**
 * Defines the possible slocations for a state in the open list.
 *
 * Below is the meaning of each of the values:
 *
 * open - in the open list.\n
 * closed - in the closed list.\n
 * none - not currently stored in open or closed.\n
 *
 * @todo Is this enum really the best way to do this?
 * @class SearchStatus
 */
enum class StateLocation
{
    open, closed, none
};

/**
 * A class for a Best First Search node.
 *
 * @class BFSNode
 */
template<class state_t, class action_t>
class BFSNode
{
public:
    /**
     * A constructor for a BFSNode that sets the node attributes.
     *
     * The created node is assumed to be immediately put in the open list, and has not be reopened.
     *
     * @todo Make this more generic.
     *
     * @param node_state The state corresponding to the node.
     * @param parent The id of the parent of this node. Should be set to 0 if it has no parent.
     * @param action The action that generated this node from the corresponding parent.
     * @param g The g-cost of this node.
     * @param h The heuristic value of this node.
     * @param node_eval The evaluation of this node.
     * @param loc The location of this node in the open list.
     */
    BFSNode(const state_t &node_state, NodeID parent, const action_t &action, double g, double h, double node_eval,
            BFSOpenLocation loc);

    /**
     * Destructor for a best-first search node. Does nothing.
     */
    ~BFSNode();

    state_t state; ///< The state corresponding to this node.
    NodeID parent_id; ///< The id of the parent of this node.
    action_t gen_action; ///< A action used to generate this node.

    double g_cost; ///< The g-cost of the node.
    double h_value; ///< The heuristic value of the node.
    double eval; ///< The evaluation of the node.

    BFSOpenLocation location; ///< The location of the node in the open list heap. Means nothing if in_open is false.
    bool in_open; ///< If the node is in the open list or not.

    bool reopened; ///< If the node has been reopened.
};

/**
 * A class for an open and closed list to be used by a best-first search.
 *
 * @todo Make the NodeTable store pointers to BFSNode, so that it can creat things that inherit from them.
 * @todo Make more generic for OCL algorithm.
 *
 * @class OpenClosedList
 */
template<class state_t, class action_t>
class OpenClosedList
{
public:
    /**
     * Constructor for OpenClosedList. Does nothing.
     */
    OpenClosedList();

    /**
     * Destructor for OpenClosedList. Does nothing.
     */
    virtual ~OpenClosedList();

    /**
     * Clears the open closed list by emptying the table and heap.
     */
    void clear();

    /**
     * Returns whether the state is in the open list, closed list, or is not stored.
     *
     * @param state The state to check for.
     * @param hash_value The hash value of the state to check for.
     * @param id The id of the state if it is found to be stored.
     * @return The location of the given state.
     */
    StateLocation getStateLocation(const state_t &state, StateHash hash_value, NodeID &id);

    /**
     * Returns the node corresponding to the given id.
     *
     * @param id The id of the node to return.
     * @return The node corresponding to the given id.
     */
    BFSNode<state_t, action_t> &getNode(NodeID id);

    /**
     * Returns a const reference to a node corresponding to the given id.
     *
     * @param id The id of the node to return.
     * @return The node corresponding to the given id.
     */
    const BFSNode<state_t, action_t> &getNode(NodeID id) const;

    /**
     * Adds a new node with the given attributes to the open list.
     *
     * @param state The state being added.
     * @param action The action that generated this state.
     * @param hash_value The hash value of this state.
     * @param g The g-cost of the node.
     * @param h The heuristic value of the node.
     * @param node_eval The evaluation of the node.
     * @param parent The node id of the parent of this node, or 0 if this is the first node added.
     * @return The id of the node added.
     */
    virtual NodeID addNewNodeToOpen(const state_t &state, const action_t &action, StateHash hash_value, double g,
            double h, double node_eval, NodeID parent);

    virtual NodeID addLowGToOpen(const state_t &state, const action_t &action, StateHash hash_value, double g,
            double h, double node_eval, NodeID parent);
    virtual NodeID addHighGToOpen(const state_t &state, const action_t &action, StateHash hash_value, double g,
            double h, double node_eval, NodeID parent);
    /**
     * Adds the initial node with the given attributes to the open list.
     *
     * @param state The state being added.
     * @param action The action that generated this state.
     * @param hash_value The hash value of this state.
     * @param h The heuristic value of the node.
     * @param node_eval The evaluation of the node.
     * @return The id of the node added.
     */
    virtual NodeID addInitialNodeToOpen(const state_t &state, const action_t &action, StateHash hash_value,
            double h, double node_eval);

    /**
     * Finds the best node in the open list according to the node evaluation, returns its id, and moves it to closed.
     *
     * @return The node id of the best node on the open list.
     */
    virtual NodeID getBestNodeAndClose();

    /**
     * Adjusts the open list as needed because the node evaluation of the node with the given id has changed.
     *
     * @param id The id of the node whose evaluation has changed.
     */
    virtual void openNodeEvalChanged(NodeID id);

    /**
     * Reopens the node with the given id.
     *
     * @param id The id of the node to reopen.
     */
    virtual void reopenNode(NodeID id);

    /**
     * Returns the number of nodes in the open list.
     *
     * @return The size of the open list.
     */
    std::size_t openListSize() const;

    /**
     * Returns the number of nodes in the closed list.
     *
     * @return The size of the closed list.
     */
    std::size_t closedListSize() const;

    /**
     * Returns the total number of nodes being stored.
     *
     * @return The number of nodes being stored.
     */
    std::size_t size() const;

    /**
     * Returns if the open list is empty or not.
     *
     * @return If the open list is empty or not.
     */
    bool isOpenEmpty() const;

protected:
    /**
     * Returns true if the evaluation of the first node is less than or equal to the second.
     *
     * @param node_1 The first node being compared.
     * @param node_2 The second node being compared.
     * @return If the evaluation of the first node is less than or equal to the second.
     */
    virtual bool nodeNoWorse(const BFSNode<state_t, action_t> &node_1,
            const BFSNode<state_t, action_t> &node_2) const;

    /**
     * Returns true if the evaluation of the node in the open list at the first location is less than or equal to the second.
     *
     * @param loc_1 The location in the open list of the first node being compared.
     * @param loc_2 The location in the open list of the second node being compared.
     * @return If the evaluation of the first node is less than or equal to the second.
     */
    virtual bool nodeNoWorse(BFSOpenLocation loc_1, BFSOpenLocation loc_2) const;

    /**
     * Heapify's up the node at the given open list location if it needs to be moved up.
     *
     * @param loc The location to heapify up from.
     * @return If the node at the given location has been heapified up at least one level in the heap.
     */
    bool heapifyUp(BFSOpenLocation loc);

    /**
     * Heapify's down the node at the given open list location if it needs to be moved down.
     *
     * @param loc The location to heapify down from.
     * @return If the node at the given location has been heapified down at least one level in the heap.
     */
    bool heapifyDown(BFSOpenLocation loc);

    /**
     * Swaps the elements in their locations in the heaps at the given locations, and updates the nodes accordingly.
     *
     * @param loc_1 The first location in the heap.
     * @param loc_2 The second location in the heap.
     */
    void swapOpenLocations(BFSOpenLocation loc_1, BFSOpenLocation loc_2);

    void printOpen();

    NodeTable<BFSNode<state_t, action_t> > node_table; ///< The table of nodes being stored.
    std::vector<NodeID> open_list_heap; ///< The heap holding node ids representing the open list.
};

template<class state_t, class action_t>
BFSNode<state_t, action_t>::BFSNode(const state_t& node_state, NodeID parent, const action_t& action, double g,
        double h, double node_eval, BFSOpenLocation loc)
        : state(node_state), parent_id(parent), gen_action(action), g_cost(g), h_value(h), eval(node_eval),
                location(loc), in_open(true), reopened(false)
{
}

template<class state_t, class action_t>
BFSNode<state_t, action_t>::~BFSNode()
{
}

template<class state_t, class action_t>
inline OpenClosedList<state_t, action_t>::OpenClosedList()
{
}

template<class state_t, class action_t>
inline OpenClosedList<state_t, action_t>::~OpenClosedList()
{
}

template<class state_t, class action_t>
void OpenClosedList<state_t, action_t>::clear()
{
    node_table.clear();
    open_list_heap.clear();
}

template<class state_t, class action_t>
StateLocation OpenClosedList<state_t, action_t>::getStateLocation(const state_t& state, StateHash hash_value,
        NodeID& id)
{
    bool is_stored = node_table.isNodeStored(hash_value, id);

    if(is_stored) {
        if(node_table.getNode(id).in_open)
            return StateLocation::open;
        return StateLocation::closed;
    }
    return StateLocation::none;
}

template<class state_t, class action_t>
NodeID OpenClosedList<state_t, action_t>::addNewNodeToOpen(const state_t& state, const action_t &action,
        StateHash hash_value, double g, double h, double node_eval, NodeID parent)
{
    NodeID new_id = node_table.addNewSearchNode(
            BFSNode<state_t, action_t>(state, parent, action, g, h, node_eval, open_list_heap.size()), hash_value);

    //std::cout << "New ID " << new_id << std::endl;
    //std::cout << "Node Table Size " << node_table.size() << std::endl;

    open_list_heap.push_back(new_id);

    heapifyUp(open_list_heap.size() - 1);

    //printOpen();
    //std::cout << std::endl;

    return new_id;
}

template<class state_t, class action_t>
NodeID OpenClosedList<state_t, action_t>::addLowGToOpen(const state_t& state, const action_t &action,
        StateHash hash_value, double g, double h, double node_eval, NodeID parent)
{
    NodeID new_id = node_table.addNewSearchNode(
            BFSNode<state_t, action_t>(state, parent, action, g, h, node_eval, open_list_heap.size()), hash_value);

    //std::cout << "New ID " << new_id << std::endl;
    //std::cout << "Node Table Size " << node_table.size() << std::endl;

    std::vector<unsigned int>::reverse_iterator it;

    for (it = open_list_heap.rbegin(); it != open_list_heap.rend(); ++it) {

        unsigned int i = it.base() - open_list_heap.begin() - 1;
        std::cout << "eval" << i << "\t" << getNode(open_list_heap[i]).eval << "\t" << node_eval << std::endl;
        std::cout << "g" << i << "\t" << getNode(open_list_heap[i]).g_cost << "\t" << g << std::endl;


        // Iterate from the end of the list. Appending new costs, should be at least the cost of 
        // the last item in the open list. 

        // If not equal, just append to the end
        if (!fp_equal(getNode(open_list_heap[i]).eval, node_eval)) {

            // std::cout << it.base() << std::endl;
            break;
        } else if (fp_less(g, getNode(open_list_heap[i]).g_cost)){
            // std::cout << "here" << std::endl;
            break;
        }
    }

    open_list_heap.insert(it.base(), new_id);

    heapifyUp(open_list_heap.size() - 1);

    //printOpen();
    //std::cout << std::endl;

    return new_id;
}

template<class state_t, class action_t>
NodeID OpenClosedList<state_t, action_t>::addHighGToOpen(const state_t& state, const action_t &action,
        StateHash hash_value, double g, double h, double node_eval, NodeID parent)
{
    NodeID new_id = node_table.addNewSearchNode(
            BFSNode<state_t, action_t>(state, parent, action, g, h, node_eval, open_list_heap.size()), hash_value);

    //std::cout << "New ID " << new_id << std::endl;
    //std::cout << "Node Table Size " << node_table.size() << std::endl;

    std::vector<unsigned int>::reverse_iterator it;
    it = open_list_heap.rbegin();

    for (it = open_list_heap.rbegin(); it != open_list_heap.rend(); ++it) {

        unsigned int i;
        i = it.base() - open_list_heap.begin() - 1;
        std::cout << i << std::endl;

        if (fp_equal(getNode(open_list_heap[i]).eval, node_eval) && fp_greater(g, getNode(open_list_heap[i]).g_cost)){
            break;
        }
    }

    open_list_heap.insert(it.base(), new_id);
    
    heapifyUp(open_list_heap.size() - 1);

    //printOpen();
    //std::cout << std::endl;

    return new_id;
}

template<class state_t, class action_t>
inline std::size_t OpenClosedList<state_t, action_t>::openListSize() const
{
    return open_list_heap.size();
}

template<class state_t, class action_t>
inline std::size_t OpenClosedList<state_t, action_t>::closedListSize() const
{
    return size() - openListSize();
}

template<class state_t, class action_t>
inline std::size_t OpenClosedList<state_t, action_t>::size() const
{
    return node_table.size();
}

template<class state_t, class action_t>
bool OpenClosedList<state_t, action_t>::heapifyUp(BFSOpenLocation loc)
{
    assert(loc < open_list_heap.size());
    if(loc == 0)
        return false;

    BFSOpenLocation parent_loc = (loc - 1) / 2;

    if(!nodeNoWorse(parent_loc, loc)) {
        swapOpenLocations(loc, parent_loc);

        heapifyUp(parent_loc);
        return true;
    }
    return false;
}

template<class state_t, class action_t>
inline bool OpenClosedList<state_t, action_t>::nodeNoWorse(const BFSNode<state_t, action_t>& node_1,
        const BFSNode<state_t, action_t>& node_2) const
{
    return !fp_greater
    (node_1.eval, node_2.eval);
}

template<class state_t, class action_t>
inline bool OpenClosedList<state_t, action_t>::nodeNoWorse(BFSOpenLocation loc_1, BFSOpenLocation loc_2) const
{
    return nodeNoWorse(getNode(open_list_heap[loc_1]), getNode(open_list_heap[loc_2]));
}

template<class state_t, class action_t>
void OpenClosedList<state_t, action_t>::openNodeEvalChanged(NodeID id)
{
    if(!heapifyUp(node_table[id].location))
        heapifyDown(heapifyUp(node_table[id].location));
}

template<class state_t, class action_t>
void OpenClosedList<state_t, action_t>::reopenNode(NodeID id)
{
    assert(node_table[id].in_open == false);
    node_table[id].in_open = true;
    node_table[id].reopened = true;
    node_table[id].location = open_list_heap.size();
    open_list_heap.push_back(id);

    heapifyUp(node_table[id].location);
}

template<class state_t, class action_t>
NodeID OpenClosedList<state_t, action_t>::getBestNodeAndClose()
{
    assert(!open_list_heap.empty());

    NodeID best_id = open_list_heap[0];
    node_table[best_id].in_open = false;

    open_list_heap[0] = open_list_heap.back();
    node_table.getNode(open_list_heap[0]).location = 0;
    open_list_heap.pop_back();

    heapifyDown(0);

    return best_id;
}

// template<class state_t, class action_t>
// NodeID OpenClosedList<state_t, action_t>::getLowGAndClose()
// {

//     // Keep track of indices which have same h-value
//     unsigned int i;
//     std::vector<int> tie_list;

//     assert(!open_list_heap.empty());

//     for (i = 0; i < open_list_heap.size()-1; i++) {

//         if (open_list_heap[i] == open_list_heap[i+1]) {
//             tie_list.push_back(i);
//         }

//     }

//     NodeID best_id = open_list_heap[0];
//     node_table[best_id].in_open = false;

//     open_list_heap[0] = open_list_heap.back();
//     node_table.getNode(open_list_heap[0]).location = 0;
//     open_list_heap.pop_back();

//     heapifyDown(0);

//     std::cout << getNode(best_id).eval << std::endl;

//     return best_id;
// }

template<class state_t, class action_t>
inline BFSNode<state_t, action_t>& OpenClosedList<state_t, action_t>::getNode(NodeID id)
{
    return node_table[id];
}

template<class state_t, class action_t>
inline const BFSNode<state_t, action_t>& OpenClosedList<state_t, action_t>::getNode(NodeID id) const
{
    return node_table[id];
}

template<class state_t, class action_t>
inline NodeID OpenClosedList<state_t, action_t>::addInitialNodeToOpen(const state_t& state, const action_t& action,
        StateHash hash_value, double h, double node_eval)
{
    return addNewNodeToOpen(state, action, hash_value, 0.0, h, node_eval, 0);
}

template<class state_t, class action_t>
inline bool OpenClosedList<state_t, action_t>::isOpenEmpty() const
{
    return open_list_heap.size() == 0;
}

template<class state_t, class action_t>
bool OpenClosedList<state_t, action_t>::heapifyDown(BFSOpenLocation loc)
{
    BFSOpenLocation left_child_loc = loc * 2 + 1;
    BFSOpenLocation right_child_loc = loc * 2 + 2;
    BFSOpenLocation best_child_loc;

    if(left_child_loc >= open_list_heap.size())
        return false;
    else if(right_child_loc >= open_list_heap.size() || nodeNoWorse(left_child_loc, right_child_loc))
        best_child_loc = left_child_loc;
    else
        best_child_loc = right_child_loc;

    if(!nodeNoWorse(loc, best_child_loc)) {
        swapOpenLocations(loc, best_child_loc);

        heapifyDown(best_child_loc);
        return true;
    }

    return false;
}

template<class state_t, class action_t>
inline void OpenClosedList<state_t, action_t>::swapOpenLocations(BFSOpenLocation loc_1, BFSOpenLocation loc_2)
{
    NodeID id = open_list_heap[loc_1];
    open_list_heap[loc_1] = open_list_heap[loc_2];
    open_list_heap[loc_2] = id;

    node_table.getNode(open_list_heap[loc_1]).location = loc_1;
    node_table.getNode(open_list_heap[loc_2]).location = loc_2;
}

template<class state_t, class action_t>
void OpenClosedList<state_t, action_t>::printOpen()
{
    for(unsigned i = 0; i < open_list_heap.size(); i++) {
        std::cout << "ID " << open_list_heap[i] << ", eval " << getNode(open_list_heap[i]).eval << std::endl;
    }
}

#endif /* OPEN_CLOSED_LIST_H_ */
