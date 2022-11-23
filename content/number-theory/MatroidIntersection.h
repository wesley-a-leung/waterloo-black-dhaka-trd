/**
 * Author: wleung_bvg
 * Description: Find the largest subset of a ground set of N elements that is independent in both matroids
 */

template <class Matroid1, class Matroid2>
struct MatroidIntersection {
  int N; vector<bool> inSet; vector<int> independentSet;
  bool augment(Matroid1 &m1, Matroid2 &m2) {
    vector<int> par(N + 1, -1); queue<int> q; q.push(N);
    while (!q.empty()) {
      int v = q.front(); q.pop(); if (inSet[v]) {
        m1.clear(); for (int i = 0; i < N; i++)
          if (inSet[i] && i != v) m1.add(i);
        rep(i, 0, N)
          if (!inSet[i] && par[i] == -1 && m1.independent(i)) {
            par[i] = v; q.push(i);
          }
      } else {
        auto backE = [&] {
          m2.clear(); rep(c, 0, 2) rep(i, 0, N)
            if ((v == i || inSet[i]) && (par[i] == -1) == c) {
              if (!m2.independent(i)) {
                if (c) { par[i] = v; q.push(i); return i; }
                else return -1;
              }
              m2.add(i);
            }
          return N;
        };
        for (int w = backE(); w != -1; w = backE())
          if (w == N) {
            for (; v != N; v = par[v]) inSet[v] = !inSet[v];
            return true;
          } 
      }
    }
    return false;
  }
  MatroidIntersection(int N, Matroid1 m1, Matroid2 m2)
      : N(N), inSet(N + 1, false) {
    m1.clear(); m2.clear(); inSet[N] = true;
    for (int i = N - 1; i >= 0; i--)
      if (m1.independent(i) && m2.independent(i)) {
        inSet[i] = true; m1.add(i); m2.add(i);
      }
    while (augment(m1, m2));
    inSet.pop_back();
    rep(i, 0, N) if (inSet[i]) independentSet.push_back(i);
  }
};
