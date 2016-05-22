#include "Forwarder.h"

bool Forwarder::ForwardNodeResults(const std::size_t &nodeId) {
  const unsigned argc = 1;
  auto ctx = Nan::GetCurrentContext();

  v8::Local<v8::Value> argv[argc] = {
    _graph->getJSNodeByInternalId(_isolate, nodeId)
  };
  
  auto cbResult = Nan::MakeCallback(ctx->Global(), _cb, argc, argv);

  // We ask iterator to stop if client's callback returned true.
  return cbResult->IsTrue();
}

bool Forwarder::ForwardNodeAndLink(const std::size_t &nodeId, const std::size_t &linkId) {
  auto ctx = Nan::GetCurrentContext();

  auto node = _graph->getJSNodeByInternalId(_isolate, nodeId);
  v8::Local<v8::Value> cbResult;

  if (_graph->_hasDataId(_graph->_linkData, linkId)) {
    auto linkData = _graph->_linkData[linkId].Get(_isolate);
    
    v8::Local<v8::Value> argv[2] = {
      node,
      linkData
    };
    cbResult = Nan::MakeCallback(ctx->Global(), _cb, 2, argv);
  } else {
    v8::Local<v8::Value> argv[1] = {
      _graph->getJSNodeByInternalId(_isolate, nodeId)
    };
    
    cbResult = Nan::MakeCallback(ctx->Global(), _cb, 1, argv);
  }

  // We ask iterator to stop if client's callback returned true.
  return cbResult->IsTrue();
}