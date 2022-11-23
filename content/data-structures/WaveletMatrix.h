/**
 * Author: wleung_bvg
 * Description: Wavelet Matrix
 */
struct BitPrefixSumArray {
  int M; vector<uint64_t> mask; vector<int> pre;
  BitPrefixSumArray(int N)
      : M((N >> 6) + 1), mask(M, 0), pre(M + 1, 0) {}
  void set(int i, bool v) {
    int j = i >> 6, k = i & 63;
    mask[j] = (mask[j] & ~(uint64_t(1) << k))
               | (uint64_t(v) << k);
  }
  bool get(int i) { return (mask[i >> 6] >> (i & 63)) & 1; }
  void build() {
    for (int j = 0; j < M; j++)
      pre[j + 1] = pre[j] + __builtin_popcountll(mask[j]);
  }
  int query(int r) {
    r++; int j = r >> 6, k = r & 63;
    return pre[j] + __builtin_popcountll(
        mask[j]& ((uint64_t(1) << k) - 1));
  }
  int query(int l, int r) { return query(r) - query(l - 1); }
};
template <class T, class Cmp = less<T>> struct WaveletMatrix {
#define clt [&](const T &a, const T &b) { return cmp(a, b); }
#define cle [&](const T &a, const T &b) { return !cmp(b, a); }
  int N, H; Cmp cmp; vector<int> mid;
  vector<BitPrefixSumArray> B; vector<T> S;
  WaveletMatrix(vector<T> A, Cmp cmp = Cmp())
      : N(A.size()), H(N == 0 ? 0 : __lg(N) + 1), cmp(cmp),
        mid(H), B(H, BitPrefixSumArray(N)), S(move(A)) {
    vector<T> temp = S; sort(S.begin(), S.end(), cmp); vi C(N);
    rep(i, 0, N) C[i] = lower_bound(S.begin(), S.end(),
                                    temp[i], cmp) - S.begin();
    for (int h = H - 1; h >= 0; h--) {
      int ph = 1<<h; rep(i, 0, N) B[h].set(i, C[i] <= ph - 1);
      mid[h] = stable_partition(C.begin(), C.end(), [&](int v){
                                  return v <= ph - 1;
                                }) - C.begin();
      B[h].build(); rep(i, mid[h], N) C[i] -= ph;
    }
  }
  template <class F> int cnt(int l, int r, const T &v, F f) {
    int ret = 0; for (int cur = 0, h = H - 1; h >= 0; h--) {
      int ph = 1<<h, ql = B[h].query(l-1), qr = B[h].query(r);
      if (cur + ph - 1 >= N || f(v, S[cur + ph - 1])) {
        l = ql; r = qr - 1;
      } else {
        cur += ph; ret += qr - ql;
        l += mid[h] - ql; r += mid[h] - qr;
      }
    }
    return ret;
  }
  int rank(int l, int r, const T &v) {
    return cnt(l, r, v, cle);
  }
  int count(int l, int r, const T &lo, const T &hi) {
    return cnt(l, r, hi, clt) - cnt(l, r, lo, cle);
  }
  T select(int l, int r, int k) {
    int cur = 0; for (int h = H - 1; h >= 0; h--) {
      int ql = B[h].query(l - 1), qr = B[h].query(r);
      if (k < qr - ql) { l = ql; r = qr - 1; }
      else {
        cur += 1 << h; k -= qr - ql;
        l += mid[h] - ql; r += mid[h] - qr;
      }
    }
    return S[cur];
  }
#undef clt
#undef cle
};
