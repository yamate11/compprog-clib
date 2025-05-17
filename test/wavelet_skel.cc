#include <bits/stdc++.h>
#include <cassert>
using namespace std;
using ll = long long int;
using u64 = unsigned long long;
using pll = pair<ll, ll>;
#define REP(i, a, b) for (ll i = (a); i < (b); i++)
#define REPrev(i, a, b) for (ll i = (a); i >= (b); i--)
#define ALL(coll) (coll).begin(), (coll).end()
#define SIZE(v) ((ll)((v).size()))
#define REPOUT(i, a, b, exp, sep) REP(i, (a), (b)) cout << (exp) << (i + 1 == (b) ? "" : (sep)); cout << "\n"

// @@ !! LIM(debug wavelet random)

Random myrand;

void test_bv1() {
  BitVector bv(8);
  bv.set(1, 3);
  bv.set(1, 5);
  bv.set(1, 6);
  assert(bv.at(0) == 0);
  assert(bv.at(1) == 0);
  assert(bv.at(6) == 1);
  assert(bv.rank(0, 0) == 0);
  assert(bv.rank(0, 4) == 3);
  assert(bv.rank(0, 5) == 4);
  assert(bv.rank(0, 6) == 4);
  assert(bv.rank(0, 7) == 4);
  assert(bv.rank(0, 8) == 5);
  assert(bv.rank(1, 0) == 0);
  assert(bv.rank(1, 4) == 1);
  assert(bv.rank(1, 6) == 2);
  assert(bv.rank(1, 8) == 3);
  assert(bv.at(2) == 0);
  assert(bv.at(3) == 1);
  assert(bv.at(4) == 0);
}

void test_bv2() {
  BitVector bv(1000);
  bv.set(1, 256);
  bv.set(1, 511);
  bv.set(1, 513);
  assert(bv.rank(1, 256) == 0);
  assert(bv.rank(1, 257) == 1);
  assert(bv.rank(1, 511) == 1);
  assert(bv.rank(1, 512) == 2);
  assert(bv.rank(1, 513) == 2);
  assert(bv.rank(1, 514) == 3);
  assert(bv.rank(0, 514) == 511);
}

void test_wm1() {
  vector<ll> vec{6, 2, 6, 4, 1, 5, 0, 3};
  WaveletMatrix wm(vec, 6);
  
  assert(wm.N == 8);
  assert(wm.ht == 3);
  vector<vector<bool>> exp{{1, 0, 1, 1, 0, 1, 0, 0}, {1, 0, 0, 1, 1, 1, 0, 0}, {1, 0, 0, 1, 0, 1, 0, 0}};
  vector<ll> exp1{4, 4, 5};
  for (ll h = 2; h >= 0; h--) {
    assert(wm.vbv[h].vec_view() == exp[2 - h]);
    assert(wm.mid[h] == exp1[2 - h]);
  }

  for (ll i = 0; i < ssize(vec); i++) {
    assert(wm.access(i) == vec[i]);
  }
  assert(wm.rank(6, 0) == 0);
  assert(wm.rank(6, 1) == 1);
  assert(wm.rank(6, 2) == 1);
  assert(wm.rank(6, 3) == 2);
  assert(wm.rank(6, 8) == 2);
  assert(wm.rank(64+6, 8) == 0);

  assert(wm.kth_smallest(0, 0, 8) == 0);
  assert(wm.kth_smallest(1, 1, 5) == 2);
  assert(wm.kth_smallest(2, 1, 5) == 4);
  assert(wm.kth_smallest(4, 0, 5) == 6);

  try {
    wm.kth_smallest(5, 0, 5);
    assert(false);
  }catch(runtime_error& e) {
  }

  assert(wm.kth_largest(3, 0, 8) == 4);
  assert(wm.kth_largest(1, 0, 3) == 6);

  assert(wm.range_freq(4, 0, 8) == 4);
  assert(wm.range_freq(1, 2, 6) == 0);
  assert(wm.range_freq(6, 2, 6) == 3);

  assert(wm.range_freq(1, 5, 3, 7) == 2);
  assert(wm.range_freq(2, 4, 3, 7) == 0);
}

void test_wm2() {
  ll rep = 10000;
  for (ll _r = 0; _r < rep; _r++) {
    ll N = myrand.range(1, 20);
    ll amax = myrand.range(1, 20);
    vector<ll> vec(N);
    REP(i, 0, N) vec.push_back(myrand.range(0, amax + 1));
    WaveletMatrix wm(vec, (myrand.range(0, 100) < 50 ? amax : -1));
    ll i = myrand.range(0, N);
    assert(wm.access(i) == vec[i]);
    ll x = myrand.range(0, amax + 2);  // x == amax+1 is tested
    ll cnt1 = 0;
    REP(j, 0, N) {
      if (vec[j] == x) cnt1++;
      assert(wm.rank(x, j + 1) == cnt1);
    }
    ll l1 = myrand.range(0, N);
    ll r1 = myrand.range(l1 + 1, N + 1);
    ll k = myrand.range(0, r1 - l1);
    ll k2 = myrand.range(0, r1 - l1);
    vector<ll> vv;
    REP(j, l1, r1) vv.push_back(vec[j]);
    sort(ALL(vv));
    assert(wm.kth_smallest(k, l1, r1) == vv[k]);
    sort(ALL(vv), greater<ll>());
    assert(wm.kth_largest(k2, l1, r1) == vv[k2]);
    ll t1 = myrand.range(0, amax + 2); // amax+1 is tested
    ll t2 = myrand.range(0, amax + 2);
    if (t1 > t2) swap(t1, t2);
    ll cnt2 = 0;
    REP(r, l1, N + 1) {
      assert(wm.range_freq(t1, t2, l1, r) == cnt2);
      if (r == N) break;
      if (t1 <= vec[r] and vec[r] < t2) cnt2++;
    }
  }
}

void test_wm3() { // vec is empty
  vector<ll> vec;
  WaveletMatrix wm(vec, -1);
  assert(wm.rank(1, 0) == 0);
  assert(wm.range_freq(0, 10, 0, 0) == 0);
}


int main() {

  test_bv1();
  test_bv2();

  test_wm1();
  test_wm2();
  test_wm3();

  cerr << "ok" << endl;
  return 0;
}


