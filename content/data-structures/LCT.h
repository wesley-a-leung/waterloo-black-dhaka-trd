/**
 * Author: wleung_bvg
 * Description: Link Cut Tree
 */
#include "Splay.h"
template <class Node>
struct LCT : public Splay<Node, vector<Node>> {
  using Tree = Splay<Node, vector<Node>>; using Tree::TR;
  using Tree::makeNode; using Tree::splay; using Tree::select;
  using Data = typename Node::Data;
  using Lazy = typename Node::Lazy;
  int vert(Node *x) { return x - TR.data(); }
  Node *access(Node *x) {
    Node *last = nullptr; for (Node *y = x; y; y = y->p) {
      splay(y); y->r = last; last = y;
    }
    splay(x); return last;
  }
  void makeRoot(Node *x) { access(x); x->reverse(); }
  Node *findMin(Node *x) {
    for (x->propagate(); x->l; (x = x->l)->propagate());
    splay(x); return x;
  }
  Node *findMax(Node *x) {
    for (x->propagate(); x->r; (x = x->r)->propagate());
    splay(x); return x;
  }
  void makeRoot(int x) { makeRoot(&TR[x]); }
  int lca(int x, int y) {
    if (x == y) return x;
    access(&TR[x]); Node *ny = access(&TR[y]);
    return TR[x].p ? vert(ny) : -1;
  }
  bool connected(int x, int y) { return lca(x, y) != -1; }
  void link(int x, int y) { makeRoot(y); TR[y].p = &TR[x]; }
  bool safeLink(int x, int y) {
    if (connected(x, y)) return false;
    link(x, y); return true;
  }
  bool linkParent(int par, int ch) {
    access(&TR[ch]); if (TR[ch].l) return false;
    TR[ch].p = &TR[par]; return true;
  }
  bool cut(int x, int y) {
    makeRoot(x); access(&TR[y]);
    if (&TR[x] != TR[y].l || TR[x].r) return false;
    TR[y].l->p = nullptr; TR[y].l = nullptr; return true;
  }
  bool cutParent(int x) {
    access(&TR[x]); if (!TR[x].l) return false;
    TR[x].l->p = nullptr; TR[x].l = nullptr; return true;
  }
  int findParent(int x) {
    access(&TR[x]);
    return TR[x].l ? vert(findMax(TR[x].l)) : -1;
  }
  int findRoot(int x) {
    access(&TR[x]); return vert(findMin(&TR[x]));
  }
  int depth(int x) {
    access(&TR[x]); return TR[x].l ? TR[x].l->sz : 0;
  }
  int kthParent(int x, int k) {
    int d = depth(x); Node *nx = &TR[x];
    return k <= d ? vert(select(nx, d - k)) : -1;
  }
  void updateVertex(int x, const Lazy &v) {
    access(&TR[x]); Node *l = TR[x].l; TR[x].l = nullptr;
    TR[x].apply(v); TR[x].propagate(); TR[x].update();
    TR[x].l = l;
  }
  void updatePathFromRoot(int to, const Lazy &v) {
    access(&TR[to]); TR[to].apply(v);
  }
  void updatePath(int from, int to, const Lazy &v) {
    makeRoot(from); access(&TR[to]);
    if (from != to && !TR[from].p) return false;
    TR[to].apply(v); return true;
  }
  Data queryVertex(int x) { access(&TR[x]); return TR[x].val; }
  Data queryPathFromRoot(int to) {
    access(&TR[to]); return TR[to].sbtr;
  }
  Data queryPath(int from, int to) {
    makeRoot(from); access(&TR[to]);
    return from == to || TR[from].p
        ? TR[to].sbtr : Node::qdef();
  }
  LCT(const vector<Data> &A) {
    TR.reserve(A.size()); for (auto &&a : A) makeNode(a);
  }
};
