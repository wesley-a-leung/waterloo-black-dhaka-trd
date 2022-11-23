/**
 * Author: wleung_bvg
 * Description: Line operations in 3D
 */
#include "Point3D.h"
// represented in parametric form o + kd for real k
struct Line3D {
  pt3 o, d;
  Line3D(pt3 p=pt3(0,0,0), pt3 q=pt3(0,0,0)) : o(p), d(q-p) {}
  bool onLine(pt3 p) const { return eq(norm(d * (p - o)), 0); }
  T distSq(pt3 p) const { return norm(d * (p - o)) / norm(d); }
  T dist(pt3 p) const { return sqrt(distSq(p)); }
  Line3D translate(pt3 p) const { return Line3D(o + p, d); }
  pt3 proj(pt3 p) const { return o+d*(d|(p-o))/norm(d); }
  pt3 refl(pt3 p) const { return proj(p) * T(2) - p; }
  int cmpProj(pt3 p, pt3 q) const { return sgn((d|p)-(d|q)); }
};

T lineLineDist(Line3D l1, Line3D l2) {
  pt3 n = l1.d*l2.d; if (eq(norm(n), 0)) return l1.dist(l2.o);
  return abs((l2.o - l1.o) | n) / abs(n);
}

pt3 closestOnL1ToL2(Line3D l1, Line3D l2) {
  pt3 n = l1.d * l2.d;
  if (eq(norm(n), 0)) return l1.proj(pt3(0, 0, 0));
  pt3 n2 = l2.d * n;
  return l1.o + l1.d * ((l2.o - l1.o) | n2) / (l1.d | n2);
}

int lineLineIntersection(Line3D l1, Line3D l2, pt3 &res) {
  pt3 n = l1.d * l2.d;
  if (eq(norm(n), 0)) return eq(l1.dist(l2.o), 0) ? 2 : 0;
  res = closestOnL1ToL2(l1, l2); return 1;
}

bool onSeg(pt3 p, pt3 a, pt3 b) {
  if (a == b) return p == a;
  Line3D l(a, b);
  return l.onLine(p) && l.cmpProj(a, p) <= 0
      && l.cmpProj(p, b) <= 0;
}

vector<pt3> segSegIntersection(pt3 a, pt3 b, pt3 p, pt3 q) {
  vector<pt3> ret; if (a == b) {
    if (onSeg(a, p, q)) ret.push_back(a);
  } else if (p == q) {
    if (onSeg(p, a, b)) ret.push_back(p);
  } else {
    pt3 inter; Line3D l1(a, b), l2(p, q);
    int cnt = lineLineIntersection(l1, l2, inter);
    if (cnt == 1) ret.push_back(inter);
    else if (cnt == 2) {
      if (onSeg(p, a, b)) ret.push_back(p);
      if (onSeg(q, a, b)) ret.push_back(q);
      if (onSeg(a, p, q)) ret.push_back(a);
      if (onSeg(b, p, q)) ret.push_back(b);
    }
  }
  sort(ret.begin(), ret.end());
  ret.erase(unique(ret.begin(), ret.end()), ret.end());
  return ret;
}

pt3 closestPtOnSeg(pt3 p, pt3 a, pt3 b) {
  if (a != b) {
    Line3D l(a, b);
    if (l.cmpProj(a,p)<0 && l.cmpProj(p,b)<0) return l.proj(p);
  }
  return lt(dist(p, a), dist(p, b)) ? a : b;
}

T ptSegDist(pt3 p, pt3 a, pt3 b) {
  if (a != b) {
    Line3D l(a, b);
    if (l.cmpProj(a,p)<0 && l.cmpProj(p,b)<0) return l.dist(p);
  }
  return min(dist(p, a), dist(p, b));
}

T segSegDist(pt3 a, pt3 b, pt3 p, pt3 q) {
  return !segSegIntersection(a, b, p, q).empty()
      ? 0
      : min({ptSegDist(p, a, b), ptSegDist(q, a, b),
             ptSegDist(a, p, q), ptSegDist(b, p, q)});
}
