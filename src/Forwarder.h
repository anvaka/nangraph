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

  bool ForwardNodeResults(const int &nodeId, const Node *node);

//  void ForwardLinkedNode(NodeDef *node, LinkDef *link) {
//    const unsigned argc = 2;
//
//    v8::Local<v8::Value> argv[argc] = {
//      Forwarder::wrapNodeDef(node),
//      Forwarder::wrapLinkDef(link)
//    };
//
//    Nan::MakeCallback(Nan::GetCurrentContext()->Global(), cb, argc, argv);
//  }

//  bool forward_link_result(LinkDef *result) {
//    const unsigned argc = 1;
//
//    v8::Local<v8::Value> argv[argc] = { Forwarder::wrapLinkDef(result) };
//    auto cbResult = Nan::MakeCallback(Nan::GetCurrentContext()->Global(), cb, argc, argv);
//
//    if (cbResult->IsBoolean()) {
//       return cbResult->BooleanValue();
//    }
//
//    return false; // no, don't stop the iteration
//  }
};
#endif /* ifndef FORWARDER_H */
