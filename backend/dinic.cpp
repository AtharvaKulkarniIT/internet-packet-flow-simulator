#include "dinic.h"
#include <queue>
#include <climits>
#include <vector>
#include <sstream>
using namespace std;

struct DEdge
{
    int v, cap, flow, rev;
};

struct Dinic
{
    int V;
    vector<vector<DEdge>> adj;
    vector<int> level, start;

    Dinic(int V) : V(V)
    {
        adj.resize(V);
        level.assign(V, -1);
        start.assign(V, 0);
    }

    void addEdge(int u, int v, int cap)
    {
        DEdge a{v, cap, 0, (int)adj[v].size()};
        DEdge b{u, 0, 0, (int)adj[u].size()};
        adj[u].push_back(a);
        adj[v].push_back(b);
    }

    bool bfs(int s, int t)
    {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            for (auto &e : adj[u])
            {
                if (level[e.v] < 0 && e.flow < e.cap)
                {
                    level[e.v] = level[u] + 1;
                    q.push(e.v);
                }
            }
        }
        return level[t] >= 0;
    }

    int sendFlow(int u, int t, int flow)
    {
        if (u == t)
            return flow;
        for (int &i = start[u]; i < adj[u].size(); i++)
        {
            DEdge &e = adj[u][i];
            if (level[e.v] == level[u] + 1 && e.flow < e.cap)
            {
                int curr_flow = min(flow, e.cap - e.flow);
                int temp_flow = sendFlow(e.v, t, curr_flow);
                if (temp_flow > 0)
                {
                    e.flow += temp_flow;
                    adj[e.v][e.rev].flow -= temp_flow;
                    return temp_flow;
                }
            }
        }
        return 0;
    }

    int maxFlow(int s, int t)
    {
        if (s == t)
            return -1;
        int total = 0;
        while (bfs(s, t))
        {
            fill(start.begin(), start.end(), 0);
            while (int flow = sendFlow(s, t, INT_MAX))
                total += flow;
        }
        return total;
    }
};

void runDinic(const Graph &graph)
{
    int n = graph.n;
    int s = graph.src, t = graph.sink;
    Dinic dinic(n);
    // Save original edges to report flows later.
    vector<tuple<int, int, int>> originalEdges = graph.edges;
    for (auto &edge : graph.edges)
    {
        int u, v, cap;
        u = std::get<0>(edge);
        v = std::get<1>(edge);
        cap = std::get<2>(edge);
        dinic.addEdge(u, v, cap);
    }
    int max_flow = dinic.maxFlow(s, t);

    // Compose JSON output.
    stringstream ss;
    ss << "{";
    ss << "\"algorithm\":\"Dinic Algorithm\",";
    ss << "\"maxFlow\":" << max_flow << ",";
    ss << "\"edgeFlows\":[";
    for (int i = 0; i < originalEdges.size(); i++)
    {
        int u, v, cap;
        tie(u, v, cap) = originalEdges[i];
        int flowFound = 0;
        for (auto &e : dinic.adj[u])
        {
            if (e.v == v)
            {
                flowFound = e.flow;
                break;
            }
        }
        ss << "{";
        ss << "\"u\":" << u << ",";
        ss << "\"v\":" << v << ",";
        ss << "\"cap\":" << cap << ",";
        ss << "\"flow\":" << flowFound;
        ss << "}";
        if (i < originalEdges.size() - 1)
            ss << ",";
    }
    ss << "]";
    ss << "}";
    cout << ss.str();
}

/*
TC Analysis:
  - Worst-case: O(V^2 * E) but typically O(E * sqrt(V)) in many cases.
SC Analysis:
  - O(V + E)
*/
