#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;

/* 
   Topological Order
     The given graph must be a DAG.

   Usage
     TopOrd to(N);
     to.add_edge(0, 3);
     to.add_edge(4, 2);
     ...
     for (ll nd : to.sorted()) ....   // visits nodes in a topologically sorted order
       for (ll cld : to.forward(nd)) ...  // forward relation

   keywords:  topological sort, dag
*/

//////////////////////////////////////////////////////////////////////
// See help of libins command for dependency spec syntax.
// @@ !! BEGIN() ---- topord.cc

class TopOrd {
  bool built;
  ll n;
  vector<vector<ll>> fwd;
  vector<vector<ll>> bwd;
  vector<ll> ordered;
public:
  TopOrd(ll n_) : built(false), n(n_), fwd(n), bwd(n) {}
  void add_edge(ll x, ll y) {
    built = false;
    fwd[x].push_back(y);
    bwd[y].push_back(x);
  }
  const vector<ll>& sorted() {
    if (not built) {
      ordered.resize(0);
      vector<int> donePrev(n);
      vector<ll> stk;
      for (int i = 0; i < n; i++) if (bwd[i].empty()) stk.push_back(i);
      while (not stk.empty()) {
        ll i = stk.back(); stk.pop_back();
        ordered.push_back(i);
        for (ll j : fwd[i]) if ((int)bwd[j].size() == ++donePrev[j]) stk.push_back(j);
      }
      built = true;
    }
    return ordered;
  }
  ll size() { return n; }
  const vector<ll>& forward(ll x) { return fwd[x]; }
  const vector<ll>& backward(ll x) { return bwd[x]; }
  friend struct TopOrd_test;
};

// @@ !! END() ---- topord.cc
