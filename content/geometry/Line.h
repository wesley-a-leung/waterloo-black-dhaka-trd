/**
 * Author: wleung_bvg
 * Description: Line operations
 */
#include "Point.h"
struct Line {
  pt v; T c;
  // ax + by = c, left side is ax + by > c
  Line(T a = 0, T b = 0, T c = 0) : v(b, -a), c(c) {}
  // direction vector v with offset c
  Line(pt v, T c) : v(v), c(c) {}
  // points p and q
  Line(pt p, pt q) : v(q - p), c(cross(v, p)) {}
  T eval(pt p) const { return cross(v, p) - c; }
  // sign of onLeft, dist: 1 if left of line, 0 if on line, -1 if right of line
  int onLeft(pt p) const { return sgn(eval(p)); }
  T dist(pt p) const { return eval(p) / abs(v); }
  T distSq(pt p) const {T e = eval(p); return e * e / norm(v);}
  Line perpThrough(pt p) const { return Line(p, p + perp(v)); }
  Line translate(pt p) const {return Line(v, c + cross(v, p));}
  Line shiftLeft(T d) const { return Line(v, c + d * abs(v)); }
  pt proj(pt p) const {return p - perp(v) * eval(p) / norm(v);}
  pt refl(pt p) const {
    return p - perp(v) * T(2) * eval(p) / norm(v);
  }
  int cmpProj(pt p, pt q) const {
    return sgn(dot(v, p) - dot(v, q));
  }
};

Line bisector(Line l1, Line l2, bool interior) {
  T s = interior ? 1 : -1;
  return Line(l2.v / abs(l2.v) + l1.v / abs(l1.v) * s,
              l2.c / abs(l2.v) + l1.c / abs(l1.v) * s);
}

int lineLineIntersection(Line l1, Line l2, pt &res) {
  T d = cross(l1.v, l2.v);
  if (eq(d, 0)) return l2.v * l1.c == l1.v * l2.c ? 2 : 0;
  res = (l2.v * l1.c - l1.v * l2.c) / d; return 1;
}

bool onSeg(pt p, pt a, pt b) {
  return !ccw(p, a, b) && !lt(0, dot(a - p, b - p));
}

int segSegIntersects(pt a, pt b, pt p, pt q) {
  if (ccw(a, b, p) * ccw(a, b, q) < 0
      && ccw(p, q, a) * ccw(p, q, b) < 0)
    return 1;
  if (onSeg(p, a, b) || onSeg(q, a, b)
      || onSeg(a, p, q) || onSeg(b, p, q))
    return 2;
  return 0;
}

vector<pt> segSegIntersection(pt a, pt b, pt p, pt q) {
  int intersects = segSegIntersects(a, b, p, q);
  if (!intersects) return vector<pt>();
  if (intersects == 1) {
    T c1 = cross(p - a, b - a), c2 = cross(q - a, b - a);
    return vector<pt>{(c1 * q - c2 * p) / (c1 - c2)};
  }
  vector<pt> ret; if (onSeg(p, a, b)) ret.push_back(p);
  if (onSeg(q, a, b)) ret.push_back(q);
  if (onSeg(a, p, q)) ret.push_back(a);
  if (onSeg(b, p, q)) ret.push_back(b);
  sort(ret.begin(), ret.end());
  ret.erase(unique(ret.begin(), ret.end()), ret.end());
  return ret;
}

pt closestPtOnSeg(pt p, pt a, pt b) {
  if (a == b) return a;
  T d = distSq(a, b), t = min(d, max(T(0), dot(p - a, b - a)));
  return a + (b - a) * t / d;
}

T ptSegDist(pt p, pt a, pt b) {
  if (a == b) return dist(a, p);
  T d = distSq(a, b), t = min(d, max(T(0), dot(p - a, b - a)));
  return abs((p - a) * d - (b - a) * t) / d;
}

T segSegDist(pt a, pt b, pt p, pt q) {
  return segSegIntersects(a, b, p, q) > 0
      ? 0
      : min({ptSegDist(p, a, b), ptSegDist(q, a, b),
             ptSegDist(a, p, q), ptSegDist(b, p, q)});
}
