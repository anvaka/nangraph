#ifndef GRAPH_H
#define GRAPH_H

#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <set>


/**
 * This is our core graph structure. I'm trying to optimize for RAM and speed.
 *
 * Targets:
 * Node insertion time: O(1);
 * Node removal time: O(m * lg(m)), where m is a max degree of adjacent nodes.
 */
class Graph {

public:
  class Node {
    private:
      // these sets store all outgoing and incoming edges for a given node.
      // As you see this brings us to the world of oriented graphs. I'm not sure
      // if I'm going to add non-oriented graphs yet.
      std::multiset<int> inNodes, outNodes;

    public:
      Node() : inNodes(), outNodes() {}

    friend class Graph;
  };

private:
  std::unordered_map<int, Node> _nodes;

public:
  Graph();
  ~Graph();

  int addNode(const int& id);
  int getNodesCount() { return _nodes.size(); }
  
  Graph::Node* getNode(const int& id);
};

#endif
