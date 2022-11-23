/**
 * Author: wleung_bvg
 * Description: Computes the maximum matching (and minimum vertex cover) on an unweighted bipartite graph. The maximum independent set is any vertex not in the minimum vertex cover.
 * mate is the other vertex in the matching, or -1 if unmatched. inCover is whether a vertex is in the minimum vertex cover or not.
 * Time: O(\sqrt{V}E)
 */
struct HopcroftKarpMaxMatch {
  int V, cardinality; vector<int> mate, lvl, q, type0;
  vector<bool> co, inCover, vis;
  template <class BG> bool bfs(const BG &G) {
    int front = 0, back = 0; for (int v : type0) {
      if (mate[v] == -1) lvl[q[back++] = v] = 0;
      else lvl[v] = -1;
    }
    while (front < back) {
      int v = q[front++]; for (int w : G[v]) {
        if (mate[w] == -1) return true;
        else if (lvl[mate[w]] == -1)
        	lvl[q[back++] = mate[w]] = lvl[v] + 1;
      }
    }
    return false;
  }
  template <class BG> bool dfs(const BG &G, int v) {
    for (int w : G[v])
      if (mate[w] == -1
      		|| (lvl[mate[w]] == lvl[v] + 1 && dfs(G, mate[w]))) {
        mate[mate[v] = w] = v; return true;
      }
    lvl[v] = -1; return false;
  }
  template <class BG> void dfs2(const BG &G, int v) {
    if (vis[v]) return;
    vis[v] = true;
    for (int w : G[v]) if ((mate[v] == w) == co[v]) dfs2(G, w);
  }
  template <class BG>
  HopcroftKarpMaxMatch(const BG &G, const vector<bool> &co)
      : V(G.size()), cardinality(0), mate(V, -1), lvl(V), q(V),
        co(co), inCover(V, false), vis(V, false) {
    rep(v, 0, V) if (!co[v]) type0.push_back(v);
    while (bfs(G)) for (int v : type0)
    	if (mate[v] == -1 && dfs(G, v)) cardinality++;
    for (int v : type0) if (mate[v] == -1) dfs2(G, v);
    for (int v = 0; v < V; v++) inCover[v] = vis[v] == co[v];
  }
};
