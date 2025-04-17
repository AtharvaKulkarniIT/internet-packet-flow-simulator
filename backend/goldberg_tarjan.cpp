#include "goldberg_tarjan.h"
#include <climits>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

void runGoldbergTarjan(const Graph &graph)
{
    int n = graph.n, s = graph.src, t = graph.sink;
    vector<vector<int>> capacity(n, vector<int>(n, 0));
    vector<vector<int>> flow(n, vector<int>(n, 0));
    vector<vector<int>> adj(n);

    for (auto &edge : graph.edges)
    {
        int u, v, cap;
        tie(u, v, cap) = edge;
        capacity[u][v] += cap;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<int> excess(n, 0), height(n, 0);
    height[s] = n;
    for (int v = 0; v < n; v++)
    {
        flow[s][v] = capacity[s][v];
        flow[v][s] = -flow[s][v];
        excess[v] = capacity[s][v];
        excess[s] -= capacity[s][v];
    }

    auto push = [&](int u, int v) -> bool
    {
        int send = min(excess[u], capacity[u][v] - flow[u][v]);
        if (send > 0 && height[u] == height[v] + 1)
        {
            flow[u][v] += send;
            flow[v][u] -= send;
            excess[u] -= send;
            excess[v] += send;
            return true;
        }
        return false;
    };

    auto relabel = [&](int u)
    {
        int minHeight = INT_MAX;
        for (int v : adj[u])
        {
            if (capacity[u][v] - flow[u][v] > 0)
                minHeight = min(minHeight, height[v]);
        }
        if (minHeight < INT_MAX)
            height[u] = minHeight + 1;
    };

    bool progress = true;
    while (progress)
    {
        progress = false;
        for (int u = 0; u < n; u++)
        {
            if (u == s || u == t)
                continue;
            if (excess[u] > 0)
            {
                for (int v : adj[u])
                {
                    if (push(u, v))
                        progress = true;
                }
                if (excess[u] > 0)
                {
                    relabel(u);
                    progress = true;
                }
            }
        }
    }
    int max_flow = 0;
    for (int v = 0; v < n; v++)
    {
        max_flow += flow[s][v];
    }

    // Build JSON output.
    stringstream ss;
    ss << "{";
    ss << "\"algorithm\":\"Goldberg-Tarjan Algorithm\",";
    ss << "\"maxFlow\":" << max_flow << ",";
    ss << "\"edgeFlows\":[";
    bool first = true;
    for (auto &edge : graph.edges)
    {
        int u, v, cap;
        tie(u, v, cap) = edge;
        int flowFound = 0;
        // Look for flow from u to v in the computed flow matrix.
        flowFound = flow[u][v];
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
  - Worst-case: O(V^2 * E)
SC Analysis:
  - O(V^2)
*/
