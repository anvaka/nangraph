var test = require('tap').test;
var createGraph = require('../index.js');

test('it can count nodes and links', function(t) {
 var graph = createGraph();
 var count = graph.getNodesCount();
 t.equals(count, 0, 'no nodes');
 t.equal(graph.getLinksCount(), 0, 'no links');

 graph.addNode('anvaka');

 t.equal(graph.getLinksCount(), 0, 'no links');
 t.equals(graph.getNodesCount(), 1, 'one added');

 graph.addLink('anvaka', 'github');
 t.equal(graph.getLinksCount(), 1, 'one link');
 t.equals(graph.getNodesCount(), 2, 'two nodes');

 t.end();
});

test('it can add nodes with data', function(t) {
  var graph = createGraph();
  var data = { text: "hello world" };
  graph.addNode('anvaka', data);

  var node = graph.getNode('anvaka');
  t.equals(node.id, 'anvaka', 'id is correct');
  t.equals(node.data, data, 'data is the same');

  t.end();
});

test('hasLink checks links', function (t) {
  var graph = createGraph();

  graph.addLink('anvaka', 'github');

  var link12 = graph.hasLink('anvaka', 'github');
  t.ok(link12.fromId === 'anvaka' && link12.toId === 'github', 'link is found');

  var noLink = graph.hasLink('github', 'anvaka');
  t.notOk(noLink, 'hasLink is always directional');

  var obviousNo = graph.hasLink();
  t.notOk(obviousNo, 'No links there');

  t.end();
});

test('it can add links with data', function(t) {
  var graph = createGraph();
  var data = { text: "hello world" };
  graph.addLink('anvaka', 'github', data);

  var link = graph.getLink('anvaka', 'github');

  t.equals(link.fromId, 'anvaka', 'from id is correct');
  t.equals(link.toId, 'github', 'to id is correct');
  t.equals(link.data, data, 'data is the same');
  t.ok(link.data === data, 'referentially equal');

  t.end();
});

test('data survives garbage collection', function(t) {
  var graph = createGraph();
  var data = { text: 'hello world' };
  graph.addLink('anvaka', 'github', data);

  collectGarbage();

  var link = graph.getLink('anvaka', 'github');

  t.equals(link.fromId, 'anvaka', 'from id is correct');
  t.equals(link.toId, 'github', 'to id is correct');
  t.equals(link.data, data, 'data is the same');
  t.ok(link.data === data, 'referentially equal');

  t.end();
});

test('forEachNode will stop when requested', function(t) {
  var graph = createGraph();
  var calledCount = 0;
  graph.addLink('anvaka', 'github');

  graph.forEachNode(function(node) {
    t.ok(node.id === 'anvaka' || node.id === 'github',  'We visit only one node')
    calledCount += 1;
    return true;
  });

  t.ok(calledCount === 1, 'called only once');
  t.end();
});

test('forEachNode returns data', function(t) {
  var graph = createGraph();
  var calledCount = 0;
  var data = { answer: 42 };
  graph.addNode('anvaka', data);

  graph.forEachNode(function(node) {
    t.equals(node.id, 'anvaka', 'We visit only one node');
    t.equals(node.data, data, 'Data is valid');
    calledCount += 1;
  });

  t.ok(calledCount === 1, 'called only once');
  t.end();
});

test('forEachNode visits all nodes', function(t) {
  var graph = createGraph();
  var calledCount = 0;
  graph.addLink('anvaka', 'github');
  var expect = {};

  graph.forEachNode(function(node) {
    calledCount += 1;
    expect[node.id] = true;
  });

  t.ok(calledCount === 2, 'called for each node');
  t.ok(expect['github'], 'visited github');
  t.ok(expect['anvaka'], 'visited anvaka');
  t.end();
});

function collectGarbage() {
  if (global.gc) {
    global.gc();
  } else {
    console.warn('to test garbage collection tests run node with --expose-gc flag');
  }
}
