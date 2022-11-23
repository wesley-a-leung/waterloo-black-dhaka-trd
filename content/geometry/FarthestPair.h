/**
 * Author: wleung_bvg
 * Description: Farthest pair of points
 */
#include "Point.h"
#include "ConvexHull.h"
struct FarthestPair {
  pt best1, best2; T bestDistSq; vector<pt> hull;
  FarthestPair(const vector<pt> &P)
      : bestDistSq(0), hull(convexHull(P)) {
    int H = hull.size(); pt o(0, 0);
    for (int i = 0, j = H < 2 ? 0 : 1; i < j; i++)
      for (;; j = (j + 1) % H) {
        T dSq = distSq(hull[i], hull[j]);
        if (lt(bestDistSq, dSq)) {
          bestDistSq = dSq; best1 = hull[i]; best2 = hull[j];
        }
        pt a = hull[i+1]-hull[i], b = hull[(j+1)%H]-hull[j];
        if (ccw(o, a, b) <= 0) break;
      }
  }
};
