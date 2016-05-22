/**
 * We provide a high level wrapper here. Very thin.
 */
module.exports = createGraph;

var NativeGraph = require('bindings')('nangraph').Graph;

function createGraph() {
  var graph = new NativeGraph();

  var api = {
    getNodesCount: getNodesCount,
    getLinksCount: getLinksCount,
    addNode: addNode,
    getNode: getNode,

    addLink: addLink,
    getLink: getLink,
    hasLink: getLink,

    forEachNode: forEachNode,
    forEachLink: forEachLink,

    forEachOut: forEachOut,
    forEachIn: forEachIn,
    getIn: getIn,
    getOut: getOut
  };

  return api;

  function addNode(id, data) {
    if (id === undefined) throw new Error('Node id cannot be undefined');

    graph.addNode(id, data);
  }

  function getNodesCount() {
    return graph.getNodesCount();
  }

  function getLinksCount() {
    return graph.getLinksCount();
  }

  function getNode(id) {
    return graph.getNode(id);
  }

  function getLink(fromId, toId) {
    return graph.getLink(fromId, toId);
  }

  function addLink(fromId, toId, data) {
    if (fromId === undefined) throw new Error('fromId is not set');
    if (toId === undefined) throw new Error('toId is not set');

    return graph.addLink(fromId, toId, data);
  }

  function forEachNode(cb) {
    if (typeof cb !== 'function') throw new Error('Callback is expected to be a function');

    return graph.forEachNode(cb);
  }

  function forEachLink(cb) {
    throw "implement me!";

    if (typeof cb !== 'function') throw new Error('Callback is expected to be a function');

    return graph.forEachLink(cb);
  }

  function forEachOut(nodeId, cb) {
    if (typeof cb !== 'function') throw new Error('Callback is expected to be a function');

    graph.forEachOut(nodeId, cb);
  }

  function forEachIn(nodeId, cb) {
    if (typeof cb !== 'function') throw new Error('Callback is expected to be a function');

    graph.forEachIn(nodeId, cb);
  }

  function getOut(nodeId) {
    var out = [];
    graph.forEachOut(nodeId, function(other) {
      out.push(other);
    })

    return out;
  }

  function getIn(nodeId) {
    var results = [];
    graph.forEachIn(nodeId, function(other) {
      results.push(other);
    })

    return results;
  }
}
