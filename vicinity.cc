#include <bits/stdc++.h>
#include <cassert>
using namespace std;
using ll = long long int;

/*
  Vicinity

  For set, multiset, and map, gets iterators near to specified value.

  Usage:

    set<ll> myset;
    myset.add(...); ...

    Vicinity vc(myset);
    ll x = ...;
    auto it1 = vc.le(x);   // iterator to the biggest element y x.t. y <= x.  
    auto it2 = vc.lt(x);   // iterator to the biggest element y x.t. y <  x.  
    auto it3 = vc.ge(x);   // iterator to the least element y   x.t. y >= x.  I.e. same as myset.lower_bound()
    auto it4 = vc.gt(x);   // iterator to the least element y   x.t. y >  x.  I.e. same as myset.upper_bound()
    // In all function, myset.end() is returned if there is no such element y.    
    // Thus, using sentinels is still useful.

*/

//////////////////////////////////////////////////////////////////////
// @@ !! BEGIN()  ---- vicinity.cc

template<typename Cont>
struct Vicinity {
  using Key = Cont::key_type;
  Cont& cont;
  Vicinity(Cont& cont_) : cont(cont_) {}
  Cont::iterator _le_lt(const Key& x, bool with_eq) {
    if (with_eq) {
      auto it_eq = cont.find(x);
      if (it_eq != cont.end()) return it_eq;
    }
    auto it = cont.lower_bound(x);
    if (it == cont.begin()) return cont.end();
    return std::prev(it);
  }
  Cont::iterator le(const Key& x) { return _le_lt(x, true); }
  Cont::iterator lt(const Key& x) { return _le_lt(x, false); }
  Cont::iterator ge(const Key& x) { return cont.lower_bound(x); }
  Cont::iterator gt(const Key& x) { return cont.upper_bound(x); }

};

// @@ !! END  ---- vicinity.cc
