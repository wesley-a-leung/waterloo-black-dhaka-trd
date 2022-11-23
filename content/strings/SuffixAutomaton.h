/**
 * Author: wleung_bvg
 * Description: Suffix Automaton with the root node at 0
 * Each distinct path from the root is a substring of the set of words inserted
 */

template <class _T> struct SAMMapNode {
  using T = _T; int len, link; map<T, int> to;
  SAMMapNode(int len) : len(len), link(-1) {}
  bool hasEdge(const T &a) const { return to.count(a); }
  int getEdge(const T &a) const { return to.at(a); }
  void setEdge(const T &a, int n) { to[a] = n; }
};

template <class Node> struct SuffixAutomaton {
  using T = typename Node::T; vector<Node> TR; int last;
  SuffixAutomaton() : TR(1, Node(0)), last(0) {}
  void add(const T &a) {
    int u = -1; if (!TR[last].hasEdge(a)) {
      u = TR.size(); TR.emplace_back(TR[last].len + 1);
      for (; last != -1 && !TR[last].hasEdge(a);
            last = TR[last].link)
        TR[last].setEdge(a, u);
      if (last == -1) { TR[last = u].link = 0; return; }
    }
    int p = TR[last].getEdge(a);
    if (TR[p].len == TR[last].len + 1) {
      (u == -1 ? last : TR[last = u].link) = p; return;
    }
    int q = TR.size(); TR.push_back(TR[p]);
    TR[q].len = TR[last].len + 1;
    TR[p].link = q; if (u != -1) TR[u].link = q;
    while (last != -1 && TR[last].hasEdge(a)
          && TR[last].getEdge(a) == p) {
      TR[last].setEdge(a, q); last = TR[last].link;
    }
    last = u == -1 ? q : u;
  }
  void terminate() { last = 0; }
};
