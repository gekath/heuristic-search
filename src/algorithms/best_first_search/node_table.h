/*
 * node_table.h
 *
 * LICENSE HERE
 *
 *  Created on: 2016-09-12
 *      Author: Rick Valenzano
 */

#ifndef NODE_TABLE_H_
#define NODE_TABLE_H_

#include <cassert>
#include <vector>
#include <unordered_map>
#include <utility>
#include <stdio.h>
#include <iostream>
#include "../../generic_defs/state_hash_function.h"

typedef unsigned NodeID; ///< The ID of a node is the location of the node in the node table.

/**
 * A struct that turns a given hash value and turns it into something an unordered_map can use.
 *
 * @todo Why does this only compile if I define the function here? Inconsistent with everything else.
 * @struct NodeKeyHash
 */
struct NodeKeyHash
{
    std::size_t operator()(StateHash hash_value) const;
};

/**
 * A table of search nodes that maintains a hash map that can be used to tell if items are already held in the table.
 *
 * The node type is a a template type, and so can be anything. It uses the given hash value to determine if the item
 * is already there. The ID of a node is the location in the table.
 *
 * @todo Change the NodeKeyHash so that we can check the state if the hash function isn't injective.
 *
 * @class NodeTable
 */
template<class node_t>
class NodeTable
{
    typedef std::unordered_map<StateHash, NodeID, NodeKeyHash> NodeMap; ///< Defines the type for a map.

public:
    /**
     * The constructor for the node table. Does nothing.
     */
    NodeTable();

    /**
     * Destructor for the node table. Does nothing.
     */
    virtual ~NodeTable();

    /**
     * Returns whether the node with the given hash value is already stored.
     *
     * The ID is set to the stored node id if it is already stored.
     *
     * @param hash_value The hash value searching for.
     * @param id The id of the node is set if it is found.
     * @return If there is already a node associated with the given hash value.
     */
    bool isNodeStored(StateHash hash_value, NodeID &id) const;

    /**
     * Adds the given search node to the node table.
     *
     * Assumes the hash value is not already associated with a node.
     *
     * @param new_node The new node to store.
     * @param hash_value The hash value to associate the node with.
     * @return The ID of the added node.
     */
    NodeID addNewSearchNode(node_t new_node, StateHash hash_value);

    /**
     * Clears the node table.
     */
    void clear();

    /**
     * Returns the number of nodes in the node table.
     *
     * @return Numb
     */
    std::size_t size() const;

    /**
     * Returns a reference to the node with the given id.
     *
     * Does not check if no such node exists.
     *
     * @param node_id The id of the node to return.
     * @return A reference to the node to return.
     */
    node_t &getNode(NodeID node_id);

    const node_t &getNode(NodeID node_id) const;

    /**
     * Returns a reference to the node with the given id.
     *
     * Does not check if no such node exists.
     *
     * @param node_id The id of the node to return.
     * @return A refernce to the node with the given id.
     */
    node_t &operator[](NodeID node_id);

    const node_t &operator[](NodeID node_id) const;

protected:
    std::vector<node_t> nodes; ///< A list of the nodes being stored.

    NodeMap node_map; ///< The map used to determine if a hash value is already associated with a node.
};

template<class node_t>
inline NodeTable<node_t>::NodeTable()
{
}

template<class node_t>
inline NodeTable<node_t>::~NodeTable()
{
}

template<class node_t>
bool NodeTable<node_t>::isNodeStored(StateHash hash_value, NodeID& id) const
{
    assert(nodes.size() == node_map.size());

    typename NodeMap::const_iterator node_check = node_map.find(hash_value);

    if(node_check == node_map.end())
        return false;

    id = node_check->second;
    assert(id < nodes.size());
    return true;
}

template<class node_t>
NodeID NodeTable<node_t>::addNewSearchNode(node_t new_node, StateHash hash_value)
{
    nodes.push_back(new_node);
    node_map[hash_value] = nodes.size() - 1;

    return nodes.size() - 1;
}

template<class node_t>
void NodeTable<node_t>::clear()
{
    nodes.clear();
    node_map.clear();
}

template<class node_t>
node_t& NodeTable<node_t>::getNode(NodeID node_id)
{
    if(node_id >= nodes.size()) {
        //std::cout << "Bad node id " << node_id << std::endl;
        assert(false);
    }
    return nodes[node_id];
}

template<class node_t>
const node_t& NodeTable<node_t>::getNode(NodeID node_id) const
{
    if(node_id >= nodes.size()) {
        //std::cout << "Bad node id " << node_id << std::endl;
        assert(false);
    }
    return nodes[node_id];
}

template<class node_t>
inline std::size_t NodeTable<node_t>::size() const
{
    return nodes.size();
}

template<class node_t>
node_t& NodeTable<node_t>::operator [](NodeID node_id)
{
    return getNode(node_id);
}

template<class node_t>
const node_t& NodeTable<node_t>::operator [](NodeID node_id) const
{
    return getNode(node_id);
}

#endif /* NODE_TABLE_H_ */
