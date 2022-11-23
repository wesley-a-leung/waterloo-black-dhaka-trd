/**
 * Author: wleung_bvg
 * Description: Push-relabel flow
 * Time: $O(V^2\sqrt E)$
 */
template <class _FU> struct FlowEdge {
  using FU = _FU; int to, rev; FU cap, resCap;
};
template <class Ed> struct PushRelabelMaxFlow {
  using FU = typename Ed::FU;
  int V; FU EPS; vector<vector<Ed>> G; vector<bool> cut;
  PushRelabelMaxFlow(int V, FU EPS)
      : V(V), EPS(EPS), G(V) {}
  void addEdge(int v, int w, FU vwCap, FU wvCap) {
    if (v == w) return;
    G[v].emplace_back(w, int(G[w].size()), vwCap);
    G[w].emplace_back(v, int(G[v].size()) - 1, wvCap);
  }
  FU getFlow(int s, int t) {
    cut.assign(V, false); if (s == t) return FU();
    vector<FU> ex(V, FU()); vector<int> h(V, 0), cnt(V * 2, 0);
    vector<vector<int>> hs(V * 2);
    vector<typename vector<Ed>::iterator> cur(V);
    auto push = [&] (int v, Ed &e, FU df) {
      int w = e.to; if (abs(ex[w]) <= EPS && df > EPS)
      	hs[h[w]].push_back(w);
      e.resCap -= df; G[w][e.rev].resCap += df;
      ex[v] -= df; ex[w] += df;
    };
    h[s] = V; ex[t] = FU(1); cnt[0] = V - 1;
    rep(v, 0, V) cur[v] = G[v].begin();
    for (auto &&e : G[s]) push(s, e, e.resCap);
    if (!hs[0].empty()) for (int hi = 0; hi >= 0;) {
      int v = hs[hi].back(); hs[hi].pop_back();
      while (ex[v] > EPS) {
        if (cur[v] == G[v].end()) {
          h[v] = INT_MAX;
          for (auto e = G[v].begin(); e != G[v].end(); e++)
            if (e->resCap > EPS && h[v] > h[e->to] + 1)
              h[v] = h[(cur[v] = e)->to] + 1;
          cnt[h[v]]++; if (--cnt[hi] == 0 && hi < V)
            rep(w, 0, V) if (hi < h[w] && h[w] < V) {
						  cnt[h[w]]--; h[w] = V + 1;
						}
          hi = h[v];
        } else if (cur[v]->resCap > EPS
        		&& h[v] == h[cur[v]->to] + 1) {
          push(v, *cur[v], min(ex[v], cur[v]->resCap));
        } else cur[v]++;
      }
      while (hi >= 0 && hs[hi].empty()) hi--;
    }
    rep(v, 0, V) cut[v] = h[v] >= V;
    return -ex[s];
  }
};
