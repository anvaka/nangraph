        #include "Graph.h"

Graph::Graph() {}
Graph::~Graph() {
  for (auto &i : _nodes) {
    delete i.second;
  }
}

Graph::Node* Graph::addNode(const int& id) {
  auto node = getNode(id);
  if (node == nullptr) {
    node = new Node();
    _nodes[id] = node;
  }

  return node;
}

Graph::Node* Graph::getNode(const int& id) {
  auto iter = _nodes.find(id);
  if (iter == _nodes.end()) {
    return nullptr;
  }
  
  return iter->second;
}

int Graph::addLink(const int& fromId, const int &toId) {
  auto fromNode = addNode(fromId);
  auto toNode = addNode(toId);

  fromNode->outNodes.insert(toId);
  toNode->inNodes.insert(fromId);
  
  return getLinkId(fromId, toId);
}

int Graph::getLinkId(const int& fromId, const int &toId) {
  // TODO: Should I inline this?
  return fromId ^ (toId << 1);
}

bool Graph::hasLink(const int& fromId, const int &toId) {
  auto fromNode = getNode(fromId);
  if (fromNode == nullptr) return false;
  
  return fromNode->outNodes.find(toId) != fromNode->outNodes.end();
}