/**
 * Author: wleung_bvg
 * Description: Closest pair of points
 */
#include "Point.h"
struct ClosestPair {
  static bool yOrdLt(pt p, pt q) { return lt(p.y, q.y); }
  pt best1, best2; T bestDistSq;
  void closest(vector<pt> &P, int lo, int hi) {
    if (hi <= lo) return;
    int mid = lo + (hi - lo) / 2; pt median = P[mid];
    closest(P, lo, mid);closest(P, mid + 1, hi);vector<pt> aux;
    merge(P.begin()+lo, P.begin()+mid+1, P.begin()+mid+1,
          P.begin() + hi + 1, back_inserter(aux), yOrdLt);
    copy(aux.begin(), aux.end(), P.begin() + lo);
    for (int i = lo, k = 0; i <= hi; i++) {
      T dx = P[i].x - median.x, dx2 = dx * dx;
      if (lt(dx2, bestDistSq)) {
        for (int j = k - 1; j >= 0; j--) {
          T dy = P[i].y - aux[j].y, dy2 = dy * dy;
          if (!lt(dy2, bestDistSq)) break;
          T dSq = distSq(P[i],aux[j]); if (lt(dSq,bestDistSq)){
            bestDistSq = dSq; best1 = P[i]; best2 = aux[j];
          }
        }
        aux[k++] = P[i];
      }
    }
  }
  ClosestPair(vector<pt> P)
      : bestDistSq(numeric_limits<T>::max()) {
    sort(P.begin(), P.end()); closest(P, 0, int(P.size()) - 1);
  }
};
