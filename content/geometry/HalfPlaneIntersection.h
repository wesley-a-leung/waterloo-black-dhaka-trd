/**
 * Author: wleung_bvg
 * Description: Computes the intersection of half-planes defined by the left side of a set of lines (including the line itself).
 * Code 0 = finite intersection, 1 = infinite and bounded by line segs and rays, 2 = infinite and bounded by two lines (4 points given), 3 = entire plane
 */
#include "Point.h"
#include "Angle.h"
#include "Line.h"
pair<int,vector<pt>> halfPlaneIntersection(vector<Line> lines){
  Angle::setPivot(pt(0, 0));
  sort(lines.begin(), lines.end(), [&] (Line a, Line b) {
    Angle angA(a.v), angB(b.v);
    return angA == angB ? a.onLeft(b.proj(pt(0, 0))) < 0
      : angA < angB;
  });
  lines.erase(unique(lines.begin(), lines.end(),
      [&] (Line a, Line b) {
    return Angle(a.v) == Angle(b.v); 
  }), lines.end());
  int N = lines.size();
  if (N == 0) return make_pair(3, vector<pt>());
  if (N == 1) {
    pt p = lines[0].proj(pt(0, 0));
    return make_pair(1, vector<pt>{p, p + lines[0].v});
  }
  int code = 0; for (int i = 0; code == 0 && i < N; i++) {
    Angle diff=Angle(lines[i].v)-Angle(lines[i==0?N-1:i-1].v);
    if (diff < Angle(pt(1, 0))) {
      rotate(lines.begin(), lines.begin() + i, lines.end());
      code = 1; if (N == 2 && diff == Angle(pt(-1, 0))) {
        pt p = lines[0].proj(pt(0, 0));
        if (lines[1].onLeft(p) < 0)
          return make_pair(0, vector<pt>());
        pt q = lines[1].proj(pt(0, 0));
        return make_pair(2, vector<pt>{p, p + lines[0].v,
            q, q + lines[1].v});
      }
    }
  }
  vector<Line> q(N + 1, lines[0]); vector<pt> ret(N); pt inter;
  int front = 0, back = 0; for (int i = 1; i <= N - code; i++) {
    if (i == N) lines.push_back(q[front]);
    while (front < back && lines[i].onLeft(ret[back - 1]) < 0)
      back--;
    while (i != N && front < back
        && lines[i].onLeft(ret[front]) < 0) front++;
    if (lineLineIntersection(lines[i], q[back], inter) != 1)
      continue;
    ret[back++] = inter; q[back] = lines[i];
  }
  if (code == 0 && back - front < 3)
    return make_pair(code, vector<pt>());
  vector<pt> P(ret.begin() + front, ret.begin() + back);
  if (code == 1) {
    P.insert(P.begin(), P[0] - q[front].v);
    P.push_back(P.back() + q[back].v);
  }
  return make_pair(code, P);
}
