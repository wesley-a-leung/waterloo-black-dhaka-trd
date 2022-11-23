/**
 * Author: wleung_bvg
 * Description: Convex hull
 */
#include "Point.h"
vector<pt> convexHull(vector<pt> P) {
  vector<pt> hull; sort(P.begin(), P.end()); rep(h, 0, 2) {
    int st = hull.size(); for (auto &&p : P) {
      while (int(hull.size()) >= st + 2
          && ccw(hull[hull.size() - 2], hull.back(), p) <= 0)
        hull.pop_back();
      hull.push_back(p);
    }
    hull.pop_back(); reverse(P.begin(), P.end());
  }
  if (int(hull.size()) == 2 && hull[0] == hull[1])
    hull.pop_back();
  if (hull.empty() && !P.empty()) hull.push_back(P[0]);
  return hull;
}
