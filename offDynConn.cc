#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;

/*
  Offline Dynamic Connectivity

  see https://yamate11.github.io/blog/posts/2024/01-01-offline-dynamic-connectivity/

*/

//////////////////////////////////////////////////////////////////////
// See help of libins command for dependency spec syntax.
// @@ !! BEGIN(UndoUnionFind)    ---- offDynConn.cc

struct OffDynConn { 
  bool built;
  ll size;
  ll tlim; 
  ll node_h_size;
  UndoUnionFind uuf;
  ll seq_idx;
  map<pair<ll, ll>, ll> edge_idx;
  vector<pair<ll, ll>> v_edge;
  vector<vector<pair<ll, ll>>> ops;
  vector<vector<ll>> nodes;

  OffDynConn(ll size_, ll tlim_)
    : built(false), size(size_), tlim(tlim_), node_h_size((ll)bit_ceil((unsigned long long)tlim)),
      uuf(size), seq_idx(0), edge_idx(), v_edge(), ops(), nodes(2 * node_h_size) {}
  
  void set_edge(ll t, ll i, ll j, bool conn) {
    if (i == j) return;
    if (i > j) swap(i, j);
    const pair<int, int> p(i, j);
    ll idx = -1;
    auto it = edge_idx.find(p);
    if (it == edge_idx.end()) {
      idx = seq_idx++;
      v_edge.resize(idx + 1);
      v_edge[idx] = p;
      edge_idx[p] = idx;
      ops.resize(idx + 1);
    }else {
      idx = it->second;
    }
    if (not ops[idx].empty()) {
      ll prev = ops[idx].back().first;
      if (prev > t) throw runtime_error("OffDynConn: edge_set(): time reversed.");
      if (prev == t) ops[idx].pop_back();
    }
    ops[idx].emplace_back(t, conn);
    built = false;
  }

  void _range_set(ll idx, ll t1, ll t2) {
    ll l = node_h_size + t1;
    ll r = node_h_size + t2 - 1;
    while (l <= r) {
      if (l % 2 != 0) nodes[l].push_back(idx);
      l = (l + 1) / 2;
      if (r % 2 == 0) nodes[r].push_back(idx);
      r = (r - 1) / 2;
    }
  }

  void build() {
    if (built) return;
    for (ll idx = 0; idx < seq_idx; idx++) {
      ll t_prev = -1;
      for (auto [t, b] : ops[idx]) {
        if (b) {
          if (t_prev >= 0) continue;
          t_prev = t;
        }else {
          if (t_prev < 0) continue;
          _range_set(idx, t_prev, t);
          t_prev = -1;
        }
      }
      if (t_prev >= 0) _range_set(idx, t_prev, node_h_size);
    }
    built = true;
  }

  void run(auto f) {
    build();
    ll cur = 1;
    for (; cur <= node_h_size; cur *= 2) {
      for (ll i : nodes[cur]) {
        auto [a, b] = v_edge[i];
        uuf.merge(a, b);
      }
    }
    cur = node_h_size;
    ll l = 0, r = 1;
    for (ll t = 0; t < tlim; t++) {
      while (not (l == t and r == t + 1)) {
        if (r <= t) {
          for (ll j = 0; j < (ll)nodes[cur].size(); j++) uuf.undo();
          if (cur % 2 == 0) r += r - l;
          else l -= r - l;
          cur /= 2;
        }else {
          ll m = (l + r) / 2;
          if (t < m) {
            cur = cur * 2;
            r = m;
          }else {
            cur = cur * 2 + 1;
            l = m;
          }
          for (ll j : nodes[cur]) {
            auto [a, b] = v_edge[j];
            uuf.merge(a, b);
          }
        }
      }
      f(t);
    }
  }
};

// @@ !! END ---- offDynConn.cc
