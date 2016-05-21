/**
 * We provide a high level wrapper here. Probably this will come at extra cost
 */
module.exports = createGraph;

var NativeGraph = require('bindings')('nangraph').Graph;

function createGraph() {
  var graph = new NativeGraph();

  var api = {
    getNodesCount: getNodesCount,
    addNode: addNode,
    getNode: getNode,
    addLink: addLink,
    getLink: getLink,
    hasLink: getLink
  };

  return api;

  function getNodesCount() {
    return graph.getNodesCount();
  }

  function addNode(id, data) {
    if (id === undefined) {
      throw new Error('Node id cannot be undefined');
    }
    graph.addNode(id, data);
  }

  function getNode(id) {
    return graph.getNode(id);
  }

  function getLink(fromId, toId) {
    return graph.getLink(fromId, toId);
  }

  function addLink(fromId, toId, data) {
    return graph.addLink(fromId, toId, data);
  }
}
