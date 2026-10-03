#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;

/*
  UnionFind with Undo operations

  see https://yamate11.github.io/blog/posts/2024/01-01-undo-union-fild/

*/

//////////////////////////////////////////////////////////////////////
// See help of libins command for dependency spec syntax.
// @@ !! BEGIN()    ---- UndoUnionFind.cc

struct UndoUnionFind {
  ll size;
  vector<ll> _parent;
  vector<ll> _gsize;
  vector<pair<ll, ll>> _history;

  UndoUnionFind(ll size_) : size(size_), _parent(size), _gsize(size, 1), _history() {
    iota(_parent.begin(), _parent.end(), 0);
  }

  ll leader(ll i) {
    for (; _parent[i] != i; i = _parent[i]);
    return i;
  }

  ll gsize(ll i) { return _gsize[i]; }

  ll merge(ll i, ll j) {
    ll x = leader(i);
    ll y = leader(j);
    if (x == y) {
      _history.emplace_back(-1, -1);
      return x;
    }else {
      if (_gsize[x] < _gsize[y]) swap(x, y);
      _history.emplace_back(y, x);
      _parent[y] = x;
      _gsize[x] += _gsize[y];
      return x;
    }
  }

  void undo() {
    if (_history.empty()) { throw runtime_error("UndoUnionFInd: Fatal.  Empty history stack."); }
    auto [y, x] = _history.back(); _history.pop_back();
    if (y >= 0) {
      _parent[y] = y;
      _gsize[x] -= _gsize[y];
    }
  }
};

// @@ !! END ---- UndoUnionFind.cc
