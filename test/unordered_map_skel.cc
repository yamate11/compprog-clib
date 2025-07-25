#include <bits/stdc++.h>
#include <cassert>
using namespace std;
using ll = long long int;
using pll = pair<ll, ll>;
// #include <atcoder/all>
// using namespace atcoder;

// @@ !! LIM(unordered_map)

struct A {
  int i;
  vector<int> vi;
  string s;
  A() {}
  A(int i_, vector<int> vi_, string s_) : i(i_), vi(move(vi_)), s(move(s_)) {}
  auto operator<=>(const A&) const = default;
};



int main() {

  /*
  {
    using mymap = gp_hash_table<ll, ll, safe_custom_hash>;
    mymap mp;
    mp[10] = mp[20] + 100;
    assert(mp.find(0) == mp.end());
    assert(mp.find(10) != mp.end());
    assert(mp.find(20) != mp.end());
    assert(mp[10] == 100);
    assert(mp[20] == 0);
  }
  */

  {
    using mymap = unordered_map<ll, ll, safe_custom_hash>;
    mymap mp;
    mp[10] = mp[20] + 100;
    assert(mp.find(0) == mp.end());
    assert(mp.find(10) != mp.end());
    assert(mp.find(20) != mp.end());
    assert(mp[10] == 100);
    assert(mp[20] == 0);
  }

  {
      

  }

  {
    using myset = unordered_set<ll, safe_custom_hash>;
    myset ms;
    ms.insert(10000);
    ms.insert(20000);
    ms.insert(30000);
    assert(ms.contains(20000));
    assert(not ms.contains(15000));
    ms.erase(20000);
    assert(ms.find(20000) == ms.end());
  }

  {
    using mymultiset = unordered_multiset<ll, safe_custom_hash>;
    mymultiset mms;
    mms.insert(10000);
    mms.insert(20000);
    mms.insert(30000);
    mms.insert(20000);
    assert(mms.contains(20000));
    auto it = mms.find(20000);
    assert(it != mms.end() and *it == 20000);
    it = mms.erase(it);
    assert(it != mms.end() and *it == 20000);
    it = mms.erase(it);
    assert(it == mms.end() or *it != 20000);
  }
  
  cerr << "ok." << endl;
}

