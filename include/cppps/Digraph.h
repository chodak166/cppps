// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.

#ifndef DIGRAPH_H
#define DIGRAPH_H

#include <functional>
#include <list>
#include <set>
#include <map>

namespace cppps {

class DuplicatedNodeException: public std::runtime_error {
  using runtime_error::runtime_error;
};

class NoSuchNodeException: public std::runtime_error {
  using runtime_error::runtime_error;
};


/**
 * @brief Directed graph utility class.
 *
 * Class is used as a container for objects of
 * templated T type. A key of type K is used to identify
 * nodes and to acces the data. A KeyProvider (functor)
 * is used to generate unique key. In most cases a
 * KeyProvider will be a simple lambda returning the
 * T member value that identifies the object.
 */
template <class K, class T>
class Digraph
{
public:
  using KeyProvider = std::function<K (const T& data)>;
  using SortedNodes = std::list<T>;
  using Cycle = std::list<T>;
  using Cycles = std::list<Cycle>;

  Digraph(KeyProvider keyProvider);

  virtual ~Digraph() = default;

  /**
   * @brief Add new node to the graph.
   *
   * Adding a node with the same key twice
   * will result in throwing an exception.
   *
   * @param data Node data object
   */
  void addNode(T data);

  /**
   * @brief Get node reference by its key
   * @param key Node identifier
   * @return Node data object reference
   */
  T& getNode(const K& key);

  /**
   * @brief Add an edge between nodes identified by given keys
   * @param startNode Tail node
   * @param endNode Head node
   */
  void addEdge(const K& startNode, const K& endNode);

  /**
   * @brief Add an edge between existing nodes
   * @param startNode Tail node
   * @param endNode Head node
   */
  void addEdge(const T& startNode, const T& endNode);

  /**
   * @brief Perform topological sort of collected nodes.
   * @return Topologically soorted list of copied T elements.
   */
  SortedNodes topologicalSort() const;

  /**
   * @brief Find all cycles in the graph
   * @return List of cycles (lists) containing copied T elements that creates a cycle
   */
  Cycles findCycles() const;

  /**
   * @brief Count graph elements
   * @return Total number of nodes in the graph
   */
  inline size_t size() const;

private:
  using Index = size_t;
  using BoolNodes = std::vector<bool>;

  struct Node
  {
    T data;
    std::set<Index> nextNodes;
  };

  KeyProvider getKey;
  std::vector<Node> nodes;
  std::unordered_map<K, Index> keyIndexMap;

private:
  void advanceTopologicalSort(Index index, SortedNodes& sortedNodes, BoolNodes& visited) const;
  void advanceFindCycle(Index index, Cycle& cycle, BoolNodes& visited) const;

};

// ---------

template <class K, class T>
Digraph<K, T>::Digraph(typename Digraph<K, T>::KeyProvider keyProvider)
  : getKey {keyProvider}
{
  // empty
}


template<class K, class T>
size_t Digraph<K, T>::size() const
{
  return nodes.size();
}


template <class K, class T>
void Digraph<K, T>::addNode(T data)
{
  K key = getKey(data);
  if (keyIndexMap.find(key) != keyIndexMap.end()) {
    throw DuplicatedNodeException("Directed Graph error: the node with key "
                                  + key + " already exists");
  }
  nodes.push_back({data, {}});
  keyIndexMap.insert(std::make_pair(key, nodes.size() - 1));
}


template <class K, class T>
T& Digraph<K, T>::getNode(const K& key)
{
  auto it = keyIndexMap.find(key);
  if (it == keyIndexMap.end()) {
    throw NoSuchNodeException("Graph node not found: " + key);
  }
  return nodes[it->second].data;
}


template <class K, class T>
void Digraph<K, T>::addEdge(const K& startNode, const K& endNode)
{
  auto startIt = keyIndexMap.find(startNode);
  if (startIt == keyIndexMap.end()) {
    throw NoSuchNodeException("No such node: " + startNode);
  }

  auto endIt = keyIndexMap.find(endNode);
  if (endIt == keyIndexMap.end()) {
    throw NoSuchNodeException("No such node: " + endNode);
  }

  nodes[startIt->second].nextNodes.insert(endIt->second);
}


template <class K, class T>
void Digraph<K, T>::addEdge(const T& startNode, const T& endNode)
{
  addEdge(getKey(startNode), getKey(endNode));
}


template <class K, class T>
typename Digraph<K, T>::SortedNodes Digraph<K, T>::topologicalSort() const
{
  SortedNodes sortedNodes;
  BoolNodes visited(nodes.size(), false);

  for (Index i = 0; i < nodes.size(); ++i) {
    if (!visited.at(i)) {
      advanceTopologicalSort(i, sortedNodes, visited);
    }
  }

  return sortedNodes;
}


template <class K, class T>
typename Digraph<K, T>::Cycles Digraph<K, T>::findCycles() const
{
  BoolNodes visited(nodes.size(), false);
  Cycles cycles;
  Cycle cycle;

  for (Index i = 0; i < nodes.size(); ++i) {
    if (!visited.at(i)) {
      advanceFindCycle(i, cycle, visited);
      if (!cycle.empty()) {
        cycles.push_back(cycle);
        cycle.clear();
      }
    }
  }
  return cycles;
}


template <class K, class T>
void Digraph<K, T>::advanceTopologicalSort(Digraph<K, T>::Index index,
                                           Digraph<K, T>::SortedNodes& sortedNodes,
                                           Digraph<K, T>::BoolNodes& visited) const
{
  visited[index] = true;

  auto& node = nodes.at(index);
  for (auto& nextNodeIndex: node.nextNodes) {
    if (!visited.at(nextNodeIndex)) {
      advanceTopologicalSort(nextNodeIndex, sortedNodes, visited);
    }
  }

  sortedNodes.push_back(node.data);
}


template <class K, class T>
void Digraph<K, T>::advanceFindCycle(Digraph<K, T>::Index index,
                                     Digraph<K, T>::Cycle& cycle,
                                     Digraph<K, T>::BoolNodes& visited) const
{
  visited[index] = true;

  auto& node = nodes.at(index);
  for (auto& nextNodeIndex: node.nextNodes) {
    if (visited.at(nextNodeIndex)) {
      for (Index i = nextNodeIndex; i <= index; ++i) {
        cycle.push_back(nodes.at(i).data);
      }
      break;
    }
    else {
      advanceFindCycle(nextNodeIndex, cycle, visited);
    }
  }
}

} // namespace cppps


#endif // DIGRAPH_H
