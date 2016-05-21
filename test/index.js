var test = require('tap').test;
var createGraph = require('../index.js');

test('it can count nodes', function(t) {
 var graph = createGraph();
 var count = graph.getNodesCount();
 t.equals(count, 0, 'no nodes');

 graph.addNode('anvaka');
 count = graph.getNodesCount();
 t.equals(count, 1, 'one added');

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

  t.end();
});
