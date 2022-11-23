/**
 * Author: wleung_bvg
 * Description: Find a global minimum cut in an undirected graph
 * Time: O(VE \log V)
 * Status: Stress-tested together with GomoryHu
 */
template <class T> struct StoerWagnerGlobalMinCut {
  struct Edge {
    int to, rev; T wt;
    Edge(int to, int rev, T wt) : to(to), rev(rev), wt(wt) {}
  };
  struct Node {
    T w; int v; Node(T w, int v) : w(w), v(v) {}
    bool operator < (const Node &o) const { return w < o.w; }
  };
  int V; vector<vector<Edge>> G; vector<bool> cut;
  T cutWeight, INF;
  void addEdge(int v, int w, T wt) {
    if (v == w) return;
    G[v].emplace_back(w, int(G[w].size()), wt);
    G[w].emplace_back(v, int(G[v].size()) - 1, wt);
  }
  StoerWagnerGlobalMinCut(int V, T INF)
      : V(V), G(V), cut(V, false), cutWeight(INF), INF(INF) {}
  T globalMinCut() {
    vector<vector<Edge>> H = G;
    fill(cut.begin(), cut.end(), false);
    cutWeight = INF; vector<int> par(V);
    iota(par.begin(), par.end(), 0);
    for (int phase = V - 1; phase > 0; phase--) {
      vector<T> W(V, T()); __gnu_pbds::priority_queue<Node> PQ;
      vector<typename decltype(PQ)::point_iterator> ptr(
      		V, PQ.end());
      rep(v, 1, V) if (par[v] == v)
        ptr[v] = PQ.push(Node(W[v], v));
      for (auto &&e : H[0]) if (ptr[e.to] != PQ.end())
        PQ.modify(ptr[e.to], Node(W[e.to] += e.wt, e.to));
      for (int i = 0, v, last = 0; i < phase; i++, last = v) {
        T w = PQ.top().w; v = PQ.top().v; PQ.pop();
        ptr[v] = PQ.end(); if (i == phase - 1) {
          if (cutWeight > w) {
            cutWeight = w; rep(x, 0, V) cut[x] = par[x] == v;
          }
          fill(W.begin(), W.end(), T());
          for (auto &&e : H[v]) W[e.to] += e.wt;
          for (auto &&e : H[last]) {
            e.wt += W[e.to]; H[e.to][e.rev].wt += W[e.to];
            W[e.to] = T();
          }
          for (auto &&e : H[v]) if (W[e.to] != T()) {
            H[e.to][e.rev].to = last;
            H[e.to][e.rev].rev = H[last].size();
            H[last].emplace_back(e.to, e.rev, e.wt);
          }
          H[v].clear();
          rep(x, 0, V) if (par[x] == v) par[x] = last;
        } else {
          for (auto &&e : H[v]) if (ptr[e.to] != PQ.end())
            PQ.modify(ptr[e.to], Node(W[e.to] += e.wt, e.to));
        }
      }
    }
    return cutWeight;
  }
};
