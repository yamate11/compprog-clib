#include <bits/stdc++.h>
#include <cassert>
using namespace std;
using ll = long long int;
using pll = pair<ll, ll>;
// #include <atcoder/all>
// using namespace atcoder;
#define REP(i, a, b) for (ll i = (a); i < (b); i++)
#define REPrev(i, a, b) for (ll i = (a); i >= (b); i--)
#define ALL(coll) (coll).begin(), (coll).end()
#define SIZE(v) ((ll)((v).size()))
#define REPOUT(i, a, b, exp, sep) REP(i, (a), (b)) { if (i != (a)) cout << (sep); cout << (exp); } cout << "\n"
using u64 = unsigned long long;

// @@ !! LIM(doubling random mod debug)

int main(/* int argc, char *argv[] */) {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << setprecision(20);
  
  Random rand;

  { // interfaces
    Doubling dobj(100, 4, vector<int>{1, 2, 3, 0});
    assert(dobj.val(13, 0) == 1);
    Doubling<int, plus<int>> dobj2(100, 4, vector<int>{1, 2, 3, 0}, vector<int>{0, 0, 0, 1}, 0, plus<int>());
    auto [i2, x2] = dobj2.val(13, 0);
    assert(i2 == 1 and x2 == 3);
    ll big = 1e18;
    auto dobj3 = make_doubling_with_monoid_unit_add(big, 4, vector<int>{1, 2, 3, 0}, vector<ll>{0, 0, 0, 1}, 0LL, plus<ll>());
    auto [i3, x3] = dobj3.val(big, 0);
    assert(i3 == 0 and x3 == big / 4);
    auto dobj4 = make_doubling_with_monoid<ll>(big, 4, vector<int>{1, 2, 3, 0}, vector<ll>{0, 0, 0, 1});
    auto [i4, x4] = dobj4.val(big - 1, 2);
    assert(i4 == 1 and x4 == big / 4);
  }

  { // ABC179-E  
    using vll = vector<ll>;
    auto func = [&](ll N, ll K, const vll& A) -> ll {
      vector<int> vf(N);
      REP(i, 0, N) vf[i] = (i + A[i]) % N;
      auto dobj = make_doubling_with_monoid<ll>(K, N, vf, A);
      return dobj.val(K, 0).second;
    };
    assert(func(5, 3, vll({2, 1, 6, 3, 1})) == 11);
    assert(func(10, 1000000000000, vll({260522, 914575, 436426, 979445, 648772, 690081, 933447, 190629, 703497, 47202})));
  }

  {
    ll n = 10;
    ll K = 100;
    ll rep = 1000;

    { // function only
      REP(_r, 0, rep) {
        vector<ll> A(n);
        REP(i, 0, n) A[i] = rand.range(0, n);
        Doubling d(K, n, A);
        auto naive_f = [&](ll r, ll x) -> ll {
          REP(i, 0, r) x = A[x];
          return x;
        };
        REP(i, 0, n) {
          ll r = rand.range(0, K + 1);
          assert(d.val(r, i) == naive_f(r, i));
        }
      }
    }
    { // monoid: Fp
      using Fp = FpB;
      REP(_r, 0, rep) {
        vector<ll> A(n);
        REP(i, 0, n) A[i] = rand.range(0, n);
        vector<Fp> B(n);
        REP(i, 0, n) B[i] = Fp(rand.range(0, (ll)1e9));
        auto naive_m = [&](ll r, ll x) -> pair<int, Fp> {
          Fp v = 0;
          REP(i, 0, r) {
            v += B[x];
            x = A[x];
          }
          return {x, v};
        };
        auto d = make_doubling_with_monoid<Fp>(K, n, A, B);
        REP(i, 0, n) {
          ll r = rand.range(0, K + 1);
          assert(d.val(r, i) == naive_m(r, i));
        }
      }
    }
    { // monoid: min
      REP(_r, 0, rep) {
        vector<ll> A(n);
        REP(i, 0, n) A[i] = rand.range(0, n);
        vector<ll> B(n);
        REP(i, 0, n) B[i] = rand.range(0, (ll)1e9);
        auto naive_m = [&](ll r, ll x) -> pair<int, ll> {
          ll v = 1e9;
          REP(i, 0, r) {
            v = min(v, B[x]);
            x = A[x];
          }
          return {x, v};
        };
        auto mymin = [](ll a, ll b) -> ll { return min(a, b); };
        auto d = make_doubling_with_monoid_unit_add(K, n, A, B, (ll)1e9, mymin);
        REP(i, 0, n) {
          ll r = rand.range(0, K + 1);
          assert(d.val(r, i) == naive_m(r, i));
        }
      }
    }
  }
  
  cout << "ok" << endl;
  return 0;
}

