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

test('can get in and out edges', function(t) {
  var graph = createGraph();
  graph.addLink('anvaka', 'github');
  graph.addLink('anvaka', 'twitter');

  var anvakaOut = graph.getOut('anvaka');
  var anvakaIn = graph.getIn('anvaka');

  t.equals(anvakaOut.length, 2, 'Two outgoing connections')
  t.equals(anvakaIn.length, 0, 'no incoming nodes')

  var githubIn = graph.getIn('github');
  var githubOut = graph.getOut('github');

  t.equals(githubOut.length, 0, 'no github out')
  t.equals(githubIn.length, 1, 'one github in')

  t.end();
});

test('it can iterate edges with data', function(t) {
  var graph = createGraph();

  graph.addLink('anvaka', 'github', { language: 'js' });
  graph.addLink('anvaka', 'twitter', { language: 'birds' });

  var calledCount = 0;
  graph.forEachOut('anvaka', function(other, data) {
    calledCount += 1;
    var otherId = other.id;
    if (otherId === 'github') {
      t.equals(data.language, 'js', 'github data is here');
    } else if (otherId === 'twitter') {
      t.equals(data.language, 'birds', 'twitter data is here');
    } else {
      t.fail('Unknown other node: ' + otherId);
    }
  });

  t.equals(calledCount, 2, 'All outgoing edges visited');

  calledCount = 0;
  graph.forEachIn('github', function(other, data) {
    calledCount += 1;
    var otherId = other.id;
    if (otherId === 'anvaka') {
      t.equals(data.language, 'js', 'github data is here');
    } else {
      t.fail('only one incoming link for github');
    }
  });

  t.equals(calledCount, 1, 'All incoming edges visited');

  t.end();
});

test('it can iterate over all edges', function(t) {
  var graph = createGraph();

  graph.addLink('anvaka', 'github', { language: 'js' });
  graph.addLink('anvaka', 'twitter', { language: 'birds' });
  var calledCount = 0;

  graph.forEachLink(function(link) {
    calledCount += 1;
    t.equals(link.fromId, 'anvaka', 'anvaka edge is here');
    if (link.toId === 'github') {
      t.equals(link.data.language, 'js', 'github data is here');
    } else if (link.toId === 'twitter') {
      t.equals(link.data.language, 'birds', 'twitter data is here');
    } else {
      t.fail('Unknown edge: ' + link.toId);
    }
  });

  t.equals(calledCount, 2, 'All edges traversed');
  t.end();
});

test('it throw if no node id is passed', function(t) {
  var graph = createGraph();
  t.throws(function() {
    graph.addNode(); // no id, should throw
  }, 'It throws on undefined node');

  t.end();
});

test('it can add node with id similar to reserved prototype property', function(t) {
  var graph = createGraph();
  graph.addNode('constructor');
  graph.addLink('watch', 'constructor');

  var iterated = 0;
  graph.forEachNode(function() {
    iterated += 1;
  });

  t.ok(graph.hasLink('watch', 'constructor'));
  t.equals(graph.getLinksCount(), 1, 'one link');
  t.equal(iterated, 2, 'has two nodes');
  t.end();
});

test('hasLink is the same as getLink', function (t) {
  var graph = createGraph();

  t.equals(graph.getLink, graph.hasLink, 'hasLink is synonim for getLink');
  t.end();
});

test('add link adds link', function(t) {
  var graph = createGraph();

  var link = graph.addLink('1', '2'),
    firstNodeLinks = graph.getLinks('1'),
    secondNodeLinks = graph.getLinks('2');

  t.equal(graph.getNodesCount(), 2, 'Two nodes');
  t.equal(graph.getLinksCount(), 1, 'One link');
  t.equal(firstNodeLinks.length, 1, 'number of links of the first node is wrong');
  t.equal(secondNodeLinks.length, 1, 'number of links of the second node is wrong');
  t.end();
});

function collectGarbage() {
  if (global.gc) {
    global.gc();
  } else {
    console.warn('to test garbage collection tests run node with --expose-gc flag');
  }
}
