#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;

/*
  cycle detction  or  loop detection

  Three types are supported: functional graph, directed graph, undirected graph.

  auto [vec, idx] = func_cycle_detect(n, init, f);

      f : {0, ..., n-1} -> {0, ..., n-1}  function
      The function makes sequence init, f(init), ff(init), fff(init), ....
      i.e.  a_0 = init, a_{k+1} = f(a_k).
      It returns (vec, idx) where vec = [a_0, a_1, ..., a_idx, ..., a_{K-1}] with a_K = a_idx.
      I.e. vec consists of the prefix and the cycle, and idx is the index where the cycle starts.
  
  
  auto [vec, idx] = directed_cycle_detect(n, init, fwd);
  auto [vec, idx] = undirected_cycle_detect(n, init, nbr);

      fwd is in type vector<vector<int>> and represents the directed relation.
      bwd is in type vector<vector<int>> and represents the undirected relation.
      The underlying set should be {0, 1, ..., n-1}
      If a cycle is reachable from init, they return (vec, idx) where vec is the prefix + cycle
      and idx is the index (in vec) of the node where the cycle begins.
      If no cyle is reachable from init, it they return empty vector and -1.

 */

//////////////////////////////////////////////////////////////////////
// See help of libins command for dependency spec syntax.
// @@ !! BEGIN() ---- cycledetect.cc

pair<vector<ll>, ll> func_cycle_detect(ll n, ll init, auto f) {
  vector<ll> ret;
  vector<ll> record(n, -1);
  ll a = init;
  for (ll i = 0; true; i++) {
    if (record[a] >= 0) return {move(ret), record[a]};
    ret.push_back(a);
    record[a] = i;
    a = f(a);
  }
}

template<bool directed>
pair<vector<ll>, ll> graph_cycle_detect(ll n, ll init, const auto& fwd) {
  struct myexc : exception {};
  vector<ll> ret;
  vector<ll> record(n, -1);
  vector<bool> visited(n);
  ll ret_idx = -1;
  auto dfs = [&](auto rF, ll nd, ll idx, ll pt) -> void {
    if (record[nd] >= 0) {
      ret_idx = record[nd];
      throw myexc();
    }
    if (visited[nd]) return;
    visited[nd] = true;
    ret.push_back(nd);
    record[nd] = idx;
    for (ll cld : fwd[nd]) {
      if constexpr (not directed) {
        if (cld == pt) continue;
      }
      rF(rF, cld, idx + 1, nd);
    }
    record[nd] = -1;
    ret.pop_back();
  };
  try {
    dfs(dfs, init, 0, -1);
    return {vector<ll>(), -1};
  }catch (myexc& e) {
    return {move(ret), ret_idx};
  }
}

pair<vector<ll>, ll> directed_cycle_detect(ll n, ll init, const auto& fwd) {
  return graph_cycle_detect<true>(n, init, fwd);
}
pair<vector<ll>, ll> undirected_cycle_detect(ll n, ll init, const auto& nbr) {
  return graph_cycle_detect<false>(n, init, nbr);
}

// @@ !! END ---- cycledetect.cc

