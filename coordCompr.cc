#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;

/*
  CoordCompr  -- coordinate compression (one-dimensional)

  CoordCompr<T, Compare>

    Constructors
      CoordCompr()
      CoordCompr(vector<T> vec)
  
    Adding elements
      void add(const T& x)
      void add(const vector<T>& vec)
  
    Compressed value
      int c(const T& x)
  
    Decompressed value
      T d(int i)

    Size
      int size()

  Usage

    vector<ll> vecA(N); cin >> vecA; // or whatever
    CoordCompr cc(move(vecA));       // or just cc(vecA) if you keep vecA
    cc.add(0); cc.add(T);  // or whatever
    int i = cc.c(x1);
    ll x2 = cc.d(i);   // x1 == x2 should hold.

 */

//////////////////////////////////////////////////////////////////////
// See help of libins command for dependency spec syntax.
// @@ !! BEGIN() ---- coordCompr.cc

template <class T = ll, class Compare = std::less<T>>
struct CoordCompr {
  mutable bool built = false;
  mutable vector<T> rev;
  Compare comp;

  CoordCompr() = default;
  explicit CoordCompr(Compare c) : built(false), rev(), comp(move(c)) {}
  explicit CoordCompr(vector<T> v, Compare c = Compare{}) : built(false), rev(std::move(v)), comp(move(c)) {}

  void add(const T& x) { rev.push_back(x); built = false; }

  void add(const vector<T>& v) {
    rev.insert(rev.end(), v.begin(), v.end());
    built = false;
  }

  bool equiv(const T& a, const T& b) const { return !comp(a, b) && !comp(b, a); }

  void build() const {
    if (built) return;
    sort(rev.begin(), rev.end(), comp);
    rev.erase(unique(rev.begin(), rev.end(), [this](const T& a, const T& b) { return equiv(a, b); }),
              rev.end());
    built = true;
  }

  int c(const T& x) const {
    build();
    auto it = lower_bound(rev.begin(), rev.end(), x, comp);
    assert(it != rev.end() && equiv(*it, x));
    return (int)(it - rev.begin());
  }

  const T& d(int i) const {
    build();
    assert(0 <= i and i < (int)rev.size());
    return rev[i];
  }

  int size() const {
    build();
    return (int)rev.size();
  }
};

// @@ !! END ---- coordCompr.cc
