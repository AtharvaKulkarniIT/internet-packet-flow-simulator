#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <tuple>
using namespace std;

// Graph structure with number of nodes (n), source, sink,
// and edges represented as (u, v, capacity)
struct Graph
{
    int n, src, sink;
    vector<tuple<int, int, int>> edges;
};

#endif // GRAPH_H
