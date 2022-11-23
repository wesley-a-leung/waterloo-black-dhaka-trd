/**
 * Author: wleung_bvg
 * Description: Point operations
 */
#include "EpsCmp.h"
#define OP(op, U, a, x, y) \
  pt operator op (U a) const { return pt(x, y); } \
  pt &operator op##= (U a) { return *this = *this op a; }
#define CMP(op, body) \
  bool operator op (pt p) const { return body; }
struct pt {
  T x, y; constexpr pt(T x = 0, T y = 0) : x(x), y(y) {}
  pt operator + () const { return *this; }
  pt operator - () const { return pt(-x, -y); }
  OP(+, pt, p, x + p.x, y + p.y) OP(-, pt, p, x - p.x, y - p.y)
  OP(*, T, a, x * a, y * a) OP(/, T, a, x / a, y / a)
  friend pt operator * (T a, pt p) {
    return pt(a * p.x, a * p.y);
  }
  bool operator < (pt p) const {
    return eq(x, p.x) ? lt(y, p.y) : lt(x, p.x);
  }
  CMP(<=, !(p < *this)) CMP(>, p < *this) CMP(>=, !(*this < p))
  CMP(==, !(*this < p) && !(p < *this))
  CMP(!=, *this < p || p < *this)
  OP(*, pt, p, x * p.x - y * p.y, y * p.x + x * p.y)
  OP(/, pt, p, (x * p.x + y * p.y) / (p.x * p.x + p.y * p.y),
               (y * p.x - x * p.y) / (p.x * p.x + p.y * p.y))
};
#undef OP
#undef CMP
istream &operator >> (istream &stream, pt &p) {
  return stream >> p.x >> p.y;
}
ostream &operator << (ostream &stream, pt p) {
  return stream << p.x << ' ' << p.y;
}
pt conj(pt a) { return pt(a.x, -a.y); }
T dot(pt a, pt b) { return a.x * b.x + a.y * b.y; }
T cross(pt a, pt b) { return a.x * b.y - a.y * b.x; }
T norm(pt a) { return dot(a, a); }
T abs(pt a) { return sqrt(norm(a)); }
T arg(pt a) { return atan2(a.y, a.x); }
pt polar(T r, T theta) {
  return r * pt(cos(theta), sin(theta));
}
T distSq(pt a, pt b) { return norm(b - a); }
T dist(pt a, pt b) { return abs(b - a); }
T ang(pt a, pt b) { return arg(b - a); }
T ang(pt a, pt b, pt c) {
  a -= b; c -= b; return arg(pt(dot(c, a), cross(c, a)));
}
T area2(pt a, pt b, pt c) { return cross(b - a, c - a); }
int ccw(pt a, pt b, pt c) { return sgn(area2(a, b, c)); }
pt rot(pt a, pt p, T theta) {
  return (a - p) * pt(polar(T(1), theta)) + p;
}
pt perp(pt a) { return pt(-a.y, a.x); }
