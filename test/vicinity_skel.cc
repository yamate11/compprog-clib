#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;

// @@ !! LIM(vicinity)

int main() {

  {
    ll big = 1e18;
    set<ll> myset{-big, 0, 4, 5, 7, big};
    auto it_mb = myset.find(-big);
    auto it_0 = myset.find(0);
    auto it_4 = myset.find(4);
    auto it_5 = myset.find(5);
    auto it_7 = myset.find(7);
    auto it_pb = myset.find(big);
    Vicinity vc(myset);
    assert(vc.ge(5) == it_5);
    assert(vc.gt(5) == it_7);
    assert(vc.le(5) == it_5);
    assert(vc.lt(5) == it_4);
    assert(vc.ge(6) == it_7);
    assert(vc.le(6) == it_5);
    assert(vc.ge(big) == it_pb);
    assert(vc.gt(big) == myset.end());
    assert(vc.lt(-big) == myset.end());
  }

  {
    set<ll> mymultiset{0, 2, 2, 4, 4};
    Vicinity vc(mymultiset);
    assert(*vc.ge(2) == 2);
    assert(*vc.gt(2) == 4);
    assert(*vc.le(2) == 2);
    assert(*vc.lt(2) == 0);
    assert(*vc.ge(3) == 4);
    assert(*vc.le(3) == 2);
    assert(*vc.ge(4) == 4);
    assert(vc.gt(4) == mymultiset.end());
    assert(*vc.le(0) == 0);
    assert(vc.lt(0) == mymultiset.end());
  }

  {
    map<ll, ll> mymap{{0, 100}, {2, 102}, {4, 104}};
    Vicinity vc(mymap);
    assert(vc.ge(2)->second == 102);
    assert(vc.gt(2)->second == 104);
    assert(vc.le(2)->second == 102);
    assert(vc.lt(2)->second == 100);
    assert(vc.ge(1)->second == 102);
    assert(vc.le(3)->second == 102);
    assert(vc.gt(4) == mymap.end());
    assert(vc.lt(0) == mymap.end());
  }


  cout << "ok" << endl;
  return 0;
}

