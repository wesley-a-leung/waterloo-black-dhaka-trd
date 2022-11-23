/**
 * Author: wleung_bvg
 * Description: Convex hull trick for maximum value with undo
 */
const bool FIRST = true, LAST = false;
template <const bool ISFIRST, class T, class F>
T bsearch(T lo, T hi, F f) {
  hi--; while (lo <= hi) {
    T mid = lo+(hi-lo)/2; if (f(mid) == ISFIRST) hi = mid-1;
    else lo = mid+1;
  }
  return ISFIRST ? lo : hi;
}

template <class T, class Cmp = less<T>>
struct ConvexHullTrickUndo {
  struct Line {
    T m, b; Line(T m = T(), T b = T()) : m(m), b(b) {}
    T eval(T x) const { return m * x + b; }
  };
  vector<pair<int, Line>> history; vector<Line> L;
  Cmp cmp; int back;
  ConvexHullTrickUndo(Cmp cmp = Cmp()) : cmp(cmp), back(0) {}
  int size() const { return back; }
  void addLine(T m, T b) {
    int i = back; if (i >= 1)
      i = bsearch<LAST>(1, i + 1, [&] (int j) {
        return cmp(m, L[j - 1].m) || cmp(L[j - 1].m, m)
            || cmp(b, L[j - 1].b);
      });
    if (i >= 2)
      i = bsearch<LAST>(2, i + 1, [&] (int j) {
        T c1 = (L[j - 1].m - m) * (L[j - 2].b - b);
        T c2 = (L[j - 1].b - b) * (L[j - 2].m - m);
        return c1 > c2;
      });
    if (i == int(L.size())) L.emplace_back();
    history.emplace_back(back, L[i]);
    L[i] = Line(m, b); back = i + 1;
  }
  void undo() {
    tie(back,L[back - 1]) = history.back(); history.pop_back();
  }
  T getMax(T x) const {
    return L[bsearch<FIRST>(0, back - 1, [&] (int i) {
      return cmp(L[i + 1].eval(x), L[i].eval(x));
    })].eval(x);
  }
  void reserve(int N) { L.reserve(N); history.reserve(N); }
};
