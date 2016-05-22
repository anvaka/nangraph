#ifndef NAN_GRAPH_UTILS_H
#define NAN_GRAPH_UTILS_H

#include <nan.h>
#include <string>
#include <unordered_map>

/**
* Converts v8::value to std::string
*/
std::string v8toString(const v8::Local<v8::Value>& arg);

/**
 * This class manages translation between user space ids (strings) and graph
 * ids (numbers). I think most natural graphs have more edges than nodes, and
 * thus storing edges the same way as nodes (string values) could result in
 * bad memory profile.
 */
class IdManager {
private:
  // We keep map of "hash(string) to string" so that clients
  // with hash value could get actual strings later.
  std::unordered_map<std::size_t, std::string> _memory;

  // This is the hasher that will give us ids for each string.
  std::hash<std::string> _hasher;

public:
  IdManager();
  ~IdManager();

  /**
   * Gets graph id for a given user space id (string). Remembers mapping into
   * internal storage.
   * The graph id is equal to hash(stringId).
   */
  std::size_t getAndRemember(const std::string &id);

  /**
   * Returns pointer to id if we've seen given string. Returns
   * nullptr otherwise.
   */
  const std::size_t* getHashPtrFromString(const std::string &id);

  const std::string* getStringPtrFromHash(const std::size_t &hash);
};

#endif
