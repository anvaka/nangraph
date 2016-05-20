#include "NanGraph.h"

NanGraph::NanGraph() {
  _graph = std::make_unique<Graph>();
};

NanGraph::~NanGraph() {
  // TODO: Should I reset all Persistent<> data?
};

Nan::Persistent<v8::Function> NanGraph::constructor;

NAN_MODULE_INIT(NanGraph::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);

  tpl->SetClassName(Nan::New("NanGraph").ToLocalChecked());
  // need to set internal fields in order to be able to wrap object.
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "getNodesCount", GetNodesCount);
  Nan::SetPrototypeMethod(tpl, "addNode", AddNode);
  Nan::SetPrototypeMethod(tpl, "getNode", GetNode);

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

  if (info.Length() > 1 && !info[1]->IsUndefined()) {
    self->_saveData(nodeId, info[1]);
  }

  self->_graph->addNode(nodeId);
}

NAN_METHOD(NanGraph::GetNode) {
  NanGraph* self = ObjectWrap::Unwrap<NanGraph>(info.This());
  auto nodeIdStr = v8toString(info[0]);
  auto nodeId = self->_idManager.get(nodeIdStr);
  if (nodeId < 0) {
    return; // no such node.
  }

  v8::Local<v8::Object> node = Nan::New<v8::Object>();
  Nan::Set(node, Nan::New("id").ToLocalChecked(), Nan::New(nodeIdStr).ToLocalChecked());

  // append data if we have any
  auto dataIndex = self->_getDataIndex(nodeId);
  if (dataIndex >= 0) {
    auto data = self->_nodeData[nodeId].Get(info.GetIsolate());
    Nan::Set(node, Nan::New("data").ToLocalChecked(), data);
  }

  info.GetReturnValue().Set(node);
}

void NanGraph::_saveData(int nodeId, const v8::Local<v8::Value>& arg) {
    _nodeData[nodeId].Reset(arg);
}

int NanGraph::_getDataIndex(const int& nodeId) {
  auto search = _nodeData.find(nodeId);
  if (search == _nodeData.end()) return -1;
  return nodeId;
}
