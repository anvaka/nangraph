#include "Forwarder.h"

bool Forwarder::ForwardNode(const std::size_t &nodeId) {
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

bool Forwarder::ForwardLink(const std::size_t &fromId, const std::size_t &toId, const std::size_t &linkId) {
  auto ctx = Nan::GetCurrentContext();
  auto idManager = &(_graph->_idManager);
  // TODO: Throw?
  auto fromIdStrPtr = idManager->getStringPtrFromHash(fromId);
  auto toIdStrPtr = idManager->getStringPtrFromHash(toId);
  
  auto link = Nan::New<v8::Object>();
  Nan::Set(link, Nan::New("fromId").ToLocalChecked(), Nan::New(*fromIdStrPtr).ToLocalChecked());
  Nan::Set(link, Nan::New("toId").ToLocalChecked(), Nan::New(*toIdStrPtr).ToLocalChecked());
  
  // append data if we have any...
  if (_graph->_hasDataId(_graph->_linkData, linkId)) {
    auto data = _graph->_linkData[linkId].Get(_isolate);
    Nan::Set(link, Nan::New("data").ToLocalChecked(), data);
  }
  
  v8::Local<v8::Value> argv[1] = { link };
  
  auto cbResult = Nan::MakeCallback(ctx->Global(), _cb, 1, argv);
  return cbResult->IsTrue();
}
