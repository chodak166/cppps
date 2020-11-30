// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.


#include <catch2/catch.hpp>
//#include <fakeit/catch/fakeit.hpp>

#include <nixlab/stdeasylog> //TODO: remove

#include <list>
#include <set>
#include <map>

//NOTE: use -Wno-disabled-macro-expansion to suppress fakeit macro warnings
//using namespace fakeit;

namespace {

}

template <class K, class T>
class DirectedGraph
{
public:
  using KeyProvider = std::function<K (const T& data)>;
  //using RoutedNodes = std::list<std::reference_wrapper<T>>;
  using RoutedNodes = std::list<T>;

  DirectedGraph(KeyProvider keyProvider)
    : getKey {keyProvider} {}

  virtual ~DirectedGraph() = default;

  size_t size() const {return nodes.size();}

  void addNode(T data)
  {
    //TODO throw exception if key exists
    K key = getKey(data);
    nodes.push_back({data, {}});
    keyIndexMap.insert(std::make_pair(key, nodes.size() - 1));
  }

//  void addNode(T&& data)
//  {

//  }

  void addEdge(const K& startNode, const K& endNode)
  {
    //TODO throw exception if key doesn't exist
    auto startIndex = keyIndexMap.at(startNode);
    auto endIndex = keyIndexMap.at(endNode);
    nodes[startIndex].nextNodes.insert(endIndex);
  }

  void addEdge(const T& startNode, const T& endNode)
  {
    //TODO throw exception if key doesn't exist
    auto startIndex = keyIndexMap.at(getKey(startNode));
    auto endIndex = keyIndexMap.at(getKey(endNode));
    nodes[startIndex].nextNodes.insert(endIndex);
  }

  RoutedNodes topologicalSort()
  {
    RoutedNodes routedNodes;
    BoolNodes visited(nodes.size(), false);

    for (Index i = 0; i < nodes.size(); ++i) {
      if (!visited.at(i)) {
        advanceRoutedNodes(i, routedNodes, visited);
      }
    }

    return routedNodes;
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
  std::map<K, Index> keyIndexMap;

private:

  void advanceRoutedNodes(Index index, RoutedNodes& routedNodes, BoolNodes& visited)
  {
    visited[index] = true;

    auto& node = nodes.at(index);
    for (auto& nextNodeIndex: node.nextNodes) {
      if (!visited.at(nextNodeIndex)) {
        advanceRoutedNodes(nextNodeIndex, routedNodes, visited);
      }
    }

    routedNodes.push_back(node.data);
  }

};

namespace test {
namespace {

// test class to store in the graph
class User
{
public:
  User(std::string_view login, int age)
    : login{login}, age {age} {}

  const std::string& getLogin() const {return login;}
  int getAge() const {return age;}

private:
  const std::string login;
  const int age;
};

bool operator<(const User& lhs, const User& rhs)
{
  return lhs.getLogin() < rhs.getLogin();
}

bool operator==(const User& lhs, const User& rhs)
{
  return lhs.getLogin() == rhs.getLogin();
}

using IndexMap = std::map<User, size_t>;

template <class T>
using KeyGetter = std::function<std::string(const typename T::value_type)>;

template <class T>
IndexMap getIndices(const T& collection)
{
  IndexMap map;
  size_t i = 0;
  for (const auto& item: collection) {
    map.insert(std::make_pair(item, i++));
  }

  return map;
}

const User NODE_A {"a", 25};
const User NODE_B {"b", 26};
const User NODE_C {"c", 27};
const User NODE_D {"d", 28};
const User NODE_E {"e", 29};
const User NODE_F {"f", 30};
const User NODE_G {"g", 31};
const User NODE_H {"h", 32};
const User NODE_I {"i", 33};
const User NODE_J {"j", 34};
const User NODE_K {"k", 35};
const User NODE_L {"l", 36};
const User NODE_M {"m", 37};
const User NODE_N {"n", 38};
const User NODE_O {"o", 39};

} // namespace
} // namespace test

TEST_CASE("Testing graph editing", "[graph_ed]")
{
  auto keyGetter = [](const test::User& user){return user.getLogin();};
  DirectedGraph<std::string, test::User> graph(keyGetter);

  SECTION("When the nodes are added to the graph, "
          "then the size of the graph can be calculated")
  {
    graph.addNode(test::NODE_A);
    graph.addNode(test::NODE_B);
    graph.addNode(test::NODE_C);

    REQUIRE(graph.size() == 3);
  }
}


TEST_CASE("Testing graph topological sorting", "[graph_ts]")
{
  auto keyGetter = [](const test::User& user){return user.getLogin();};
  DirectedGraph<std::string, test::User> graph(keyGetter);

  SECTION("When the nodes are added to the graph, "
          "then the size of the graph can be calculated")
  {
    graph.addNode(test::NODE_A);
    graph.addNode(test::NODE_B);
    graph.addNode(test::NODE_C);

    REQUIRE(graph.size() == 3);
  }


  SECTION("When the topological sort is performed on a connected graph, "
          "then every tail has index lower than its head")
  {
    graph.addNode(test::NODE_A);
    graph.addNode(test::NODE_B);
    graph.addNode(test::NODE_C);

    graph.addEdge(test::NODE_B, test::NODE_A);
    graph.addEdge(test::NODE_B, test::NODE_C);

    auto sorted = graph.topologicalSort();

    auto indexMap = test::getIndices(sorted);
    REQUIRE(indexMap[test::NODE_A] < indexMap[test::NODE_B]);
    REQUIRE(indexMap[test::NODE_C] < indexMap[test::NODE_B]);
  }

  SECTION("When the topological sort is performed, "
          "then the returned objects have initial data preserved")
  {
    graph.addNode(test::NODE_C);
    graph.addNode(test::NODE_B);
    graph.addNode(test::NODE_A);

    graph.addEdge(test::NODE_B, test::NODE_C);
    graph.addEdge(test::NODE_B, test::NODE_A);

    auto sorted = graph.topologicalSort();

    auto indexMap = test::getIndices(sorted);
    REQUIRE(indexMap[test::NODE_A] < indexMap[test::NODE_B]);
    REQUIRE(indexMap[test::NODE_C] < indexMap[test::NODE_B]);
  }

  SECTION("When the topological sort is performed on a connected graph, "
          "then every tail has index lower than its head regardless of insertion order")
  {
    graph.addNode(test::NODE_A);
    graph.addNode(test::NODE_B);
    graph.addEdge(test::NODE_B, test::NODE_A);

    auto sorted = graph.topologicalSort();
    REQUIRE(sorted.front() == test::NODE_A);
    REQUIRE(sorted.back() == test::NODE_B);
  }

  SECTION("When the topological sort is performed on a single node graph, "
          "then the only node is returned")
  {
    graph.addNode(test::NODE_A);

    auto sorted = graph.topologicalSort();
    REQUIRE(sorted.size() == 1);
    REQUIRE(sorted.front() == test::NODE_A);
  }

  SECTION("When the topological sort is performed on a disconnected graph, "
          "then all nodes are processed")
  {
    graph.addNode(test::NODE_A);
    graph.addNode(test::NODE_B);
    graph.addNode(test::NODE_C);

    graph.addEdge(test::NODE_B, test::NODE_C);
    graph.addEdge(test::NODE_B, test::NODE_A);

    graph.addNode(test::NODE_J);
    graph.addNode(test::NODE_K);

    graph.addEdge(test::NODE_J, test::NODE_K);

    auto sorted = graph.topologicalSort();

    REQUIRE(sorted.size() == graph.size());
  }

  SECTION("When the topological sort is performed on a disconnected graph, "
          "then every tail has index lower than its head in every connected subgraph")
  {
    graph.addNode(test::NODE_A);
    graph.addNode(test::NODE_B);
    graph.addNode(test::NODE_C);

    graph.addEdge(test::NODE_B, test::NODE_A);
    graph.addEdge(test::NODE_B, test::NODE_C);

    graph.addNode(test::NODE_J);
    graph.addNode(test::NODE_K);

    graph.addEdge(test::NODE_J, test::NODE_K);

    auto sorted = graph.topologicalSort();
    auto indexMap = test::getIndices(sorted);

    REQUIRE(indexMap[test::NODE_A] < indexMap[test::NODE_B]);
    REQUIRE(indexMap[test::NODE_C] < indexMap[test::NODE_B]);
    REQUIRE(indexMap[test::NODE_K] < indexMap[test::NODE_J]);
  }

  SECTION("When the topological sort is performed on a cyclic graph, "
          "then the sorting is finished with no exception")
  {
    graph.addNode(test::NODE_A);
    graph.addNode(test::NODE_B);
    graph.addNode(test::NODE_C);

    graph.addEdge(test::NODE_A, test::NODE_B);
    graph.addEdge(test::NODE_B, test::NODE_C);
    graph.addEdge(test::NODE_C, test::NODE_A);

    REQUIRE_NOTHROW(graph.topologicalSort());
  }

  SECTION("When the topological sort is performed on a cyclic graph, "
          "then the sorting result has the same size as the graph")
  {
    graph.addNode(test::NODE_A);
    graph.addNode(test::NODE_B);
    graph.addNode(test::NODE_C);

    graph.addEdge(test::NODE_A, test::NODE_B);
    graph.addEdge(test::NODE_B, test::NODE_C);
    graph.addEdge(test::NODE_C, test::NODE_A);

    auto sorted = graph.topologicalSort();

    REQUIRE(sorted.size() == graph.size());
  }

}


namespace test {
namespace {


} // namespace
} // namespace test

