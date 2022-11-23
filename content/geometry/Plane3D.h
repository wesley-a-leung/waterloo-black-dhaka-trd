/**
 * Author: wleung_bvg
 * Description: Plane operations in 3D
 */
#include "Point3D.h"
#include "Line3D.h"
using namespace std;

struct Plane3D {
  pt3 n; T d;
  // ax + by + cz = d, above is ax + by + cz > d
  Plane3D(T a=0, T b=0, T c=0, T d=0) : n(a, b, c), d(d) {}
  // normal n, offset d
  Plane3D(pt3 n, T d) : n(n), d(d) {}
  // normal n, point p
  Plane3D(pt3 n, pt3 p) : n(n), d(n | p) {}
  // 3 non-collinear points p, q, r
  Plane3D(pt3 p, pt3 q, pt3 r) : Plane3D((q - p)*(r - p), p) {}
  T eval(pt3 p) const { return (n | p) - d; }
  int isAbove(pt3 p) const { return sgn(eval(p)); }
  T dist(pt3 p) const { return eval(p) / abs(n); }
  T distSq(pt3 p) const { T e = eval(p); return e*e/norm(n); }
  Plane3D translate(pt3 p) const {return Plane3D(n, d+(n|p));}
  Plane3D shiftUp(T e) const { return Plane3D(n, d+e*abs(n)); }
  pt3 proj(pt3 p) const { return p - n * eval(p) / norm(n); }
  pt3 refl(pt3 p) const { return p - n*T(2)*eval(p)/norm(n); }
  // looking from above
  int ccwProj(pt3 a, pt3 b, pt3 c) const {
    return sgn((b - a) * (c - a) | n);
  }
  // tuple (a, b, c) of 3 non-collinear points on the plane
  tuple<pt3, pt3, pt3> getPts() const {
    pt3 v = pt3(1,0,0); if (eq(abs(n * v), 0)) v = pt3(0,1,0);
    pt3 v1 = n * v, v2 = n * v1; pt3 a = proj(pt3(0,0,0));
    return make_tuple(a, a + v1, a + v2);
  }
};
Line3D perpThrough(Plane3D pi, pt3 o) {
  Line3D ret; ret.o = o; ret.d = pi.n; return ret;
}
Plane3D perpThrough(Line3D l, pt3 o) {return Plane3D(l.d, o);}

// Transforms points to a new coordinate system where the x and
// y axes are on the plane, with the z axis being the normal
// vector (positive z is in the direction of the normal vector)
// Z coordinate is guaranteed to be the distance to the plane
// (positive if above plane, negative if below, 0 if on)
struct CoordinateTransformation {
  pt3 o, dx, dy, dz;
  CoordinateTransformation(Plane3D pi) {
    pt3 p, q, r; tie(p, q, r) = pi.getPts(); o = p;
    dx = unit(q - p); dz = unit(dx * (r - p)); dy = dz * dx;
  }
  CoordinateTransformation(pt3 p, pt3 q, pt3 r) : o(p) {
    dx = unit(q - p); dz = unit(dx * (r - p)); dy = dz * dx;
  }
  pt3 transform(pt3 p) const {
    return pt3((p - o) | dx, (p - o) | dy, (p - o) | dz);
  }
};

int planeLineIntersection(Plane3D pi, Line3D l, pt3 &res) {
  T a = pi.n | l.d;
  if (eq(norm(a), 0)) return pi.isAbove(l.o) == 0 ? 2 : 0;
  res = l.o - l.d * pi.eval(l.o) / a; return 1;
}

int planePlaneIntersection(Plane3D pi1, Plane3D pi2, Line3D &res) {
  pt3 d = pi1.n * pi2.n; if (eq(norm(d), 0))
    return eq(abs(pi1.d / abs(pi1.n)), abs(pi2.d / abs(pi2.n)))
        ? 2 : 0;
  res.o = (pi2.n * pi1.d - pi1.n * pi2.d) * d / norm(d);
  res.d = d; return 1;
}
