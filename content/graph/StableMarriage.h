/**
 * Author: wleung_bvg
 * Description: Given N people of type A and M people of type B, and a list of their ranked preferences for partners, the goal is to arrange min(N, M) pairs such that if a person x of type A prefers a person y of type B more than their current partner, then person y prefers their current partner more than x.
 */
struct StableMarriage {
  int N, M; vector<int> bForA, aForB;
  StableMarriage(const vector<vector<int>> &aPrefs,
                 const vector<vector<int>> &bPrefs)
      : N(aPrefs.size()), M(bPrefs.size()),
        bForA(N, -1), aForB(M, -1) {
    bool rv = N > M; if (rv) { swap(N, M); bForA.swap(aForB); }
    auto &A = rv ? bPrefs : aPrefs, &B = rv ? aPrefs : bPrefs;
    vector<vector<int>> bRankOfA(M, vector<int>(N));
    rep(b, 0, M) rep(a, 0, N) bRankOfA[b][B[b][a]] = a;
    queue<int> q; rep(a, 0, N) q.push(a);
    vector<int> cur(N, 0); while (!q.empty()) {
      int a = q.front(); q.pop(); while (true) {
        int b = A[a][cur[a]++];
        if (aForB[b] == -1) { aForB[b] = a; break; }
        else if (bRankOfA[b][a] < bRankOfA[b][aForB[b]]) {
          q.push(aForB[b]); aForB[b] = a; break;
        }
      }
    }
    rep(b, 0, M) if (aForB[b] != -1) bForA[aForB[b]] = b;
    if (rv) { swap(N, M); bForA.swap(aForB); }
  }
};
