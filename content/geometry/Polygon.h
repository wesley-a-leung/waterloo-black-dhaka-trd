/**
 * Author: wleung_bvg
 * Description: Polygon operations
 */
#include "Point.h"
#include "Angle.h"
#include "Line.h"
#include "Circle.h"
int mod(int i, int n) { return i < n ? i : i - n; }

T getArea2(vector<pt> &poly) {
  T ret = 0; int n = poly.size();
  rep(i, 0, n) ret += cross(poly[i], poly[mod(i + 1, n)]);
  return ret;
}

pt getCentroid(vector<pt> &poly) {
  T A2 = 0; pt cen(0, 0); int n = poly.size(); rep(i, 0, n) {
    T a = cross(poly[i], poly[mod(i + 1, n)]); A2 += a;
    cen += a * (poly[i] + poly[mod(i + 1, n)]);
  }
  return cen / A2 / T(3);
}

int isCcwConvexPolygon(vector<pt> &poly) {
  return ccw(poly.back(), poly[0], poly[mod(1, poly.size())]);
}

int isCcwPolygon(vector<pt> &poly) {
  int n = poly.size();
  int i = min_element(poly.begin(), poly.end()) - poly.begin();
  return ccw(poly[mod(i+n-1, n)], poly[i], poly[mod(i+1, n)]);
}

int isInConvexPolygon(vector<pt> &poly, pt p) {
  int n = poly.size(), a = 1, b = n - 1;
  if (n < 3) return onSeg(p, poly[0], poly.back()) ? 0 : -1;
  if (ccw(poly[0], poly[a], poly[b]) > 0) swap(a, b);
  int o1 = ccw(poly[0],p,poly[a]), o2 = ccw(poly[0],p,poly[b]);
  if (o1 < 0 || o2 > 0) return -1;
  if (o1 == 0 || o2 == 0) return 0;
  while (abs(a - b) > 1) {
    int c = (a+b)/2; (ccw(poly[0],p,poly[c]) < 0 ? b : a) = c;
  }
  return ccw(poly[a], p, poly[b]);
}

int isInPolygon(vector<pt> &poly, pt p) {
  int n = poly.size(), windingNumber = 0; rep(i, 0, n) {
    pt a = poly[i], b = poly[mod(i + 1, n)];
    if (lt(b.y, a.y)) swap(a, b);
    if (onSeg(p, a, b)) return 0;
    windingNumber ^= (!lt(p.y,a.y)&&lt(p.y,b.y)&&ccw(p,a,b)>0);
  }
  return windingNumber == 0 ? -1 : 1;
}

// Finds a vertex that is the furthest point in that direction,
// selecting the rightmost vertex if there are multiple
int extremeVertex(vector<pt> &poly, pt dir) {
  int n = poly.size(), lo = 0, hi = n; pt pp = perp(dir);
  auto cmp = [&] (int i, int j) {
    return sgn(cross(pp, poly[mod(i, n)] - poly[mod(j, n)]));
  };
  auto extr = [&] (int i) {
    return cmp(i + 1, i) >= 0 && cmp(i, i + n - 1) < 0;
  };
  if (extr(0)) return 0;
  while (lo + 1 < hi) {
    int m = lo + (hi - lo) / 2; if (extr(m)) return m;
    int ls = cmp(lo + 1, lo), ms = cmp(m + 1, m);
    (ls < ms || (ls == ms && ls == cmp(lo, m)) ? hi : lo) = m;
  }
  return lo;
}

// (-1, -1) if no collision, (i, -1) if touching corner i,
// (i, i) if along side (i, i + 1), (i, j) if crossing sides
// (i, i + 1) and (j, j + 1) crossing corner i is treated as
// crossing side (i, i + 1)
// first index in pair is guaranteed to NOT be on the left side
pair<int, int> convexPolygonLineIntersection(
    vector<pt> &poly, Line l) {
  int n = poly.size();
  if (n == 1) return make_pair(l.onLeft(poly[0])==0?0:-1,-1);
  if (n == 2) {
    int o0 = l.onLeft(poly[0]), o1 = l.onLeft(poly[1]);
    if (o0 == 0 && o1 == 0) return make_pair(0, 0);
    if (o0 == 0) return make_pair(0, -1);
    if (o1 == 0) return make_pair(1, -1);
    return o0 == o1 ? make_pair(-1, -1) : make_pair(0, 1);
  }
  int endA = extremeVertex(poly, -perp(l.v));
  int endB = extremeVertex(poly, perp(l.v));
  auto cmpL = [&] (int i) { return l.onLeft(poly[i]); };
  pair<int, int> ret(-1, -1);
  if (cmpL(endA) > 0 || cmpL(endB) < 0) return ret;
  for (int i = 0; i < 2; i++) {
    int lo = endB, hi = endA; while (mod(lo + 1, n) != hi) {
      int m = mod((lo + hi + (lo < hi ? 0 : n)) / 2, n);
      (cmpL(m) == cmpL(endB) ? lo : hi) = m;
    }
    (i ? ret.second : ret.first) = mod(lo + !cmpL(hi), n);
    swap(endA, endB);
  }
  if (ret.first == ret.second) return make_pair(ret.first, -1);
  if (!cmpL(ret.first) && !cmpL(ret.second)) {
    switch ((ret.first - ret.second + n + 1) % n) {
      case 0: return make_pair(ret.first, ret.first);
      case 2: return make_pair(ret.second, ret.second);
    }
  }
  return ret;
}

// p is considered to be below polygon
int convexPolygonPointSingleTangent(
    vector<pt> &poly, pt p, bool left) {
  int n = poly.size(), o = ccw(p, poly[0], poly.back());
  bool farSide = o ? o < 0
      : lt(distSq(p, poly.back()), distSq(p, poly[0]));
  int lo = farSide != left, hi = lo + n - 2; while (lo <= hi) {
    int mid = lo + (hi - lo) / 2;
    if (ccw(p, poly[0], poly[mid]) == (left ? -1 : 1)) {
      if (farSide == left) hi = mid - 1;
      else lo = mid + 1;
    } else {
      if ((ccw(poly[mid], poly[mod(mid+1, n)], p) < 0) == left)
        hi = mid - 1;
      else lo = mid + 1;
    }
  }
  return mod(lo, n);
}

// c is considered to be below the polygon
pair<int, int> convexPolygonCircleTangent(
    vector<pt> &poly, Circle c, bool inner) {
  int n = poly.size(), a = 0, b = 0; vector<pair<pt, pt>> t;
  for (int h = 0; h < 2; h++) {
    pt q = t[h].second; int o = ccw(q, poly[0], poly.back());
    bool farSide = o ? o < 0
        : lt(distSq(q, poly.back()), distSq(q, poly[0]));
    int lo = farSide == h, hi = lo + n - 2; while (lo <= hi) {
      int mid = lo + (hi - lo) / 2; t.clear();
      q = t[h].second;
      if (ccw(q, poly[0], poly[mid]) == (h ? 1 : -1)) {
        if (farSide != h) hi = mid - 1;
        else lo = mid + 1;
      } else {
        if ((ccw(poly[mid], poly[mod(mid+1, n)], q) < 0) != h)
          hi = mid - 1;
        else lo = mid + 1;
      }
    }
    (h ? b : a) = mod(lo, n); t.clear();
  }
  return make_pair(a, b);
}

// second polygon is considered to be blow the first
vector<pair<int, int>> convexPolygonConvexPolygonTangent(
    vector<pt> &poly1, vector<pt> &poly2, bool inner) {
  int n = poly1.size(), a = 0, b = 0, c = 0, d = 0;
  vector<pair<int, int>> ret; for (int h = 0; h < 2; h++) {
    pt q = poly2[convexPolygonPointSingleTangent(
        poly2, poly1[0], inner ^ h)];
    int o = ccw(q, poly1[0], poly1.back());
    bool farSide = o ? o < 0
        : lt(distSq(q, poly1.back()), distSq(q, poly1[0]));
    int lo = farSide == h, hi = lo + n - 2; while (lo <= hi) {
      int mid = lo + (hi - lo) / 2;
      q = poly2[convexPolygonPointSingleTangent(
          poly2, poly1[mid], inner ^ h)];
      if (ccw(q, poly1[0], poly1[mid]) == (h ? 1 : -1)) {
        if (farSide != h) hi = mid - 1;
        else lo = mid + 1;
      } else {
        if ((ccw(poly1[mid], poly1[mod(mid+1, n)],q) < 0) != h)
          hi = mid - 1;
        else lo = mid + 1;
      }
    }
    (h ? b : a) = lo = mod(lo, n);
    (h ? d : c) = convexPolygonPointSingleTangent(
        poly2, poly1[lo], inner ^ h);
  }
  ret.emplace_back(a, c); ret.emplace_back(b, d); return ret;
}

pt closestPointOnConvexPolygon(vector<pt> &poly, pt p) {
  pair<int, int> tangent = convexPolygonPointTangent(poly, p);
  int n = poly.size(), len = tangent.second - tangent.first;
  if (len < 0) len += n;
  if (len == 0) return poly[tangent.first];
  int lo = 0, hi = len - 2; while (lo <= hi) {
    int mid = lo+(hi-lo)/2, i = mod(tangent.first + mid, n);
    if (ptSegDist(p, poly[i], poly[mod(i + 1, n)])
        < ptSegDist(p, poly[mod(i+1, n)], poly[mod(i+2, n)]))
      hi = mid - 1;
    else lo = mid + 1;
  }
  int i = mod(tangent.first + lo, n);
  return closestPtOnSeg(p, poly[i], poly[mod(i + 1, n)]);
}

vector<pt> polygonHalfPlaneIntersection(
    vector<pt> &poly, Line l) {
  int n = poly.size(); vector<pt> ret; rep(i, 0, n) {
    int j = mod(i + 1, n);
    int o1 = l.onLeft(poly[i]), o2 = l.onLeft(poly[j]);
    if (o1 >= 0) ret.push_back(poly[i]);
    if (o1 && o2 && o1 != o2) {
      pt p; if (lineLineIntersection(
          l, Line(poly[i], poly[j]), p) == 1)
        ret.push_back(p);
    }
  }
  return ret;
}

T polygonUnion(vector<vector<pt>> &polys) {
  auto rat = [&] (pt p, pt q) {
    return sgn(q.x) ? p.x / q.x : p.y / q.y;
  };
  T ret = 0; for (int i = 0; i < int(polys.size()); i++)
    for (int v = 0; v < int(polys[i].size()); v++) {
      pt a=polys[i][v], b=polys[i][mod(v+1,polys[i].size())];
      vector<pair<T, int>> segs{
          make_pair(0, 0), make_pair(1, 0)};
      for (int j = 0; j < int(polys.size()); j++) if (i != j)
        for (int w = 0; w < int(polys[j].size()); w++) {
          pt c = polys[j][w];
          pt d = polys[j][mod(w + 1, polys[j].size())];
          int sc = ccw(a,b,c), sd = ccw(a,b,d); if (sc != sd) {
            if (min(sc, sd) < 0) {
              T sa = area2(c, d, a), sb = area2(c, d, b);
              segs.emplace_back(sa / (sa - sb), sgn(sc - sd));
            }
          } else if (j<i && !sc && !sd && sgn(dot(b-a,d-c))>0){
            segs.emplace_back(rat(c - a, b - a), 1);
            segs.emplace_back(rat(d - a, b - a), -1);
          }
        }
      sort(segs.begin(), segs.end()); T sm = 0;
      for (auto &&s : segs)s.first=min(max(s.first,T(0)),T(1));
      for (int j = 1, cnt = segs[0].second;
          j < int(segs.size()); j++) {
        if (!cnt) sm += segs[j].first - segs[j - 1].first;
        cnt += segs[j].second;
      }
      ret += cross(a, b) * sm;
    }
  return ret / 2;
}

T polygonCircleIntersectionArea(vector<pt> &poly, Circle c) {
  T r2 = c.r * c.r / 2;
  auto tri = [&] (pt p, pt q) {
    pt d = q - p; T a = dot(d, p) / norm(d);
    T b = (norm(p) - c.r * c.r) / norm(d), det = a * a - b;
    if (!lt(0, det)) return ang(q, pt(0, 0), p) * r2;
    T s = max(T(0), -a-sqrt(det)), t = min(T(1), -a+sqrt(det));
    if (lt(t, 0) || !lt(s, 1)) return ang(q, pt(0, 0), p) * r2;
    pt u = p + d * s, v = p + d * t;
    return ang(u, pt(0, 0), p) * r2 + cross(u, v) / 2
        + ang(q, pt(0, 0), v) * r2;
  };
  T ret = 0; for (int n = poly.size(), i = 0; i < n; i++)
    ret += tri(poly[i] - c.o, poly[mod(i + 1, n)] - c.o);
  return ret;
}
