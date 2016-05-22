#include "NanGraphUtils.h"
#include <iostream>

std::string v8toString(const v8::Local<v8::Value>& arg) {
  v8::String::Utf8Value param(arg->ToString());

  return std::string(*param);
}

IdManager::IdManager() {}
IdManager::~IdManager() {}

std::size_t IdManager::getAndRemember(const std::string& stringId) {
  auto id = _hasher(stringId);
  _memory[id] = stringId;
  return id;
}

const std::size_t* IdManager::getHashPtrFromString(const std::string &stringId) {
  auto id = _hasher(stringId);
  auto search = _memory.find(id);
  if (search == _memory.end()) {
    return nullptr;
  }

  return &(search->first);
}

const std::string* IdManager::getStringPtrFromHash(const std::size_t &hash) {
  auto search = _memory.find(hash);
  if (search == _memory.end()) {
    return nullptr;
  }
  return &(search->second);
}
