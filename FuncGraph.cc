#include <bits/stdc++.h>
#include <cassert>
using namespace std;
using ll = long long int;

/*
  Functional Graph

  Usage:

    ll N; cin >> N;
    FuncGraph fg(N);
    for (ll i = 0; i < N; i++) {
      int a; cin >> a; a--;
      fg.add_edge(i, a);
    }
    fg.build();  // optional

    numcc = fg.numcc();        // the number of connected components
    c = fg.cc(nd);             // the id of the connected component containing node nd
    nd = fg.repr(c);           // the representative node of connected component c.  nd is on a cycle.
    sz1 = fg.cc_size(c);       // the size of connected component c
    sz2 = fg.cycle_size(c);    // the size of the cycle of connected component c
    nd1 = fg.node_on_cycle(nd);// the first node on the cycle in the trajectory beginning with nd
    b = fg.is_on_cycle(nd);    // true if nd is on a cycle
    d = fg.dist_to_cycle(nd);  // the distance to the cycle
    vec = fg.bwd(nd);          // a vector of all predecessors of nd.  If nd is on a cycle, so is vec[0].
 */

//////////////////////////////////////////////////////////////////////
// See help of libins command for dependency spec syntax.
// @@ !! BEGIN()    ---- FuncGraph.cc

struct FuncGraph {
  int size;
  vector<int> fwd;
  vector<vector<int>> _bwd;
  vector<int> _cc;
  vector<int> _repr;
  vector<int> _cc_size;
  vector<int> _cycle_size;
  vector<int> _on_cycle;
  vector<int> _dp;
  bool _built;

  FuncGraph(int size_) : size(size_), fwd(size), _bwd(size), _cc(size, -1), _repr(), _cc_size(), _cycle_size(),
                         _on_cycle(size), _dp(size, -1), _built(false) {}

  void add_edge(int i, int j) {
    fwd[i] = j;
    _bwd[j].push_back(i);   // _bwd[j][0] might be changed afterwards
  };

  int _set_depth(int i) {
    if (_dp[i] < 0) {
      if (_on_cycle[i] == i) _dp[i] = 0;
      else _dp[i] = _set_depth(fwd[i]) + 1;
    }
    return _dp[i];
  }

  void build() {
    if (_built) throw runtime_error("FuncGraph: built() should be called at most once.");
    for (int i = 0; i < size; i++) {
      if (_cc[i] < 0) {
        int tmpidx = _cc_size.size();
        vector<int> stack;
        int k;
        for (k = i; _cc[k] < 0; k = fwd[k]) {
          _cc[k] = tmpidx;
          stack.push_back(k);
        }
        if (_cc[k] == tmpidx) {
          _repr.push_back(k);
          _cc_size.push_back(stack.size());
          bool on = true;
          while (not stack.empty()) {
            int m = stack.back(); stack.pop_back();
            if (on) {
              _on_cycle[m] = m;
              auto& v = _bwd[fwd[m]];
              for (int p = 0; p < ssize(v); p++) {
                if (v[p] == m) {
                  if (p > 0) swap(v[0], v[p]);
                  break;
                }
              }
            }else {
              _on_cycle[m] = k;
            }
            if (m == k) {
              _cycle_size.push_back(_cc_size[tmpidx] - stack.size());
              on = false;
            }
          }
        }else {
          _cc_size[_cc[k]] += stack.size();
          while (not stack.empty()) {
            int m = stack.back(); stack.pop_back();
            _on_cycle[m] = _on_cycle[k];
            _cc[m] = _cc[k];
          }
        }
      }
    }
    for (int i = 0; i < size; i++) _set_depth(i);
    _built = true;
  }

  ll numcc() {
    if (not _built) build();
    return (ll)_cc_size.size();
  }

  ll cc(int i) {
    if (not _built) build();
    return _cc[i];
  }

  ll repr(int c) {
    if (not _built) build();
    return _repr[c];
  }

  ll cc_size(int c) {
    if (not _built) build();
    return _cc_size[c];
  }

  ll cycle_size(int c) {
    if (not _built) build();
    return _cycle_size[c];
  }

  ll node_on_cycle(int i) {
    if (not _built) build();
    return _on_cycle[i];
  }

  bool is_on_cycle(int i) {
    if (not _built) build();
    return _dp[i] == 0;
  }

  ll dist_to_cycle(int i) {
    if (not _built) build();
    return _dp[i];
  }

  const vector<int>& bwd(int i) {
    if (not _built) build();
    return _bwd[i];
  }


};

// @@ !! END ---- FuncGraph.cc
