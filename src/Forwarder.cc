#include "Forwarder.h"

bool Forwarder::ForwardNodeResults(const int &nodeId, const Node *node) {
  const unsigned argc = 1;
  auto ctx = Nan::GetCurrentContext();

  v8::Local<v8::Value> argv[argc] = {
    _graph->getJSNodeByInternalId(_isolate, nodeId)
  };
  
  auto cbResult = Nan::MakeCallback(ctx->Global(), _cb, argc, argv);
  
  if (cbResult->IsTrue()) {
    // JS Callback asks us to stop iteration imediately. Tell underlying visitor
    // that we need to quit:
    return true;
  }
  
  // no, continue iteration over remaining nodes.
  return false;
}