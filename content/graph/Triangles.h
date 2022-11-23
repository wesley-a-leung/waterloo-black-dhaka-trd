/**
 * Author: wleung_bvg
 * Description: Runs a callback on the triangles in a simple graph
 */
template <class F>
void triangles(int V, const vector<pair<int, int>> &eds, F f) {
  vi st(V + 1, 0), ind(V, 0), to(eds.size()), eInd(eds.size());
  vector<int> &d = ind;
  for (auto &&e : eds) { d[e.first]++; d[e.second]++; }
  auto cmp = [&] (int v, int w) {
    return d[v] == d[w] ? v > w : d[v] > d[w];
  };
  for (auto &&e : eds)
    st[cmp(e.first, e.second) ? e.second : e.first]++;
  partial_sum(st.begin(), st.end(), st.begin());
  for (int i = 0, v, w; i < int(eds.size()); i++) {
    tie(v, w) = eds[i]; if (cmp(v, w)) swap(v, w);
    to[--st[v]] = w; eInd[st[v]] = i;
  }
  fill(ind.begin(), ind.end(), -1); for (int v = 0; v < V; v++) {
    rep(e1, st[v], st[v + 1]) ind[to[e1]] = eInd[e1];
    rep(e1, st[v], st[v + 1])
      for (int w = to[e1], e2 = st[w]; e2 < st[w + 1]; e2++)
        if (ind[to[e2]] != -1)
          f(v, w, to[e2], eInd[e1], eInd[e2], ind[to[e2]]);
    rep(e1, st[v], st[v + 1]) ind[to[e1]] = -1;
  }
}
