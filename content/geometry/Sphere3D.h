/**
 * Author: wleung_bvg
 * Description: Sphere operations
 */
#include "Point3D.h"
#include "Line3D.h"
#include "Plane3D.h"
struct Sphere3D {
  pt3 o; T r; Sphere3D(T r = 0) : o(0, 0), r(r) {}
  Sphere3D(pt3 o, T r) : o(o), r(r) {}
  int contains(pt3 p) const {return sgn(r * r - distSq(o, p));}
  int contains(Sphere3D s) const {
    T dr = r - s.r;
    return lt(dr, 0) ? -1 : sgn(dr * dr - distSq(o, s.o));
  }
  int disjoint(Sphere3D s) const {
    T sr = r + s.r; return sgn(distSq(o, s.o) - sr * sr);
  }
  pt3 proj(pt3 p) const { return o + (p - o) * r / dist(o, p); }
  pt3 inv(pt3 p) const { return o + (p - o)*r*r/distSq(o, p); }
  T greatCircleDist(pt3 a, pt3 b) const {return r*ang(a,o,b);}
  bool isGreatCircleSeg(pt3 a, pt3 b) const {
    assert(contains(a) == 0 && contains(b) == 0);
    a -= o; b -= o; return !eq(norm(a*b), 0) || lt(0, (a|b)); 
  }
  bool onSphSeg(pt3 p, pt3 a, pt3 b) const {
    p -= o; a -= o; b -= o; pt3 n = a * b;
    if (eq(norm(n), 0)) return eq(norm(a*p),0) && lt(0,(a|p));
    return eq((n|p),0) && !lt((n|a*p),0) && !lt(0,(n|b*p));
  }
  vector<pt3> greatCircleSegIntersection(pt3 a, pt3 b,
      pt3 p, pt3 q) const {
    pt3 ab = (a - o) * (b - o), pq = (p - o) * (q - o);
    int oa = sgn(pq | (a - o)), ob = sgn(pq | (b - o));
    int op = sgn(ab | (p - o)), oq = sgn(ab | (q - o));
    if (oa != ob && op != oq && oa != op)
      return vector<pt3>{proj(o + ab * pq * op)};
    vector<pt3> ret; if (onSphSeg(p, a, b)) ret.push_back(p);
    if (onSphSeg(q, a, b)) ret.push_back(q);
    if (onSphSeg(a, p, q)) ret.push_back(a);
    if (onSphSeg(b, p, q)) ret.push_back(b);
    sort(ret.begin(), ret.end());
    ret.erase(unique(ret.begin(), ret.end()), ret.end());
    return ret;
  }
  T angSph(pt3 a, pt3 b, pt3 c) const {
    a -= o; b -= o; c -= o;
    T theta = ang(b * a, pt3(0, 0, 0), b * c);
    return (a * b | c) < 0 ? -theta : theta;
  }
  T surfaceAreaOnSph(const vector<pt3> &poly) const {
    int n = poly.size(); T PI = acos(T(-1)), a = -(n - 2) * PI;
    for (int i = 0; i < n; i++) {
      T ang = angSph(poly[i], poly[(i+1)%n], poly[(i+2)%n]);
      if (ang < 0) ang += 2 * PI;
      a += ang;
    }
    return r * r * a;
  }
};

vector<pt3> sphereLineIntersection(Sphere3D s, Line3D l) {
  vector<pt3> ret; T h2 = s.r * s.r - l.distSq(s.o);
  pt3 p = l.proj(s.o); if (eq(h2, 0)) ret.push_back(p);
  else if (lt(0, h2)) {
    pt3 h = l.d * sqrt(h2) / abs(l.d);
    ret.push_back(p - h); ret.push_back(p + h);
  }
  return ret;
}

// res contains circle centre and radius
bool spherePlaneIntersection(
    Sphere3D s, Plane3D pi, pair<pt3, T> &res) {
  T d2 = s.r * s.r - pi.distSq(s.o);
  if (lt(d2, 0)) return false;
  res.first = pi.proj(s.o); res.second = sqrt(max(d2, T(0)));
  return true;
}

pair<T, T> sphereHalfSpaceIntersectionSAV(
    Sphere3D s, Plane3D pi) {
  T d2 = s.r * s.r - pi.distSq(s.o);
  T h = lt(d2, 0) ? T(0) : s.r - abs(pi.dist(s.o));
  if (pi.isAbove(s.o) > 0) h = s.r * 2 - h;
  T PI = acos(T(-1));
  return make_pair(PI*2*s.r*h, PI*h*h/3*(3*s.r - h));
}

// c is tuple containing containing the plane the circle lies
// on (pointing away from s1), the center of the circle,
// and the radius
int sphereSphereIntersection(Sphere3D s1, Sphere3D s2,
                             tuple<Plane3D, pt3, T> &c) {
  pt3 d = s2.o - s1.o; T d2 = norm(d);
  if (eq(d2, 0)) return eq(s1.r, s2.r) ? 2 : 0;
  T pd = (d2 + s1.r * s1.r - s2.r * s2.r) / 2;
  T h2 = s1.r * s1.r - pd * pd / d2; if (lt(h2, 0)) return 0;
  pt3 o = s1.o + d * pd / d2;
  c = make_tuple(Plane3D(d,o),o,sqrt(max(h2,T(0)))); return 1;
}

pair<T, T> sphereSphereIntersectionSAV(
    Sphere3D s1, Sphere3D s2) {
  pt3 d = s2.o - s1.o; T d2 = norm(d), dr = abs(s1.r - s2.r);
  T PI = acos(T(-1)); if (!lt(dr * dr, d2)) {
    T r = min(s1.r, s2.r);
    return make_pair(PI * 4 * r * r, PI * 4 * r * r * r / 3);
  }
  T sr = s1.r + s2.r;
  if (lt(sr * sr, d2)) return make_pair(T(0), T(0));
  T pd = (d2 + s1.r * s1.r - s2.r * s2.r) / 2;
  Plane3D pi = Plane3D(d, s1.o + d * pd / d2);
  pair<T, T> a = sphereHalfSpaceIntersectionSAV(s1, pi);
  pair<T, T> b = sphereHalfSpaceIntersectionSAV(
    s2, Plane3D(-pi.n, -pi.d));
  a.first += b.first; a.second += b.second; return a;
}
