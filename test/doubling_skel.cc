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

// @@ !! LIM(doubling)

int main(/* int argc, char *argv[] */) {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << setprecision(20);
  
  random_device rd;
  mt19937 rng(rd());
  auto randrange = [&rng](ll i, ll j) -> ll {
    uniform_int_distribution<ll> dist(i, j - 1);
    return dist(rng);
  };

  { // ABC179-E  
    using vll = vector<ll>;
    auto func = [&](ll N, ll K, const vll& A) -> ll {
      auto d1 = doubling_from_func(K, N, [&](int i) -> int { return (i + A[i]) % N; });
      auto d2 = doubling_cum_from_container<ll>(d1, A);
      return d2.val(K, 0);
    };
    assert(func(5, 3, vll({2, 1, 6, 3, 1})) == 11);
    assert(func(10, 1000000000000, vll({260522, 914575, 436426, 979445, 648772, 690081, 933447, 190629, 703497, 47202})));
  }

  { // interfaces
    ll r = 20;
    ll n = 5;

    auto f1 = [&](ll i) -> ll { return (i + 2) % n; };
    vector<ll> v1{2, 3, 4, 0, 1};
    deque<ll> v2{2, 3, 4, 0, 1};
    auto m1 = [&](ll i) -> ll { return i * 10; };
    vector<ll> mv1{0, 10, 20, 30, 40};
    auto mymax = [](ll a, ll b) -> ll { return max(a, b); };
    auto d1 = doubling_from_func(r, n, f1);
    auto d2 = doubling_from_container(r, n, v1);
    auto d3 = doubling_from_container(r, n, v2);
    auto d4 = doubling_cum_from_func<ll>(d1, m1);
    auto d5 = doubling_cum_from_container<ll>(d1, mv1);
    auto d6 = doubling_cum_from_func<ll>(d1, m1, LLONG_MIN, mymax);
    auto d7 = doubling_cum_from_container<ll>(d1, mv1, LLONG_MIN, mymax);
    assert(d1.val(4, 0) == 3);
    assert(d2.val(3, 1) == 2);
    assert(d3.val(10, 0) == 0);
    assert(d4.val(3, 3) == 50);
    assert(d5.val(2, 1) == 40);
    assert(d6.val(2, 1) == 30);
    assert(d7.val(3, 0) == 40);
  }


  {
    ll n = 10;
    ll K = 100;
    ll rep = 1000;
    vector<ll> A(n), B(n);
    REP(i, 0, n) {
      A[i] = randrange(0, n);
      B[i] = randrange(0, (ll)1e9);
    }
    auto naive = [&](ll r, ll x) -> ll {
      REP(i, 0, r) x = A[x];
      return x;
    };
    auto naive2 = [&](ll r, ll x) -> ll {
      ll s = 0;
      REP(i, 0, r) {
        s += B[x];
        x = A[x];
      }
      return s;
    };
    auto d = doubling_from_container(K, n, A);
    auto d2 =doubling_cum_from_container<ll>(d, B);
    REP(i, 0, rep) {
      ll r = randrange(0, K + 1);
      ll x = randrange(0, n);
      assert(naive(r, x) == d.val(r, x));
    }
    REP(i, 0, rep) {
      ll r = randrange(0, K + 1);
      ll x = randrange(0, n);
      assert(naive2(r, x) == d2.val(r, x));
    }
    REP(i, 0, rep) {
      ll k = randrange(1, K + 1);
      ll x = randrange(0, n);
      auto d3 = doubling_from_container(k, n, A);
      assert(naive(k, x) == d3.val(k, x));
    }
    REP(i, 0, rep) {
      ll k = randrange(1, K + 1);
      ll x = randrange(0, n);
      auto d4 = doubling_from_container(k, n, A);
      auto d5 = doubling_cum_from_container<ll>(d4, B);
      assert(naive2(k, x) == d5.val(k, x));
    }
  }

  cout << "ok" << endl;
  return 0;
}

