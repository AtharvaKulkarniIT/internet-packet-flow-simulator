#include <iostream>
#include <sstream>
#include <vector>
#include <tuple>
#include "graph.h"
#include "dinic.h"
#include "edmonds_karp.h"
#include "goldberg_tarjan.h"
#include "mcmf.h"
#include "boykov_kolmogorov.h"
#include "min_cut_algorithms.h"
using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Usage: flow_solver <algorithm>" << endl;
        return 1;
    }
    string algo = argv[1];

    int n, s, t;
    cin >> n >> s >> t;
    Graph graph;
    graph.n = n;
    graph.src = s;
    graph.sink = t;
    int u, v, cap;
    while (cin >> u >> v >> cap)
    {
        graph.edges.push_back(make_tuple(u, v, cap));
    }

    if (algo == "dinic")
        runDinic(graph);
    else if (algo == "edmonds_karp")
        runEdmondsKarp(graph);
    else if (algo == "goldberg_tarjan")
        runGoldbergTarjan(graph);
    else if (algo == "mcmf")
        runMCMF(graph);
    else if (algo == "bk")
        runBoykovKolmogorov(graph);
    else
        cout << "Unknown algorithm: " << algo << endl;

    return 0;
}
