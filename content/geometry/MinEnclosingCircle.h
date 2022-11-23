/**
 * Author: wleung_bvg
 * Description: Finds the minimum enclosing circle of a set of points
 */
#include "Point.h"
#include "Circle.h"
Circle minEnclosingCircle(vector<pt> P) {
  mt19937_64 rng(0); shuffle(P.begin(), P.end(), rng);
  Circle c(P[0], 0);
  rep(i, 0, P.size()) if (lt(c.r, dist(P[i], c.o))) {
    c = Circle(P[i], 0);
    rep(j, 0, i) if (lt(c.r, dist(P[j], c.o))) {
      pt p = (P[i] + P[j])/T(2); c = Circle(p, dist(P[i], p));
      rep(k, 0, j) if (lt(c.r, dist(P[k], c.o)))
        c = circumcircle(P[i], P[j], P[k]); 
    }
  }
  return c;
}
