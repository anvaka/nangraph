#include "NanGraphUtils.h"

std::string v8toString(const v8::Local<v8::Value>& arg) {
  v8::String::Utf8Value param(arg->ToString());

  return std::string(*param);
}


IdManager::IdManager() {}
IdManager::~IdManager() {}

int IdManager::getAndRemember(const std::string& id) {
  auto search = _memory.find(id);

  if(search == _memory.end()) {
    _memory[id] = _lastAvailableId;
    _lastAvailableId++;
    return _memory[id];
  }

  return search->second;
}
