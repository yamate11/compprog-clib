/*
  Fenwick Tree

  // Usage
  vector<ll> init{5, 2, 4, -3, 1};
  auto fwt = make_fenwick_tree(0LL, plus<ll>(), init);
  // or
     auto fwt = make_fenwick_tree(0LL, plus<ll>());
     fwt.set_data(init);
  //
  fwt.add(1, -3) // becomes {5, -1, 4, -3, 1};
  ll x = fwt.query(3);  // 5 + (-1) + 4
  cerr << fwt;  // "[5, 7, 11, 8, 9]"  You need (f:<<).
 */

//////////////////////////////////////////////////////////////////////
// @@ !! BEGIN() ---- feinwick.cc

template <typename DAT, typename ADD_t>
struct FenwickTree {
  vector<DAT> node;
  DAT unit;
  ADD_t f_add;

  FenwickTree() {}
  FenwickTree(DAT unit_, ADD_t f_add_, const vector<DAT>& idata) : unit(unit_), f_add(f_add_) { set_data(idata); }

  void set_data(const vector<DAT>& idata) {
    node = vector<DAT>(ssize(idata) + 1, unit);
    for (int i = 0; i < ssize(idata); i++) add(i, idata[i]);
  }

  void add(ll i, const DAT& x) {
    for (u64 p = i + 1; p < node.size(); p += 1ULL << countr_zero(p)) node[p] = f_add(node[p], x);
  }

  DAT query(ll n) const {
    DAT ret = unit;
    for (u64 p = n; p; p ^= 1ULL << countr_zero(p)) ret = f_add(ret, node[p]);
    return ret;
  }

  // For debugging.  Returns a string that shows prefix sums. Note that the original array may not be recoverable.
  string show() const {
    string ret = "[";
    for (int i = 0; i < ssize(node) - 1; i++) {
      if (i > 0) ret += ", ";
      ret += to_string(query(i + 1));
    }
    ret += "]";
    return ret;
  }
};

template<typename DAT, typename ADD_t>
auto make_fenwick_tree(DAT unit, ADD_t f_add, const vector<DAT>& idata = vector<DAT>{}) {
  return FenwickTree<DAT, decltype(f_add)>(unit, f_add, idata);
}

// @@ !! END() ---- fenwick.cc
