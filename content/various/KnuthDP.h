/**
 * Author: wleung_bvg
 * License: CC0
 * Source: http://codeforces.com/blog/entry/8219
 * Description: Must satisfy dp[l][r] = max(dp[l][m] + dp[m][r] + cost(l, m, r) for l <= m <= r and max(l + 1, opt[l][r - 1]) <= opt[l][r] <= min(opt[l + 1][r], r - 1), where opt[l][r] is the optimal value of m for dp[l][r]
 * Time: O(N^2)
 */
template <class T, class F, class Cmp = less<T>>
vector<vector<T>> knuth(int N, F f, Cmp cmp = Cmp()) {
  vector<vector<T>> dp(N, vector<T>(N + 1, T()));
  vector<vector<int>> opt(N, vector<int>(N + 1));
  for (int l = N - 1; l >= 0; l--)
    for (int r = l; r <= N; r++) {
      if (r - l <= 1) { opt[l][r] = l; continue; }
      int st = max(l + 1, opt[l][r - 1]);
      int en = min(opt[l + 1][r], r - 1);
      for (int m = st; m <= en; m++) {
        T cost = dp[l][m] + dp[m][r] + f(l, m, r);
        if (m == st || cmp(dp[l][r], cost)) {
          dp[l][r] = cost; opt[l][r] = m;
        }
      }
    }
  return dp;
}
