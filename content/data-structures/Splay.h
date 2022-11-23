/**
 * Author: wleung_bvg
 * Description: Splay Tree
 */
template <class C> struct NodeLazyAgg {
  using Data = typename C::Data; using Lazy = typename C::Lazy;
  bool rev; int sz; NodeLazyAgg *l, *r, *p;
  Lazy lz; Data val, sbtr;
  NodeLazyAgg(const Data &v)
      : rev(false), sz(1), l(nullptr), r(nullptr), p(nullptr),
        lz(C::ldef()), val(v), sbtr(v) {}
  void update() {
    sz = 1; sbtr = val;
    if (l) { sz += l->sz; sbtr = C::merge(l->sbtr, sbtr); }
    if (r) { sz += r->sz; sbtr = C::merge(sbtr, r->sbtr); }
  }
  void propagate() {
    if (rev) {
      if (l) l->reverse();
      if (r) r->reverse();
      rev = false;
    }
    if (lz != C::ldef()) {
      if (l) l->apply(lz);
      if (r) r->apply(lz);
      lz = C::ldef();
    }
  }
  void apply(const Lazy &v) {
    lz = C::mergeLazy(lz, v); val = C::applyLazy(val, v, 1);
    sbtr = C::applyLazy(sbtr, v, sz);
  }
  void reverse() { rev = !rev; swap(l, r); C::revData(sbtr); }
  static Data qdef() { return C::qdef(); }
};

template <class _Node, class Container = deque<_Node>>
struct Splay {
  using Node = _Node; Container TR; deque<Node *> del;
  template <class T> Node *makeNode(const T &v) {
    if (del.empty()) { TR.emplace_back(v); return &TR.back(); }
    Node *x = del.back(); del.pop_back();
    *x = typename Container::value_type(v); return x;
  }
  bool isRoot(Node *x) {
    return !x->p || (x != x->p->l && x != x->p->r);
  }
  void con(Node *x, Node *p, bool hasCh, bool isL) {
    if (x) x->p = p;
    if (hasCh) (isL ? p->l : p->r) = x;
  }
  void rot(Node *x) {
    Node *p = x->p, *g = p->p;
    bool isRootP = isRoot(p), isL = x == p->l;
    con(isL ? x->r : x->l, p, true, isL);
    con(p, x, true, !isL);
    con(x, g, !isRootP, !isRootP && p == g->l); p->update();
  }
  void splay(Node *x) {
    while (!isRoot(x)) {
      Node *p = x->p, *g = p->p; g->propagate();
      p->propagate(); x->propagate();
      if (!isRoot(p)) rot((x == p->l) == (p == g->l) ? p : x);
      rot(x);
    }
    x->propagate(); x->update();
  }
  template <class F>
  void applyToRange(Node *&root, int i, int j, F f) {
    if (i == 0 && j == (root ? root->sz : 0) - 1) {
      f(root); if (root) { root->propagate(); root->update(); }
    } else {
      Node *t = i ? select(root, i - 1)->r
                  : select(root, j + 1)->l;
      con(nullptr, root, true, !i); root->update();
      con(t, nullptr, false, !i); applyToRange(t, 0, j - i, f);
      con(t, root, true, !i); root->update();
    }
  }
  Node *select(Node *&root, int k) {
    Node *last = nullptr; while (root) {
      (last = root)->propagate();
      int t = root->l ? root->l->sz : 0;
      if (t > k) root = root->l;
      else if (t < k) { root = root->r; k -= t + 1; }
      else break;
    }
    if (last) splay(root = last);
    return root;
  }
  int index(Node *&root, Node *x) {
    root = x; if (!root) return -1;
    splay(root); return root->l ? root->l->sz : 0;
  }
  template <class T, class Comp>
  pair<int, Node *> getFirst(Node *&root, const T &v,
                             Comp cmp) {
    pair<int, Node *> ret(0, nullptr); Node *last = nullptr;
    while (root) {
      (last = root)->propagate();
      if (!cmp(root->val, v)) {
        ret.second = root; root = root->l;
      } else {
        ret.first += 1 + (root->l ? root->l->sz : 0);
        root = root->r;
      }
    }
    if (last) splay(root = last);
    return ret;
  }
  template <class T>
  Node *buildRec(const vector<T> &A, int l, int r) {
    if (l > r) return nullptr;
    int m = l + (r - l) / 2;
    Node *left = buildRec(A, l, m - 1);
    Node *ret = makeNode(A[m]), *right = buildRec(A, m + 1, r);
    con(left, ret, ret, true); con(right, ret, ret, false);
    ret->update(); return ret;
  }
  template <class T>
  Node *build(const vector<T> &A) {
    return buildRec(A, 0, int(A.size()) - 1);
  }
  void clear(Node *x) {
    if (!x) return;
    clear(x->l); del.push_back(x); clear(x->r);
  }
};
