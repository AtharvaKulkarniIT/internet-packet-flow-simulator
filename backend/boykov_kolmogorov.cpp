#include "boykov_kolmogorov.h"
#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <sstream>
#include <climits>
using namespace std;

void runBoykovKolmogorov(const Graph &graph)
{
    int n = graph.n, s = graph.src, t = graph.sink;
    // Build capacity and adjacency
    vector<vector<int>> capacity(n, vector<int>(n, 0)), flow(n, vector<int>(n, 0));
    vector<vector<int>> adj(n);
    for (auto &e : graph.edges) {
        int u,v,c; tie(u,v,c)=e;
        capacity[u][v] += c;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // label:  1 = in S-tree; -1 = in T-tree; 0 = free
    vector<int> label(n,0), parent(n,-1);
    queue<int> active, orphans;

    // Initialize: add source and sink roots
    label[s]=1;  active.push(s);
    label[t]=-1; active.push(t);

    auto findAugmentingEdge = [&]() -> pair<int,int> {
        // Growth phase: find edge between trees
        int u = active.front(); active.pop();
        for (int v: adj[u]) {
            if (capacity[u][v] - flow[u][v] <= 0) continue;
            if (label[v] == 0) {
                // Grow same tree
                label[v] = label[u];
                parent[v] = u;
                active.push(v);
            }
            else if (label[v] == -label[u]) {
                // Found augmenting path between u and v
                return {u,v};
            }
        }
        return {-1,-1};
    };

    auto augment = [&](int u, int v) {
        // Determine bottleneck along s->…->u->v->…->t
        int bottleneck = INT_MAX, cur;
        // u back to s
        cur = u;
        while (cur != s) {
            int p = parent[cur];
            bottleneck = min(bottleneck, capacity[p][cur] - flow[p][cur]);
            cur = p;
        }
        // edge u->v
        bottleneck = min(bottleneck, capacity[u][v] - flow[u][v]);
        // v back to t
        cur = v;
        while (cur != t) {
            int p = parent[cur];
            // But when v is in T-tree, parent links go towards t via reverse edges
            bottleneck = min(bottleneck, capacity[cur][p] - flow[cur][p]);
            cur = p;
        }
        // Augment
        // u->v
        flow[u][v] += bottleneck;
        flow[v][u] -= bottleneck;
        // s->...->u
        cur = u;
        while (cur != s) {
            int p = parent[cur];
            flow[p][cur] += bottleneck;
            flow[cur][p] -= bottleneck;
            if (capacity[p][cur] - flow[p][cur] == 0)
                orphans.push(cur);
            cur = p;
        }
        // v -> ... -> t (reverse direction)
        cur = v;
        while (cur != t) {
            int p = parent[cur];
            flow[cur][p] += bottleneck;
            flow[p][cur] -= bottleneck;
            if (capacity[cur][p] - flow[cur][p] == 0)
                orphans.push(cur);
            cur = p;
        }
        return bottleneck;
    };

    auto adopt = [&]() {
        // Adoption phase: re-assign or remove orphans
        while (!orphans.empty()) {
            int u = orphans.front(); orphans.pop();

            int side = label[u];
            bool found_new_parent = false;
            // Search neighbors for same-tree connection with residual > 0
            for (int v: adj[u]) {
                if (label[v] != side) continue;
                // For source-tree: need capacity[v][u] - flow[v][u] > 0
                // For sink-tree: need capacity[u][v] - flow[u][v] > 0
                bool ok = (side==1)
                          ? (capacity[v][u] - flow[v][u] > 0)
                          : (capacity[u][v] - flow[u][v] > 0);
                if (!ok) {
                    continue;
                }
                // Check v has a path to its root
                int cur = v;
                while (cur!=-1 && cur!= (side==1? s:t)) cur = parent[cur];
                if (cur == (side==1? s:t)) {
                    parent[u] = v;
                    found_new_parent = true;
                    break;
                }
            }
            if (!found_new_parent) {
                // Remove u from tree
                label[u] = 0;
                // Its children become orphans
                for (int v: adj[u]) {
                    if (parent[v]==u) orphans.push(v);
                }
                parent[u] = -1;
            }
        }
    };

    int max_flow = 0;
    
    while (!active.empty()) {
        pair<int, int> result = findAugmentingEdge();
        int u = result.first;
        int v = result.second;
        if (u < 0) {
            continue; // growth continues
        }
        // augment
        max_flow += augment(u,v);
        // adoption
        adopt();
    }

    // JSON Output
    stringstream ss;
    ss << "{";
    ss << "\"algorithm\":\"Boykov-Kolmogorov Algorithm \",";
    ss << "\"maxFlow\":" << max_flow << ",";
    ss << "\"edgeFlows\":[";
    bool first=true;
    for (auto &e: graph.edges) {
        int u,v,c; tie(u,v,c)=e;
        int f = flow[u][v];
        if (!first) ss << ",";
        ss << "{\"u\":"<<u<<",\"v\":"<<v<<",\"cap\":"<<c<<",\"flow\":"<<f<<"}";
        first=false;
    }
    ss << "]}";
    cout << ss.str();
}

/*
TC Analysis (practical): roughly O(E * max_flow) with tree reuse optimizations.
SC Analysis: O(V + E) for trees + O(V) for queues.
*/
