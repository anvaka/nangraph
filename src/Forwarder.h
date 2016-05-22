#ifndef FORWARDER_H
#define FORWARDER_H

#include <nan.h>
#include "v8.h"
#include "Graph.h"
#include "NanGraph.h"

class NanGraph;

struct Forwarder {
  v8::Local<v8::Function> _cb;
  NanGraph *_graph;
  v8::Isolate *_isolate;

  Forwarder(NanGraph* graph, v8::Isolate *isolate, v8::Local<v8::Function> cb): _cb(cb) {
    _graph = graph;
    _isolate = isolate;
  }

  bool ForwardNodeResults(const std::size_t &nodeId);
  bool ForwardNodeAndLink(const std::size_t &nodeId, const std::size_t &linkId);
};
#endif /* ifndef FORWARDER_H */
