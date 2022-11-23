/**
 * Author: wleung_bvg
 * Description: Given a list of edges representing an undirected flow graph,
 * returns edges of the Gomory-Hu tree. The max flow between any pair of
 * vertices is given by minimum edge weight along the Gomory-Hu tree path.
 * Time: $O(V)$ Flow Computations
 */
#include "HopcroftKarp.h"
template <class T> struct GomoryHu {
  using Edge = tuple<int, int, T>; vector<Edge> treeEdges;
  GomoryHu(int V, const vector<Edge> &edges, T EPS) {
    PushRelabelMaxFlow<FlowEdge<T>> mf(V, EPS);
    for (auto &&e : edges)
      mf.addEdge(get<0>(e), get<1>(e), get<2>(e), get<2>(e));
    vector<int> par(V, 0); rep(i, 1, V) {
      rep(v, 0, V) for (auto &&e : mf.G[v]) e.resCap = e.cap;
      treeEdges.emplace_back(i, par[i], mf.getFlow(i, par[i]));
      rep(j, i + 1, V) if (par[j] == par[i] && mf.cut[j])
        par[j] = i;
    }
  }
};
