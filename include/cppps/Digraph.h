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

template <class K, class T>
class Digraph
{
public:
  using KeyProvider = std::function<K (const T& data)>;
  using SortedNodes = std::list<T>;
  using Cycle = std::list<T>;
  using Cycles = std::list<Cycle>;

  Digraph(KeyProvider keyProvider)
    : getKey {keyProvider} {}

  virtual ~Digraph() = default;

  size_t size() const {return nodes.size();}

  void addNode(T data)
  {
    K key = getKey(data);
    if (keyIndexMap.find(key) != keyIndexMap.end()) {
      throw DuplicatedNodeException("Directed Graph error: the node with key "
                                    + key + " already exists");
    }
    nodes.push_back({data, {}});
    keyIndexMap.insert(std::make_pair(key, nodes.size() - 1));
  }

  T& getNode(const K& key)
  {
    auto it = keyIndexMap.find(key);
    if (it == keyIndexMap.end()) {
      throw NoSuchNodeException("Graph node not found: " + key);
    }
    return nodes[it->second].data;
  }

  void addEdge(const K& startNode, const K& endNode)
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

  void addEdge(const T& startNode, const T& endNode)
  {
    addEdge(getKey(startNode), getKey(endNode));
  }

  SortedNodes topologicalSort()
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

  Cycles findCycles()
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

  void advanceTopologicalSort(Index index, SortedNodes& sortedNodes, BoolNodes& visited)
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

  void advanceFindCycle(Index index, Cycle& cycle, BoolNodes& visited)
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

};

} // namespace cppps


#endif // DIGRAPH_H
