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
  int _lastAvailableId = 0;
  std::unordered_map<std::string, int> _memory;

public:
  IdManager();
  ~IdManager();

  /**
  * Gets graph id for a given user space id (string)
  */
  int getAndRemember(const std::string &id);
  
  /**
   gets integer id but does not remember anything. If no such node exist
   negative value is returned
   */
  int get(const std::string &id);
};

#endif
