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
    forEachNode: forEachNode
  };

  return api;

  function addNode(id, data) {
    if (id === undefined) {
      throw new Error('Node id cannot be undefined');
    }
    graph.addNode(id, data);
  }

  function getNodesCount() { return graph.getNodesCount(); }

  function getLinksCount() { return graph.getLinksCount(); }

  function getNode(id) { return graph.getNode(id); }

  function getLink(fromId, toId) { return graph.getLink(fromId, toId); }

  function addLink(fromId, toId, data) { return graph.addLink(fromId, toId, data); }

  function forEachNode(cb) { return graph.forEachNode(cb); }
}
