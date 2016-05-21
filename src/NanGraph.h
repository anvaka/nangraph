#ifndef NAN_GRAPH_H
#define NAN_GRAPH_H

#include <nan.h>
#include "Graph.h"
#include "NanGraphUtils.h"
#include "Forwarder.h"

typedef std::unordered_map<int, Nan::Persistent<v8::Value>> JSDataStorage;

/**
 * This class provides v8 bindings to native Graph
 */
class NanGraph : public Nan::ObjectWrap {
  friend struct Forwarder;

 public:
  static NAN_MODULE_INIT(Init);

 private:
  std::unique_ptr<Graph> _graph;
  JSDataStorage _nodeData;
  JSDataStorage _linkData;

  IdManager _idManager;

  explicit NanGraph();
  ~NanGraph();

  static Nan::Persistent<v8::Function> constructor;

  static NAN_METHOD(New);

  /**
   * Gets number of nodes in this graph.
   */
  static NAN_METHOD(GetNodesCount);
  
  /**
   * Gets number of edges (links) in this graph.
   */
  static NAN_METHOD(GetLinksCount);

  /**
   * Adds a new node. Take one or two arguments on JS side.
   *
   * @param {string} nodeId - identifier of the the node.
   * @param {any} data - any contextual data associated with the node.
   */
  static NAN_METHOD(AddNode);
  
  /**
   * Iterates over each node of a graph and invokes a callback.
   * 
   * @param {Function} callback - a callback, which will receive node instance
   *  {id, data} as its argument.
   */
  static NAN_METHOD(ForEachNode);

  /**
   * Gets a node by given id.
   *
   * @param {string} nodeId - identifier of the the node.
   * @returns {object} with two fields:
   *   `id` - node identifier (always a string)
   *   `data` - only present when node was added with `data` attribute. This
   *   is exactly the same object as it was passed to `addNode()`
   */
  static NAN_METHOD(GetNode);

  /**
   * Adds a new link (edge) to the the graph
   *
   * @param {string} fromId - identifier of the edge start
   * @param {string} toId - identifier of the edge end
   */
  static NAN_METHOD(AddLink);

  /**
   * Given from and to node ids, returns a link and associated data (if any);
   * If no link is found, returns undefined.
   *
   * @param {string} fromId - identifier of the edge start
   * @param {string} toId - identifier of the edge end
   */
  static NAN_METHOD(GetLink);
  
  void _saveData(int nodeId, const v8::Local<v8::Value>& arg);
  void _saveLinkData(int linkId, const v8::Local<v8::Value>& arg);
  bool _hasDataId(const JSDataStorage& storage, const int& id);
  v8::Local<v8::Value> _makeNode(v8::Isolate* isolate, const std::string& nodeIdStr, const int& nodeId);
  v8::Local<v8::Value> getJSNodeByInternalId(v8::Isolate* isolate, const int& internalId);
};

#endif
