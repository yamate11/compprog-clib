#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;

// @@ !! LIM(perm debug)

int cmp_vec(const auto& v1, const auto& v2) {
  ll m = min(v1.size(), v2.size());
  for (int i = 0; i < m; i++) {
    if (v1.at(i) < v2.at(i)) return -1;
    if (v1.at(i) > v2.at(i)) return 1;
  }
  if (v1.size() < v2.size()) return -1;
  if (v1.size() > v2.size()) return 1;
  return 0;
}

void sanity_perm(auto& perm, int exp_cnt) {
    int cnt = 1;
    auto prev = perm.vec_view();
    while (true) {
      if (!perm.get()) break;
      cnt++;
      auto cur = perm.vec_view();
      assert(cmp_vec(prev, cur) == -1);
      prev = move(cur);
    }
    assert(cnt == exp_cnt);
}


bool my_compare(auto& p, const auto& vec) {
  int i = 0;
  while (p.get()) {
    if (i >= (int)vec.size()) return false;
    const auto& view = p.vec_view();
    if (view.size() != vec[i].size()) return false;
    for (int j = 0; j < (int)view.size(); j++) if (view[j] != vec[i][j]) return false;
    i++;
  }
  if (i != (int)vec.size()) return false;
  return true;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << setprecision(20);

  {
    IntPerm ip(6, 4);
    ip.get();
    sanity_perm(ip, 6*5*4*3);
  }
  {
    IntPerm ip(5, 5);
    ip.get();
    sanity_perm(ip, 5*4*3*2*1);
  }
  {
    IntComb ic(5, 2);
    ic.get();
    sanity_perm(ic, (5*4)/(2*1));
  }

  using vvi = vector<vector<int>>;
  {
    IntPerm p1(3, 2);
    assert(my_compare(p1, vvi({      {0,1},{0,2},{1,0},      {1,2},{2,0},{2,1}      })));
    IntComb p2(3, 2);
    assert(my_compare(p2, vvi({      {0,1},{0,2},            {1,2},                 })));
    IntDupPerm p3(3, 2);
    assert(my_compare(p3, vvi({{0,0},{0,1},{0,2},{1,0},{1,1},{1,2},{2,0},{2,1},{2,2}})));
    IntDupComb p4(3, 2);
    assert(my_compare(p4, vvi({{0,0},{0,1},{0,2},      {1,1},{1,2},            {2,2}})));
  }
  {
    IntDupPerm p1(2, 3);
    assert(my_compare(p1, vvi({{0,0,0},{0,0,1},{0,1,0},{0,1,1},{1,0,0},{1,0,1},{1,1,0},{1,1,1}})));
    IntDupComb p2(2, 3);
    assert(my_compare(p2, vvi({{0,0,0},{0,0,1},{0,1,1},{1,1,1}})));
    IntDupComb p3(3, 3);
    assert(my_compare(p3, vvi({{0,0,0},{0,0,1},{0,0,2},{0,1,1},{0,1,2},{0,2,2},{1,1,1},{1,1,2},{1,2,2},{2,2,2}})));
  }
  {
    IntDirProd p1{vector{{1, 2, 3}}};
    assert(my_compare(p1, vvi({{0,0,0},{0,0,1},{0,0,2},{0,1,0},{0,1,1},{0,1,2}})));
    vector<ll> v2{{3, 2, 1}};
    IntDirProd p2(v2);
    assert(my_compare(p2, vvi({{0,0,0},{0,1,0},{1,0,0},{1,1,0},{2,0,0},{2,1,0}})));
  }
  {
    IntPartition p1{5};
    assert(my_compare(p1, vvi{{1,1,1,1,1},{1,1,1,2},{1,1,3},{1,2,2},{1,4},{2,3},{5}}));
  }


  {
    auto chkPerm = [&](ll n, ll r) -> void {
      IntPerm ip(n, r);
      set<string> ss;
      while (ip.get()) {
        string s;
        vector<bool> v(n);
        for (ll i = 0; i < r; i++) {
          assert(0 <= ip[i] and ip[i] < n and ip[i] == ip.at(i));
          assert(not v[ip[i]]);
          v[ip[i]] = true;
          s += 'a' + ip[i];
        }
        assert(ss.find(s) == ss.end());
        ss.insert(s);
      }
      ll num = 1;
      for (ll i = 0; i < r; i++) num = num * (n - i);
      assert((ll)ss.size() == num);
    };
    chkPerm(6, 3);
    chkPerm(8, 5);
  }
  {
    auto chkComb = [&](ll n, ll r) -> void {
      IntComb ic(n, r);
      set<string> ss;
      while (ic.get()) {
        string s;
        ll last = -1;
        for (ll i = 0; i < r; i++) {
          assert(0 <= ic.at(i) and ic.at(i) < n and ic.at(i) == ic[i]);
          assert(last < ic.at(i));
          last = ic.at(i);
          s += 'a' + ic.at(i);
        }
        assert(ss.find(s) == ss.end());
        ss.insert(s);
      }
      ll num = 1;
      for (ll i = 0; i < r; i++) num = num * (n - i);
      for (ll i = 0; i < r; i++) num = num / (r - i);
      assert((ll)ss.size() == num);
    };
    chkComb(6, 4);
    chkComb(8, 6);
  }
  {
    auto chkDupPerm = [&](ll n, ll r) -> void {
      IntDupPerm idp(n, r);
      set<string> ss;
      while (idp.get()) {
        string s;
        for (ll i = 0; i < r; i++) {
          assert(0 <= idp.at(i) and idp.at(i) < n and idp.at(i) == idp[i]);
          s += 'a' + idp.at(i);
        }
        assert(ss.find(s) == ss.end());
        ss.insert(s);
      }
      ll num = 1;
      for (ll i = 0; i < r; i++) num = num * n;
      assert((ll)ss.size() == num);
    };
    chkDupPerm(6, 3);
    chkDupPerm(8, 5);
    chkDupPerm(3, 8);
  }
  {
    auto chkDupComb = [&](ll n, ll r) -> void {
      IntDupComb idc(n, r);
      set<string> ss;
      while (idc.get()) {
        string s;
        ll last = -1;
        for (ll i = 0; i < r; i++) {
          assert(0 <= idc.at(i) and idc.at(i) < n and idc.at(i) == idc[i]);
          assert(last <= idc.at(i));
          last = idc.at(i);
          s += 'a' + idc.at(i);
        }
        assert(ss.find(s) == ss.end());
        ss.insert(s);
      }
      ll num = 1;
      for (ll i = 0; i < r; i++) num = num * (n + r - 1 - i);
      for (ll i = 0; i < r; i++) num = num / (r - i);
      assert((ll)ss.size() == num);
    };
    chkDupComb(6, 3);
    chkDupComb(8, 5);
    chkDupComb(3, 8);
  }
  {
    auto chkDirProd = [&](const auto& vec) -> void {
      IntDirProd idp(vec);
      set<string> ss;
      while (idp.get()) {
        string s;
        for (ll i = 0; i < ssize(vec); i++) {
          assert(0 <= idp[i] and idp[i] < vec[i] and idp[i] == idp.at(i));
          s += 'a' + idp.at(i);
        }
        assert(ss.find(s) == ss.end());
        ss.insert(s);
      }
      ll num = 1;
      for (ll x : vec) num *= x;
      assert((ll)ss.size() == num);
    };
    chkDirProd(vector{4, 1, 2});
    chkDirProd(vector{2, 3, 5});
  }
  {
    auto count_partition = [&](ll n) {
      IntPartition p(n);
      ll cnt = 0;
      while (p.get()) { cnt++; }
      return cnt;
    };
    assert(count_partition(10) == 42);
    assert(count_partition(20) == 627);
    assert(count_partition(30) == 5604);

    using u64 = unsigned long long;
    IntPartition part_u64((u64)10);
    part_u64.get();
    vector<u64> tmp1 = part_u64.vec_view();  // just to test if compilation is ok
    IntPartition part_ll((long long)10);
    part_ll.get();
    vector<ll> tmp2 = part_ll.vec_view();
    IntPartition part_unsigned((unsigned)10);
    part_unsigned.get();
    vector<unsigned> tmp3 = part_unsigned.vec_view();
  }


  auto test_is_empty = [&](auto p) -> void { assert(my_compare(p, vvi{})); };
  auto test_is_single = [&](auto p) -> void { assert(my_compare(p, vvi{{}})); };
  {
    test_is_empty(IntPerm(-3, 2));
    test_is_empty(IntComb(-3, 2));
    test_is_empty(IntDupPerm(-3, 2));
    test_is_empty(IntDupComb(-3, 2));
    test_is_empty(IntPerm(3, -2));
    test_is_empty(IntComb(3, -2));
    test_is_empty(IntDupPerm(3, -2));
    test_is_empty(IntDupComb(3, -2));
    test_is_empty(IntPerm(3, 5));
    test_is_empty(IntComb(3, 5));
    test_is_single(IntPerm(0, 0));
    test_is_single(IntComb(0, 0));
    test_is_single(IntDupPerm(0, 0));
    test_is_single(IntDupComb(0, 0));
    test_is_single(IntPerm(5, 0));
    test_is_single(IntComb(5, 0));
    test_is_single(IntDupPerm(5, 0));
    test_is_single(IntDupComb(5, 0));
  }

  auto test_run_twice = [&](auto p) -> void {
    vector<vector<int>> res1, res2;
    while (p.get()) {
      res1.push_back(p.vec_view());
    }
    while (p.get()) {
      res2.push_back(p.vec_view());
    }
    assert(res1 == res2);
  };
  {
    test_run_twice(IntPerm(3, 2));
    test_run_twice(IntComb(7, 3));
    test_run_twice(IntDupPerm(7, 3));
    test_run_twice(IntDupComb(7, 3));
    test_run_twice(IntPartition(10));
  }

  {
    vector<ll> vec1{10, 40, 50};
    IntPerm ip(3, 2, vec1);
    assert(my_compare(ip, vvi{{10,40},{10,50},{40,10},{40,50},{50,10},{50,40}}));
    vector<char> vec2{'a', 'b', 'c'};
    IntComb ic(3, 2, vec2);
    assert(my_compare(ic, vector<vector<char>>{{'a', 'b'}, {'a', 'c'}, {'b', 'c'}}));
    using pll = pair<ll, ll>;
    IntDupPerm dip(2, 2, vector<pll>{{1,3}, {2,4}});
    assert(my_compare(dip, vector<vector<pll>>{{{1,3}, {1,3}}, {{1,3}, {2,4}}, {{2,4}, {1,3}}, {{2,4}, {2,4}}}));
    IntDupComb dic(2, 2, vector<int>{-5, 5});
    assert(my_compare(dic, vvi{{-5, -5}, {-5, 5}, {5, 5}}));
    IntComb<pll> ic2(3, 2);
    ic2.set_mapping([](int i) { return pll(-(i + 1), i + 1); });
    assert(my_compare(ic2, vector<vector<pll>>{{{-1,1}, {-2,2}}, {{-1,1}, {-3,3}}, {{-2,2}, {-3,3}}}));
  }

  cerr << "ok\n";

}
