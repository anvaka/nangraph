#ifndef NAN_GRAPH_H
#define NAN_GRAPH_H

#include <nan.h>
#include "Graph.h"
#include "NanGraphUtils.h"

/**
 * This class provides v8 bindings to native Graph
 */
class NanGraph : public Nan::ObjectWrap {

 public:
  static NAN_MODULE_INIT(Init);

 private:
  std::unique_ptr<Graph> _graph;
  IdManager _idManager;

  explicit NanGraph();
  ~NanGraph();

  static Nan::Persistent<v8::Function> constructor;

  static NAN_METHOD(New);

  /**
   * Gets number of nodes in this graph.
   */
  static NAN_METHOD(GetNodesCount);

  /**
   * Adds a new node. Take one or two arguments on JS side.
   *
   * @param {string} nodeId - identifier of the the node.
   * @param {string} data - any contextual data associated with the node.
   */
  static NAN_METHOD(AddNode);

};

#endif
