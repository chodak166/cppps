// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.

#include "cppps/Digraph.h"

#include <catch2/catch.hpp>
//#include <fakeit/catch/fakeit.hpp>

#include <nixlab/stdeasylog> //TODO: remove



//NOTE: use -Wno-disabled-macro-expansion to suppress fakeit macro warnings
//using namespace fakeit;

namespace {

}


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

const User NODE_X {"x", 100};

} // namespace
} // namespace test

TEST_CASE("Testing graph editing", "[graph_ed]")
{
  auto keyGetter = [](const test::User& user){return user.getLogin();};
  Digraph<std::string, test::User> graph(keyGetter);


  SECTION("When no nodes were added to the graph, "
          "then the size of the graph is 0")
  {
    REQUIRE(graph.size() == 0);
  }

  SECTION("When the nodes are added to the graph, "
          "then the size of the graph can be calculated")
  {
    graph.addNode(test::NODE_A);
    graph.addNode(test::NODE_B);
    graph.addNode(test::NODE_C);

    REQUIRE(graph.size() == 3);
  }

  SECTION("When the same node is added twice, "
          "then the second call throws an exception")
  {
    graph.addNode(test::NODE_A);
    REQUIRE_THROWS_AS(graph.addNode(test::NODE_A), DuplicatedNodeException);
  }

  SECTION("When an edge with non-existent tail is added by keys, "
          "then an exception is thrown")
  {
    graph.addNode(test::NODE_A);
    REQUIRE_THROWS_AS(
          graph.addEdge(keyGetter(test::NODE_X), keyGetter(test::NODE_A)),
          NoSuchNodeException);
  }

  SECTION("When an edge with non-existent head is added by keys, "
          "then an exception is thrown")
  {
    graph.addNode(test::NODE_A);
    REQUIRE_THROWS_AS(
          graph.addEdge(keyGetter(test::NODE_A), keyGetter(test::NODE_X)),
          NoSuchNodeException);
  }

  SECTION("When an edge with non-existent tail is added by objects, "
          "then an exception is thrown")
  {
    graph.addNode(test::NODE_A);
    REQUIRE_THROWS_AS(
          graph.addEdge(test::NODE_X, test::NODE_A),
          NoSuchNodeException);
  }

  SECTION("When an edge with non-existent head is added by objects, "
          "then an exception is thrown")
  {
    graph.addNode(test::NODE_A);
    REQUIRE_THROWS_AS(
          graph.addEdge(test::NODE_A, test::NODE_X),
          NoSuchNodeException);
  }

  SECTION("When an existing node is requested by its key, "
          "then requested node is returned")
  {
    graph.addNode(test::NODE_A);
    auto& node = graph.getNode(keyGetter(test::NODE_A));
    REQUIRE(node == test::NODE_A);
  }

  SECTION("When a non-existent node is requested by a key, "
          "then an exception is thrown")
  {
    REQUIRE_THROWS_AS(graph.getNode(keyGetter(test::NODE_X)), NoSuchNodeException);
  }
}


TEST_CASE("Testing graph topological sorting", "[graph_ts]")
{
  auto keyGetter = [](const test::User& user){return user.getLogin();};
  Digraph<std::string, test::User> graph(keyGetter);

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

TEST_CASE("Testing finding cycles", "[graph_cycles]")
{
  auto keyGetter = [](const test::User& user){return user.getLogin();};
  Digraph<std::string, test::User> graph(keyGetter);

  SECTION("When the graph has no cycles, "
          "then no cycles are found")
  {
    graph.addNode(test::NODE_A);
    graph.addNode(test::NODE_B);
    graph.addNode(test::NODE_C);

    graph.addEdge(test::NODE_A, test::NODE_B);
    graph.addEdge(test::NODE_A, test::NODE_C);

    auto cycle = graph.findCycles();

    REQUIRE(cycle.empty());
  }

  SECTION("When the graph has only one node connected with itself, "
          "then the cycle is found")
  {
    graph.addNode(test::NODE_A);
    graph.addEdge(test::NODE_A, test::NODE_A);

    auto cycles = graph.findCycles();
    REQUIRE(cycles.size() == 1);
  }

  SECTION("When the graph contains a cycle, "
          "then the cycle is found")
  {
    graph.addNode(test::NODE_A);
    graph.addNode(test::NODE_B);

    graph.addEdge(test::NODE_A, test::NODE_B);
    graph.addEdge(test::NODE_B, test::NODE_A);

    auto cycles = graph.findCycles();
    REQUIRE(cycles.size() == 1);
  }

  SECTION("When the graph contains multiple, disconnected cycles, "
          "then all the cycles are found")
  {
    graph.addNode(test::NODE_A);
    graph.addNode(test::NODE_B);
    graph.addEdge(test::NODE_A, test::NODE_B);
    graph.addEdge(test::NODE_B, test::NODE_A);

    graph.addNode(test::NODE_C);
    graph.addNode(test::NODE_D);
    graph.addEdge(test::NODE_C, test::NODE_D);
    graph.addEdge(test::NODE_D, test::NODE_C);


    auto cycles = graph.findCycles();
    REQUIRE(cycles.size() == 2);
  }

  SECTION("When a cycle is found at the begining of the graph, "
          "then all the elements of that cycle are returned")
  {
    graph.addNode(test::NODE_A);
    graph.addNode(test::NODE_B);
    graph.addNode(test::NODE_C);

    graph.addEdge(test::NODE_A, test::NODE_B);
    graph.addEdge(test::NODE_B, test::NODE_C);
    graph.addEdge(test::NODE_C, test::NODE_A);

    auto cycles = graph.findCycles();
    auto cycle = cycles.front();

    auto it = cycle.begin();
    REQUIRE(*it == test::NODE_A); std::advance(it, 1);
    REQUIRE(*it == test::NODE_B); std::advance(it, 1);
    REQUIRE(*it == test::NODE_C); std::advance(it, 1);
    REQUIRE(it == cycle.end());
  }


  SECTION("When a cycle is found in the middle of the graph, "
          "then all the elements of that cycle are returned")
  {
    graph.addNode(test::NODE_A);
    graph.addNode(test::NODE_B);
    graph.addNode(test::NODE_C);
    graph.addNode(test::NODE_D);
    graph.addNode(test::NODE_E);

    graph.addEdge(test::NODE_A, test::NODE_B);
    graph.addEdge(test::NODE_B, test::NODE_C);
    graph.addEdge(test::NODE_C, test::NODE_D);
    graph.addEdge(test::NODE_D, test::NODE_B);
    graph.addEdge(test::NODE_E, test::NODE_D); // one extra node not in the path

    auto cycles = graph.findCycles();
    auto cycle = cycles.front();

    auto it = cycle.begin();
    REQUIRE(*it == test::NODE_B); std::advance(it, 1);
    REQUIRE(*it == test::NODE_C); std::advance(it, 1);
    REQUIRE(*it == test::NODE_D); std::advance(it, 1);
    REQUIRE(it == cycle.end());
  }

  SECTION("When the graph is disconnected and has a cycle in the latter subgraph, "
          "then all the elements of that cycle are returned")
  {
    graph.addNode(test::NODE_A);
    graph.addNode(test::NODE_B);
    graph.addNode(test::NODE_C);
    graph.addEdge(test::NODE_A, test::NODE_B);
    graph.addEdge(test::NODE_B, test::NODE_C);

    graph.addNode(test::NODE_D);
    graph.addNode(test::NODE_E);
    graph.addNode(test::NODE_F);
    graph.addEdge(test::NODE_D, test::NODE_E);
    graph.addEdge(test::NODE_E, test::NODE_F);
    graph.addEdge(test::NODE_F, test::NODE_D);

    auto cycles = graph.findCycles();
    auto cycle = cycles.front();

    auto it = cycle.begin();
    REQUIRE(*it == test::NODE_D); std::advance(it, 1);
    REQUIRE(*it == test::NODE_E); std::advance(it, 1);
    REQUIRE(*it == test::NODE_F); std::advance(it, 1);
    REQUIRE(it == cycle.end());
  }

  SECTION("When the graph is disconnected and contains multiple cycles, "
          "then all returned cycles contains their nodes")
  {
    graph.addNode(test::NODE_A);
    graph.addNode(test::NODE_B);
    graph.addNode(test::NODE_C);
    graph.addEdge(test::NODE_A, test::NODE_B);
    graph.addEdge(test::NODE_B, test::NODE_C);
    graph.addEdge(test::NODE_C, test::NODE_A);

    graph.addNode(test::NODE_D);
    graph.addNode(test::NODE_E);
    graph.addNode(test::NODE_F);
    graph.addEdge(test::NODE_D, test::NODE_E);
    graph.addEdge(test::NODE_E, test::NODE_F);
    graph.addEdge(test::NODE_F, test::NODE_D);

    auto cycles = graph.findCycles();
    auto cyclesIt = cycles.begin();
    auto cycle1 = *cyclesIt;
    auto it = cycle1.begin();

    REQUIRE(cycle1.size() == 3);
    REQUIRE(*it == test::NODE_A); std::advance(it, 1);
    REQUIRE(*it == test::NODE_B); std::advance(it, 1);
    REQUIRE(*it == test::NODE_C); std::advance(it, 1);
    REQUIRE(it == cycle1.end());

    std::advance(cyclesIt, 1);
    auto cycle2 = *cyclesIt;
    it = cycle2.begin();

    REQUIRE(cycle2.size() == 3);
    REQUIRE(*it == test::NODE_D); std::advance(it, 1);
    REQUIRE(*it == test::NODE_E); std::advance(it, 1);
    REQUIRE(*it == test::NODE_F); std::advance(it, 1);
    REQUIRE(it == cycle2.end());
  }


}


namespace test {
namespace {


} // namespace
} // namespace test

