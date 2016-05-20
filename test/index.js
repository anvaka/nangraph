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
