#include "Graph.h"

Graph::Graph() {}

Graph::~Graph() {
  for (auto &i : _nodes) {
    delete i.second;
  }
}

Node* Graph::addNode(const std::size_t& id) {
  auto node = getNode(id);
  if (node == nullptr) {
    node = new Node();
    _nodes[id] = node;
  }

  return node;
}

Node* Graph::getNode(const std::size_t& id) {
  auto iter = _nodes.find(id);
  if (iter == _nodes.end()) {
    return nullptr;
  }
  
  return iter->second;
}

bool Graph::forEachNode(NodeCallback callback) {
  for (auto &i : _nodes) {
    auto shouldStop = callback(i.first, i.second);
    if (shouldStop) return true;
  }
  
  return false;
}

std::size_t Graph::addLink(const std::size_t& fromId, const std::size_t &toId) {
  auto fromNode = addNode(fromId);
  auto toNode = addNode(toId);

  fromNode->outNodes.insert(toId);
  toNode->inNodes.insert(fromId);
  _linksCount += 1;
  
  return getLinkId(fromId, toId);
}

std::size_t Graph::getLinkId(const std::size_t& fromId, const std::size_t &toId) {
  std::size_t seed = 0;
  seed ^= fromId + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  seed ^= toId + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  return seed;
}

bool Graph::hasLink(const std::size_t& fromId, const std::size_t &toId) {
  auto fromNode = getNode(fromId);
  if (fromNode == nullptr) return false;
  
  return fromNode->outNodes.find(toId) != fromNode->outNodes.end();
}
