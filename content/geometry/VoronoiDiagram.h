/**
 * Author: wleung_bvg
 * Description: The Voronoi Diagram is represented as a graph with the first tri.size() vertices being the circumcenters of each triangle in the triangulation and additional vertices being added to represent infinty points in a certain direction.
 * An edge is added between (v, w) if either triangles v and w share an edge, or if v is a triangle that has an edge that is not shared with any other triangle and rayDir[w - tri.size()] is the direction of the ray from the circumcenter of v passing perpendicular to the edge of the triangle of v that is not shared.
 */
#include "Point.h"
#include "Circle.h"
#include "DelaunayTriangulation.h"
struct VoronoiDiagram {
  vector<array<pt, 3>> tri; vector<Circle> circumcircles;
  vector<pt> rayDir; vector<vector<int>> G;
  VoronoiDiagram(const vector<pt> &P)
      : tri(DelaunayTriangulation(P).tri), G(tri.size()) {
    map<pair<pt, pt>, int> seen;
    circumcircles.reserve(tri.size()); for (auto &&t : tri)
      circumcircles.push_back(circumcircle(t[0], t[1], t[2]));
    rep(h, 0, 2) rep(i, 0, tri.size()) rep(k, 0, 3) {
      pt p1 = tri[i][k], p2 = tri[i][(k + 1) % 3];
      auto it = seen.find(make_pair(p1, p2)); if (h == 0) {
        if (it == seen.end()) it = seen.find(make_pair(p2,p1));
        if (it == seen.end()) seen[make_pair(p1, p2)] = i;
        else {
          int j = it->second;
          G[i].push_back(j); G[j].push_back(i); seen.erase(it);
        }
      } else if (it != seen.end()) {
        int j = G.size(); G.emplace_back();
        rayDir.push_back(perp(p1 - p2));
        G[i].push_back(j); G[j].push_back(i);
      }
    }
  }
};
