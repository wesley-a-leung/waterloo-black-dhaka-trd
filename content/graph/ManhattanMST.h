/**
 * Author: wleung_bvg
 * Description: Computes the minimum spanning tree of a complete graph of N points where the edge weights is equal to the Manhattan distance between the points |xi - xj| + |yi - yj|. Generates up to 4N candidate edges with each point connected to its nearest neighbour in each octant.
 */
template <class T> struct ManhattanMST : public KruskalMST<T> {
  using Edge = typename KruskalMST<T>::Edge;
  static vector<Edge> generateCandidates(vector<pair<T, T>> P){
    vector<int> id(P.size()); iota(id.begin(), id.end(), 0);
    vector<Edge> ret; ret.reserve(P.size() * 4); rep(h, 0, 4) {
      sort(id.begin(), id.end(), [&] (int i, int j) {
        return P[i].first - P[j].first
            < P[j].second - P[i].second;
      });
      map<T, int> M; for (int i : id) {
        auto it = M.lower_bound(-P[i].second);
        for (; it != M.end(); it = M.erase(it)) {
          int j = it->second; T dx = P[i].first - P[j].first;
          T dy = P[i].second - P[j].second; if (dy > dx) break;
          ret.emplace_back(i, j, dx + dy);
        }
        M[-P[i].second] = i;
      }
      for (auto &&p : P) {
        if (h % 2) p.first = -p.first;
        else swap(p.first, p.second);
      }
    }
    return ret;
  }
  ManhattanMST(const vector<pair<T, T>> &P)
      : KruskalMST<T>(P.size(), generateCandidates(P)) {}
};
