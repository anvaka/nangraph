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
    getOut: getOut,

    // ngraph.graph compatibility interface
    forEachLinkedNode: forEachLinkedNode,
    getLinks: getLinks,
    getNativeGraph: getNativeGraph
  };

  return api;

  function getNativeGraph() {
    return graph;
  }

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

  function forEachLinkedNode(nodeId, cb, isOriented) {
    if (typeof cb !== 'function') throw new Error('Callback is expected to be a function');

    var quitFast = graph.forEachOut(nodeId, function(to, data) {
      var cbData = {
        fromId: nodeId,
        toId: to.id
      };
      if (data) cbData.data = data;
      return cb(to, cbData);
    });

    if (isOriented) return quitFast; // when this is passed to ngraph, it never visited incoming links
    if (quitFast) return quitFast; // quitFast means that cb wanted to quit fast

    return graph.forEachIn(nodeId, function(from, data) {
      if (from.id === nodeId) return; // this was handled before.

      var cbData = {
        fromId: from.id,
        toId: nodeId
      };
      if (data) cbData.data = data;
      return cb(from, cbData);
    });
  }

  function forEachNode(cb) {
    if (typeof cb !== 'function') throw new Error('Callback is expected to be a function');

    return graph.forEachNode(cb);
  }

  function forEachLink(cb) {
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

  function getLinks(nodeId) {
    var links = [];
    forEachLinkedNode(nodeId, function(_, link) {
      links.push(link);
    });

    return links;
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
