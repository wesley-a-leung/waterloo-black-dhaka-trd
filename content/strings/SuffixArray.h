/**
 * Author: 罗穗骞, chilli
 * Date: 2019-04-11
 * License: Unknown
 * Source: Suffix array - a powerful tool for dealing with strings
 * (Chinese IOI National team training paper, 2009)
 * Description: Builds suffix array for a string.
 * rnk is a vector of the ranks of the suffixes.
 * ind is a vector of the indices in the original array of the suffixes sorted in lexicographical order.
 * LCP is a vector of the longest common prefixes between the suffixes when sorted in lexicographical order.
 * Time: O(n \log n)
 * Status: stress-tested
 */

template <class _T> struct SuffixArray {
  using T = _T; int N; vector<int> ind, rnk, LCP;
  SuffixArray(const vector<T> &S)
      : N(S.size()), ind(N + 1), rnk(N + 1), LCP(N + 1) {
    vector<int> &tmp = LCP; iota(ind.begin(), ind.end(), 0);
    sort(ind.begin(), ind.begin() + N, [&] (int a, int b) {
      return S[a] < S[b];
    });
    rnk[ind[N]] = -1; for (int i = 0; i < N; i++) {
      rnk[ind[i]] = i > 0 && S[ind[i]] == S[ind[i - 1]]
      		? rnk[ind[i - 1]] : i;
    }
    for (int h = 1; h < N; h += h)
    	for (int l = 0, r = 1; r <= N; r++) {
	      if (rnk[ind[r - 1]] != rnk[ind[r]] && l + 1 < r) {
	        sort(ind.begin() + l, ind.begin() + r,
	        		 [&] (int a, int b) {
	          return rnk[h + a] < rnk[h + b];
	        });
	        tmp[l] = l; for (int j = l + 1; j < r; j++)
	          tmp[j] = rnk[h + ind[j - 1]] < rnk[h + ind[j]]
	          		? j : tmp[j - 1];
	        for (l++; l < r; l++) rnk[ind[l]] = tmp[l];
	      } else if (rnk[ind[r - 1]] != rnk[ind[r]]) l++;
	    }
    ind.pop_back(); rnk.pop_back(); tmp.pop_back();
    for (int i = 0, k = 0; i < N; i++) {
      if (rnk[i] == N - 1) { LCP[rnk[i]] = k = 0; continue; }
      int j = ind[rnk[i] + 1];
      while (i + k < N && j + k < N && S[i + k] == S[j + k])
      	k++;
      if ((LCP[rnk[i]] = k) > 0) k--;
    }
  }
};
