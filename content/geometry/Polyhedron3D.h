/**
 * Author: wleung_bvg
 * Description: Polyhedron functions
 */
#include "Point3D.h"
#include "Sphere3D.h"

pt3 vectorArea2(const vector<pt3> &face) {
  pt3 ret(0, 0, 0); for (int i = 0, n = face.size(); i < n; i++)
    ret += face[i] * face[(i + 1) % n];
  return ret;
}

T faceArea(const vector<pt3> &face) {
  return abs(vectorArea2(face)) / T(2);
}

// Flips some of the faces of a polyhedron such that they
// are oriented consistently
void reorient(vector<vector<pt3>> &faces) {
  int n = faces.size(); vector<vector<pair<int, bool>>> G(n);
  map<pair<pt3, pt3>, int> seen; for (int v = 0; v < n; v++)
    for (int i = 0, m = faces[v].size(); i < m; i++) {
      pt3 a = faces[v][i], b = faces[v][(i + 1) % m];
      auto it = seen.find(make_pair(a, b)); bool f = true;
      if (it == seen.end()) {
        it = seen.find(make_pair(b, a)); f = false;
      }
      if (it == seen.end()) seen[make_pair(a, b)] = v;
      else {
        int w = it->second; G[v].emplace_back(w, f);
        G[w].emplace_back(v, f);
      }
    }
  vector<char> flip(n, -1); vi q(n); int front = 0, back = 0;
  flip[q[back++] = 0] = 0; while (front < back) {
    int v = q[front++];
    for (auto &&e : G[v]) if (flip[e.first] == -1)
      flip[q[back++] = e.first] = flip[v] ^ e.second;
  }
  for (int v = 0; v < n; v++) if (flip[v])
    reverse(faces[v].begin(), faces[v].end());
}

// faces must be the same orientation
T getSurfaceArea(const vector<vector<pt3>> &faces) {
  T sa = 0; for (auto &&face : faces) sa += faceArea(face);
  return sa;
}

// faces must be the same orientation
T getVolume6(const vector<vector<pt3>> &faces) {
  T vol6 = 0; for (auto &&face : faces)
    vol6 += (vectorArea2(face) | face[0]);
  return vol6;
}

int isInPolyhedron(const vector<vector<pt3>> &faces, pt3 p) {
  T sum = 0, PI = acos(T(-1)); Sphere3D s(p, 1);
  for (auto &&face : faces) {
    pt3 a = face[0], b = face[1], c = face[2], n = (b-a)*(c-a);
    if (eq((n | p) - (n | a), 0)) return 0;
    sum += remainder(s.surfaceAreaOnSph(face), 4 * PI);
  }
  return eq(sum, 0) ? -1 : 1;
}
