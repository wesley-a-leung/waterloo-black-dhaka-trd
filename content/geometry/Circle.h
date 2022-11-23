/**
 * Author: wleung_bvg
 * Description: Opeartions with circles
 */
#include "Point.h"
#include "Angle.h"
#include "Line.h"
struct Circle {
  pt o; T r; Circle(T r = 0) : o(0, 0), r(r) {}
  Circle(pt o, T r) : o(o), r(r) {}
  int contains(pt p) const {return sgn(r * r - distSq(o, p));}
  int contains(Circle c) const {
    T dr = r - c.r;
    return lt(dr, 0) ? -1 : sgn(dr * dr - distSq(o, c.o));
  }
  int disjoint(Circle c) const {
    T sr = r + c.r; return sgn(distSq(o, c.o) - sr * sr);
  }
  pt proj(pt p) const { return o + (p - o) * r / dist(o, p); }
  pt inv(pt p) const { return o + (p - o)*r*r / distSq(o, p); }
};

vector<pt> circleLineIntersection(Circle c, Line l) {
  vector<pt> ret; T h2 = c.r * c.r - l.distSq(c.o);
  pt p = l.proj(c.o); if (eq(h2, 0)) ret.push_back(p);
  else if (lt(0, h2)) {
    pt h = l.v * sqrt(h2) / abs(l.v);
    ret.push_back(p - h); ret.push_back(p + h);
  }
  return ret;
}

vector<pt> circleSegIntersection(Circle c, pt a, pt b) {
  vector<pt> ret;
  if (a == b) { if (c.contains(a) == 0) ret.push_back(a); }
  else {
    Line l(a, b); for (auto &&p : circleLineIntersection(c, l))
      if (l.cmpProj(a, p) <= 0 && l.cmpProj(p, b) <= 0)
        ret.push_back(p);
  }
  return ret;
}

T circleHalfPlaneIntersectionArea(Circle c, Line l) {
  T h2 = c.r * c.r - l.distSq(c.o), ret = 0; if (!lt(h2, 0)) {
    pt p = l.proj(c.o), h = l.v * sqrt(max(h2, T(0)))/abs(l.v);
    pt a = p - h, b = p + h; T theta = abs(ang(a, c.o, b));
    ret = c.r * c.r * (theta - sin(theta)) / 2;
  }
  if (l.onLeft(c.o) > 0) ret = acos(T(-1)) * c.r * c.r - ret;
  return ret;
}

// first point is guaranteed to not be on the left side of
// line from c1.o to c2.o
int circleCircleIntersection(Circle c1, Circle c2,
                             vector<pt> &res) {
  pt d = c2.o - c1.o; T d2 = norm(d);
  if (eq(d2, 0)) return eq(c1.r, c2.r) ? 2 : 0;
  T pd = (d2 + c1.r * c1.r - c2.r * c2.r) / 2;
  T h2 = c1.r * c1.r - pd * pd / d2; pt p = c1.o + d * pd / d2;
  if (eq(h2, 0)) res.push_back(p);
  else if (lt(0, h2)) {
    pt h = perp(d) * sqrt(h2 / d2);
    res.push_back(p - h); res.push_back(p + h);
  }
  return !res.empty();
}

T circleCircleIntersectionArea(Circle c1, Circle c2) {
  T d = dist(c1.o, c2.o); if (!lt(d, c1.r + c2.r)) return 0;
  if (!lt(c2.r, d + c1.r)) return acos(T(-1)) * c1.r * c1.r;
  if (!lt(c1.r, d + c2.r)) return acos(T(-1)) * c2.r * c2.r;
  auto A = [&] (T r1, T r2) {
    T a = (d * d + r1 * r1 - r2 * r2) / (2 * d * r1);
    T theta = 2 * acos(max(T(-1), min(T(1), a)));
    return r1 * r1 * (theta - sin(theta)) / 2;
  };
  return A(c1.r, c2.r) + A(c2.r, c1.r);
}

// each pair represents a point on the first circle and the
// second circle; first point in each pair is guaranteed to not
// be on the left side of the line from c1.o to c2.o
int circleCircleTangent(Circle c1, Circle c2, bool inner,
                        vector<pair<pt, pt>> &res) {
  pt d = c2.o - c1.o; T r2 = inner ? -c2.r : c2.r;
  T dr = c1.r - r2, d2 = norm(d), h2 = d2 - dr * dr;
  if (eq(d2, 0) || lt(h2, 0)) return eq(h2, 0) ? 2 : 0;
  for (T sign : {T(-1), T(1)}) {
    pt v = (d * dr + perp(d) * sqrt(max(h2, T(0))) * sign)/d2;
    res.emplace_back(c1.o + v * c1.r, c2.o + v * r2);
  }
  return 1;
}

Circle circumcircle(pt a, pt b, pt c) {
  b -= a; c -= a;
  pt ret = b*c*(conj(c) - conj(b)) / (b*conj(c) - conj(b)*c);
  return Circle(a + ret, abs(ret));
}

template <class T, class Cmp = less<T>>
vector<pair<T, T>> &intervalUnion(vector<pair<T, T>> &A,
                                  Cmp cmp = Cmp()) {
  sort(A.begin(), A.end(), [&] (const pair<T, T> &a,
                                const pair<T, T> &b) {
    if (cmp(a.first, b.first)) return true;
    if (cmp(b.first, a.first)) return false;
    return cmp(a.second, b.second);
  });
  int i = 0;
  for (int l = 0, r = 0, N = A.size(); l < N; l = r, i++) {
    A[i] = A[l];
    for (r = l + 1; r < N && !cmp(A[i].second,A[r].first); r++)
      A[i].second = max(A[i].second, A[r].second, cmp);
  }
  A.erase(A.begin() + i, A.end()); return A;
}

T circleUnionArea(const vector<Circle> &circles) {
  int n = circles.size(); T ret = 0; rep(i, 0, n) {
    vector<pair<Angle, Angle>> intervals;
    Angle::setPivot(circles[i].o);
    bool inside = false; rep(j, 0, n) if (i != j) {
      int o = circles[j].contains(circles[i]);
      if (o > 0 || (o == 0 && (lt(circles[i].r, circles[j].r)
          || j < i))) {
        inside = true; break;
      }
      vector<pt> p;
      circleCircleIntersection(circles[i], circles[j], p);
      if (int(p.size()) == 2) {
        Angle a(p[0]), b(p[1]);
        if (a < b) intervals.emplace_back(a, b);
        else {
          intervals.emplace_back(a, Angle(circles[i].o));
          Angle c(circles[i].o - pt(circles[i].r, 0));
          intervals.emplace_back(c, b);
        }
      }
    }
    if (inside) continue;
    if (intervals.empty())
      ret += acos(T(-1)) * circles[i].r * circles[i].r;
    else {
      intervalUnion(intervals);
      if (intervals.back().second == circles[i].o) {
        intervals.front().first = intervals.back().first;
        intervals.pop_back();
      }
      for (int j = 0, k = int(intervals.size()); j < k; j++) {
        pt a = intervals[j].second.p;
        pt b = intervals[j + 1 == k ? 0 : j + 1].first.p;
        ret += cross(a, b) / 2;
        ret += circleHalfPlaneIntersectionArea(circles[i],
                                               Line(b, a));
      }
    }
  }
  return ret;
}
