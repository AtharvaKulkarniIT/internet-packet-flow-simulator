#include "mcmf.h"
#include <climits>
#include <vector>
#include <queue>
#include <sstream>
#include <algorithm>
using namespace std;

struct Edge
{
    int u, v, cap, cost, flow;
};

void runMCMF(const Graph &graph)
{
    int n = graph.n, s = graph.src, t = graph.sink;
    vector<Edge> edges;
    vector<vector<int>> adj(n);

    auto addEdge = [&](int u, int v, int cap, int cost)
    {
        Edge a{u, v, cap, cost, 0};
        Edge b{v, u, 0, -cost, 0};
        adj[u].push_back(edges.size());
        edges.push_back(a);
        adj[v].push_back(edges.size());
        edges.push_back(b);
    };

    // Insert each graph edge with a cost of 1 (adjust if needed)
    for (auto &edge : graph.edges)
    {
        int u, v, cap;
        tie(u, v, cap) = edge;
        addEdge(u, v, cap, 1);
    }

    int flow = 0, cost = 0;
    while (true)
    {
        vector<int> dist(n, INT_MAX), parent(n, -1), parentEdge(n, -1);
        dist[s] = 0;
        // Bellman-Ford to compute shortest path by cost.
        for (int i = 0; i < n - 1; i++)
        {
            for (int u = 0; u < n; u++)
            {
                if (dist[u] == INT_MAX)
                    continue;
                for (int idx : adj[u])
                {
                    Edge &e = edges[idx];
                    if (e.cap - e.flow > 0 && dist[e.v] > dist[u] + e.cost)
                    {
                        dist[e.v] = dist[u] + e.cost;
                        parent[e.v] = u;
                        parentEdge[e.v] = idx;
                    }
                }
            }
        }
        if (dist[t] == INT_MAX)
            break;
        int pushFlow = INT_MAX;
        for (int v = t; v != s; v = parent[v])
            pushFlow = min(pushFlow, edges[parentEdge[v]].cap - edges[parentEdge[v]].flow);
        for (int v = t; v != s; v = parent[v])
        {
            edges[parentEdge[v]].flow += pushFlow;
            edges[parentEdge[v] ^ 1].flow -= pushFlow;
        }
        flow += pushFlow;
        cost += pushFlow * dist[t];
    }

    // Build JSON output.
    stringstream ss;
    ss << "{";
    ss << "\"algorithm\":\"Min-Cost Max-Flow (MCMF) Algorithm\",";
    ss << "\"maxFlow\":" << flow << ",";
    ss << "\"cost\":" << cost << ",";
    ss << "\"edgeFlows\":[";
    bool first = true;
    // For each original edge, search in edges array.
    for (auto &edge : graph.edges)
    {
        int u, v, cap;
        tie(u, v, cap) = edge;
        int flowFound = 0;
        // Find the corresponding edge in our edges vector.
        for (int i = 0; i < edges.size(); i += 2)
        { // only even indices represent the original direction.
            if (edges[i].u == u && edges[i].v == v && edges[i].cap == cap)
            {
                flowFound = edges[i].flow;
                break;
            }
        }
        if (!first)
            ss << ",";
        ss << "{";
        ss << "\"u\":" << u << ",";
        ss << "\"v\":" << v << ",";
        ss << "\"cap\":" << cap << ",";
        ss << "\"flow\":" << flowFound;
        ss << "}";
        first = false;
    }
    ss << "]";
    ss << "}";
    cout << ss.str();
}

/*
TC Analysis:
  - Worst-case: O(E * F * V) where F is total flow.
SC Analysis:
  - O(E + V)
*/
