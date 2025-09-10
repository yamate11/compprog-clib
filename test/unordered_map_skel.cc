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
    safe_umap<ll, ll> mp;
    mp[10] = mp[20] + 100;
    assert(mp.find(0) == mp.end());
    assert(mp.find(10) != mp.end());
    assert(mp.find(20) != mp.end());
    assert(mp[10] == 100);
    assert(mp[20] == 0);
  }

  {
    safe_uset<ll> ms1;
    ms1.insert(8);
    ms1.insert(10);
    ms1.insert(8);
    assert(ssize(ms1) == 2);
    safe_umultiset<int> mms2;
    mms2.insert(5);
    mms2.insert(10);
    mms2.insert(5);
    assert(ssize(mms2) == 3);
    safe_umap<int, vector<int>> mp3;
    mp3[10] = vector<int>{5, 7};
    mp3[5].push_back(2);
    assert(ssize(mp3) == 2 and ssize(mp3[5]) == 1);
  }

  {
    safe_uset<ll> ms;
    ms.insert(10000);
    ms.insert(20000);
    ms.insert(30000);
    assert(ms.contains(20000));
    assert(not ms.contains(15000));
    ms.erase(20000);
    assert(ms.find(20000) == ms.end());
  }

  {
    safe_umultiset<ll> mms;
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
  
  {
    string s1 = "abc", s2 = "axx", s3 = "", s4 = "zzz";
    safe_umap<string, ll> mp{{s1, 10}, {s2, 20}, {s3, 30}};
    assert(mp.contains(s2) and not mp.contains(s4));
    auto it = mp.find(s1);
    assert(it->second == 10);
  }

  {
    using tp = pair<int, pair<bool, string>>;
    auto cr = [](int i, bool b, string s) -> tp { return make_pair(i, make_pair(b, s)); };
    safe_umap<tp, int> mp;
    mp[cr(10, true, "happy")] = 3;
    mp[cr(5, false, "fish")] = 7;
    mp[cr(-4, true, "evening")] = 2;
    assert(mp.contains(cr(10, true, "happy")));
    auto it1 = mp.find(cr(5, true, "fish"));
    assert(it1 == mp.end());
    auto it2 = mp.find(cr(5, false, "fish"));
    assert(it2 != mp.end() and it2->second == 7);
  }

  cerr << "ok." << endl;
}

