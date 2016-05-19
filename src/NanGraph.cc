#include "NanGraph.h"

NanGraph::NanGraph() {
  _graph = std::make_unique<Graph>();
};

NanGraph::~NanGraph() {};

Nan::Persistent<v8::Function> NanGraph::constructor;

NAN_MODULE_INIT(NanGraph::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);

  tpl->SetClassName(Nan::New("NanGraph").ToLocalChecked());
  // TODO: Do I need this?
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "getNodesCount", GetNodesCount);
  Nan::SetPrototypeMethod(tpl, "addNode", AddNode);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("Graph").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(NanGraph::New) {
  if (info.IsConstructCall()) {
    NanGraph *obj = new NanGraph();
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    const unsigned argc = 0;

    v8::Local<v8::Value> argv[argc] = {};
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(NanGraph::constructor);
    v8::Local<v8::Object> instance = cons->NewInstance(argc, argv);

    info.GetReturnValue().Set(instance);
  }
}

NAN_METHOD(NanGraph::GetNodesCount) {
  NanGraph* self = ObjectWrap::Unwrap<NanGraph>(info.This());
  auto result = self->_graph->getNodesCount();
  info.GetReturnValue().Set(result);
}

NAN_METHOD(NanGraph::AddNode) {
  NanGraph* self = ObjectWrap::Unwrap<NanGraph>(info.This());

  auto nodeIdStr = v8toString(info[0]);
  auto nodeId = self->_idManager.getAndRemember(nodeIdStr);

  self->_graph->addNode(nodeId, nullptr);

  //info.GetReturnValue().Set(result);
}
