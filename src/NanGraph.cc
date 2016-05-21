#include "NanGraph.h"

NanGraph::NanGraph() {
  _graph = std::make_unique<Graph>();
};

NanGraph::~NanGraph() {
  // release all persistent objects
  for (auto &i : _nodeData) {
    i.second.Reset();
  }
  for (auto &i : _linkData) {
    i.second.Reset();
  }
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
  Nan::SetPrototypeMethod(tpl, "addLink", AddLink);
  Nan::SetPrototypeMethod(tpl, "getLink", GetLink);

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
  auto dataIndex = self->_getDataIndex(self->_nodeData, nodeId);
  if (dataIndex >= 0) {
    auto data = self->_nodeData[nodeId].Get(info.GetIsolate());
    Nan::Set(node, Nan::New("data").ToLocalChecked(), data);
  }

  info.GetReturnValue().Set(node);
}

NAN_METHOD(NanGraph::AddLink) {
  NanGraph* self = ObjectWrap::Unwrap<NanGraph>(info.This());
  
  auto fromIdIdStr = v8toString(info[0]);
  auto fromId = self->_idManager.getAndRemember(fromIdIdStr);

  auto toIdIdStr = v8toString(info[1]);
  auto toId = self->_idManager.getAndRemember(toIdIdStr);

  auto linkId = self->_graph->addLink(fromId, toId);
  
  if (info.Length() > 2 && !info[2]->IsUndefined()) {
    // todo: undefined deletes?
    self->_saveLinkData(linkId, info[2]);
  }
}

NAN_METHOD(NanGraph::GetLink) {
  NanGraph* self = ObjectWrap::Unwrap<NanGraph>(info.This());
  
  auto fromIdIdStr = v8toString(info[0]);
  auto fromId = self->_idManager.get(fromIdIdStr);
  if (fromId < 0) return;
  
  auto toIdIdStr = v8toString(info[1]);
  auto toId = self->_idManager.get(toIdIdStr);
  if (toId < 0) return;

  if (!self->_graph->hasLink(fromId, toId)) return;
  
  v8::Local<v8::Object> link = Nan::New<v8::Object>();
  Nan::Set(link, Nan::New("fromId").ToLocalChecked(), info[0]);
  Nan::Set(link, Nan::New("toId").ToLocalChecked(), info[1]);
  
  auto linkId = self->_graph->getLinkId(fromId, toId);
  auto dataIndex = self->_getDataIndex(self->_linkData, linkId);
  if (dataIndex >= 0) {
    // link has data, add it:
    auto data = self->_linkData[linkId].Get(info.GetIsolate());
    Nan::Set(link, Nan::New("data").ToLocalChecked(), data);
  }

  info.GetReturnValue().Set(link);
}


void NanGraph::_saveLinkData(int linkId, const v8::Local<v8::Value>& arg) {
    _linkData[linkId].Reset(arg);
}

void NanGraph::_saveData(int nodeId, const v8::Local<v8::Value>& arg) {
    _nodeData[nodeId].Reset(arg);
}

int NanGraph::_getDataIndex(const JSDataStorage& storage, const int& nodeId) {
  auto search = storage.find(nodeId);
  if (search == storage.end()) return -1;
  return nodeId;
}
