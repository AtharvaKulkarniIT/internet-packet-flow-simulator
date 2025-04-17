#include "edmonds_karp.h"
#include <climits>
#include <queue>
#include <vector>
#include <sstream>
using namespace std;

void runEdmondsKarp(const Graph &graph)
{
    int n = graph.n, s = graph.src, t = graph.sink;
    vector<vector<int>> capacity(n, vector<int>(n, 0));
    // Save a copy of the original capacities for each edge.
    vector<vector<int>> original(n, vector<int>(n, 0));

    for (auto &edge : graph.edges)
    {
        int u, v, cap;
        u = std::get<0>(edge);
        v = std::get<1>(edge);
        cap = std::get<2>(edge);
        capacity[u][v] += cap;
        original[u][v] = capacity[u][v];
    }
    int max_flow = 0;
    vector<int> parent(n, -1);
    while (true)
    {
        fill(parent.begin(), parent.end(), -1);
        parent[s] = s;
        queue<int> q;
        q.push(s);
        while (!q.empty() && parent[t] == -1)
        {
            int u = q.front();
            q.pop();
            for (int v = 0; v < n; v++)
            {
                if (capacity[u][v] > 0 && parent[v] == -1)
                {
                    parent[v] = u;
                    q.push(v);
                }
            }
        }
        if (parent[t] == -1)
            break;
        int flow = INT_MAX;
        for (int cur = t; cur != s; cur = parent[cur])
            flow = min(flow, capacity[parent[cur]][cur]);
        for (int cur = t; cur != s; cur = parent[cur])
        {
            int u = parent[cur];
            capacity[u][cur] -= flow;
            capacity[cur][u] += flow;
        }
        max_flow += flow;
    }

    // Build JSON output.
    stringstream ss;
    ss << "{";
    ss << "\"algorithm\":\"Edmonds-Karp Algorithm\",";
    ss << "\"maxFlow\":" << max_flow << ",";
    ss << "\"edgeFlows\":[";
    // For each original edge, compute flow as (original - residual)
    bool first = true;
    for (auto &edge : graph.edges)
    {
        int u, v, cap;
        tie(u, v, cap) = edge;
        int flowFound = original[u][v] - capacity[u][v];
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
  - Worst-case: O(V * E^2)
SC Analysis:
  - O(V^2)
*/
