#include "Graph.h"

Graph::Graph() {}
Graph::~Graph() {}

int Graph::addNode(const int& id) {
  auto node = getNode(id);
  if (node == nullptr) {
    _nodes[id] = Node();
  }

  return id;
}

Graph::Node* Graph::getNode(const int& id) {
  auto iter = _nodes.find(id);
  if (iter == _nodes.end()) {
    return nullptr;
  }

  return &(iter->second);
}
