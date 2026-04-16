#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;
using pll = pair<ll, ll>;

// @@ !! LIM(random persSegTree)

template <typename DAT, typename OP, typename ADD_t, typename APPL_t>
struct NaiveImpl {
  
  DAT unit_dat;
  OP unit_op;
  ADD_t add;
  APPL_t appl;
  vector<vector<DAT>> data;
  
  int fromVector(const vector<DAT>& vec) {
    data.push_back(vec);
    return ssize(data) - 1;
  }

  DAT query(int idx, ll rngLo, ll rngHi) const {
    ll ret = unit_dat;
    for (int i = rngLo; i < rngHi; i++) ret = add(ret, data[idx][i]);
    return ret;
  }

  int update(int idx, ll rngLo, ll rngHi, const OP& op_asked) {
    vector<DAT> new_vec = data[idx];
    for (int i = rngLo; i < rngHi; i++) new_vec[i] = appl(op_asked, new_vec[i]);
    data.push_back(move(new_vec));
    return ssize(data) - 1;
  }

  int set(int idx, ll i, DAT t) {
    vector<DAT> new_vec = data[idx];
    new_vec[i] = move(t);
    data.push_back(move(new_vec));
    return ssize(data) - 1;
  }
};

template <typename DAT, typename OP>
auto make_naive_impl(DAT unit_dat, OP unit_op, auto add, auto appl, const vector<DAT>& init_data) {
  NaiveImpl<DAT, OP, decltype(add), decltype(appl)> ni(unit_dat, unit_op, add, appl);
  ll idx = ni.fromVector(init_data);
  return make_pair(ni, idx);
}


int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << setprecision(20);

  Random rand;

  {
    auto mymax = [&](ll a, ll b) -> ll { return max(a, b); };
    vector<ll> init_dat{3, 5, 1, 4, 10, 2};
    auto [st, r1] = make_pers_seg_tree(0LL, mymax, init_dat);
    assert(st.query(r1, 0, 6) == 10);
    assert(st.query(r1, 1, 4) == 5);
    auto r2 = st.set(r1, 2, 20);
    assert(st.query(r2, 0, 6) == 20);
    assert(st.query(r2, 1, 4) == 20);
    assert(st.query(r2, 6, 0) == 0);
  }  
  {
    auto mymax = [&](ll a, ll b) -> ll { return max(a, b); };
    vector<ll> init_dat{3, 5, 1, 4, 10, 2};
    auto [st, r1] = make_pers_seg_tree_lazy(0LL, 0LL, mymax, plus<ll>(), plus<ll>(), init_dat);
    assert(st.query(r1, 0, 6) == 10);
    assert(st.query(r1, 1, 4) == 5);
    auto r2 = st.set(r1, 2, 20);
    assert(st.query(r2, 0, 6) == 20);
    assert(st.query(r2, 1, 4) == 20);
    auto r3 = st.update(r2, 3, 5, 100);
    assert(st.query(r3, 0, 1) == 3);
    assert(st.query(r3, 1, 2) == 5);
    assert(st.query(r3, 2, 3) == 20);
    assert(st.query(r3, 3, 4) == 104);
    assert(st.query(r3, 4, 5) == 110);
    assert(st.query(r3, 5, 6) == 2);
    assert(st.query(r3, 6, 0) == 0);
  }  
  ll repeat = 100;
  ll repeat2 = 100;
  {
    for (int _r = 0; _r < repeat; _r++) {
      ll sz = rand.range(1, 11);
      vector<ll> vec(sz);
      for (int i = 0; i < sz; i++) vec[i] = rand.range(-9, 10);
      auto [st, pt0] = make_pers_seg_tree(0LL, plus<ll>(), vec);
      auto [ni, idx0] = make_naive_impl(0LL, 0LL, plus<ll>(), plus<ll>(), vec);
      vector<decltype(pt0)> rec1;
      vector<decltype(idx0)> rec2;
      rec1.push_back(pt0);
      rec2.push_back(idx0);
      for (int _r2 = 0; _r2 < repeat2; _r2++) {
        int j0 = rand.range(0, ssize(rec1));
        int j1 = rand.range(0, sz);
        int x = rand.range(-9, 10);
        auto pt = st.set(rec1[j0], j1, x);
        auto idx = ni.set(rec2[j0], j1, x);
        rec1.push_back(pt);
        rec2.push_back(idx);
        int i = rand.range(0, ssize(rec1));
        int lo = rand.range(0, sz);
        int hi = rand.range(lo + 1, sz + 1);
        ll a1 = st.query(rec1[i], lo, hi);
        ll a2 = ni.query(rec2[i], lo, hi);
        assert(a1 == a2);
      }
    }
  }
  {
    using OP = optional<ll>;
    auto mymin = [&](ll a, ll b) -> ll { return min(a, b); };
    auto comp = [&](OP f, OP g) -> OP { return f ? f : g; };
    auto appl = [&](OP f, ll x) -> ll { return f ? *f : x; };
    ll big = 1e9;
    for (int _r = 0; _r < repeat; _r++) {
      ll sz = rand.range(1, 11);
      vector<ll> vec(sz);
      for (int i = 0; i < sz; i++) vec[i] = rand.range(-9, 10);
      auto [st, pt0] = make_pers_seg_tree_lazy(big, OP{}, mymin, comp, appl, vec);
      auto [ni, idx0] = make_naive_impl(big, OP{}, mymin, appl, vec);
      vector<decltype(pt0)> rec1;
      vector<decltype(idx0)> rec2;
      rec1.push_back(pt0);
      rec2.push_back(idx0);
      for (int _r2 = 0; _r2 < repeat2; _r2++) {
        int j0 = rand.range(0, ssize(rec1));
        int j1 = rand.range(0, sz);
        int x = rand.range(-9, 10);
        auto pt = st.set(rec1[j0], j1, x);
        auto idx = ni.set(rec2[j0], j1, x);
        rec1.push_back(pt);
        rec2.push_back(idx);
        int k0 = rand.range(0, ssize(rec1));
        int k1 = rand.range(0, sz);
        int k2 = rand.range(k1 + 1, sz + 1);
        int y = rand.range(-9, 10);
        auto pt3 = st.update(rec1[k0], k1, k2, y);
        auto idx3 = ni.update(rec2[k0], k1, k2, y);
        rec1.push_back(pt3);
        rec2.push_back(idx3);
        int i = rand.range(0, ssize(rec1));
        int lo = rand.range(0, sz);
        int hi = rand.range(lo + 1, sz + 1);
        ll a1 = st.query(rec1[i], lo, hi);
        ll a2 = ni.query(rec2[i], lo, hi);
        assert(a1 == a2);
      }
    }
  }

  cout << "Ok." << endl;
  return 0;
}

