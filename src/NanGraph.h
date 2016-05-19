#ifndef NAN_GRAPH_H
#define NAN_GRAPH_H

#include <nan.h>
#include "Graph.h"

/**
 * This class provides v8 bindings to native Graph
 */
class NanGraph : public Nan::ObjectWrap {
 public:
  static NAN_MODULE_INIT(Init);

 private:
  std::unique_ptr<Graph> _graph;

  explicit NanGraph();
  ~NanGraph();

  static Nan::Persistent<v8::Function> constructor;

  static NAN_METHOD(GetNodesCount);
  static NAN_METHOD(New);
};

#endif
