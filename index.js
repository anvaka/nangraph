/**
 * We provide a high level wrapper here. Probably this will come at extra cost
 */
module.exports = createGraph;

var NativeGraph = require('bindings')('nangraph').Graph;

function createGraph() {
  var graph = new NativeGraph();

  var api = {
    getNodesCount: getNodesCount
  };

  return api;

  function getNodesCount() {
    return graph.getNodesCount();
  }
}
