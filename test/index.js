var test = require('tap').test;
var createGraph = require('../index.js');

test('it can get count', function(t) {
  var graph = createGraph();
  var count = graph.getNodesCount();
  t.equals(count, 0, 'no nodes');

  graph.addNode('anvaka');
  count = graph.getNodesCount();
  t.equals(count, 1, 'one added');

  t.end();
});
