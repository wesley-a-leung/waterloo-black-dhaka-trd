/**
 * Author: wleung_bvg
 * Description: Palindromic Tree with two roots at 0 (with length -1) and 1 (with length 0)
 */

template <class _T> struct PalTreeMapNode {
  using T = _T; int len, link, qlink; map<T, int> to;
  PalTreeMapNode(int len) : len(len), link(1), qlink(1) {}
  int getEdge(const T &a) const {
    auto it = to.find(a);
    return it == to.end() ? 1 : it->second;
  }
  void setEdge(const T &a, int n) { to[a] = n; }
};

template <class Node> struct PalindromicTree {
  using T = typename Node::T;
  T def; vector<T> S; vector<Node> TR;
  vector<int> last, modified;
  PalindromicTree(const T &def)
      : def(def), S(1, def),
        TR(vector<Node>{Node(-1), Node(0)}), last(1, 1) {
    TR[1].link = TR[1].qlink = 0;
  }
  int getLink(int x, int i) {
    while (S[i - 1 - TR[x].len] != S[i])
      x = S[i - 1 - TR[TR[x].link].len] == S[i]
          ? TR[x].link : TR[x].qlink;
    return x;
  }
  void add(const T &a) {
    int i = S.size(); S.push_back(a);
    int p = getLink(last.back(), i);
    modified.push_back(-1); if (TR[p].getEdge(a) == 1) {
      int u = TR.size(); TR.emplace_back(TR[p].len + 2);
      TR[u].link = TR[getLink(TR[p].link, i)].getEdge(a);
      T b = S[i - TR[TR[u].link].len];
      T c = S[i - TR[TR[TR[u].link].link].len];
      TR[u].qlink = b == c ? TR[TR[u].link].qlink : TR[u].link;
      TR[modified.back() = p].setEdge(a, u);
    }
    last.push_back(TR[p].getEdge(a));
  }
  void terminate() {
    S.push_back(def); last.push_back(1);
    modified.push_back(-1);
  }
  void undo() {
    if (modified.back() != -1) {
      TR[modified.back()].setEdge(S.back(), 1); TR.pop_back();
    }
    S.pop_back(); last.pop_back(); modified.pop_back();
  }
};
