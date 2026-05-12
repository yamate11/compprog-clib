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

// @@ !! LIM(debug doubling random mod)

int main(/* int argc, char *argv[] */) {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << setprecision(20);
  
  Random rand;


  { // interfaces
    auto dobj = make_doubling(100, vector<int>{1, 2, 3, 0});
    assert(dobj.f_val(13, 0) == 1);

    auto dobj2 = make_doubling(100, vector<ll>{1, 2, 3, 0}, vector<ll>{0, 0, 0, 1}, 0LL, plus<ll>());
    auto [i2, x2] = dobj2.val_pair(13, 0);
    assert(i2 == 1 and x2 == 3);

    ll big = 1LL << 60;
    auto dobj3 = make_doubling(big, vector<int>{1, 2, 3, 0}, vector<ll>{0, 0, 0, 1}, 0LL, plus<ll>());
    auto [i3, x3] = dobj3.val_pair(big, 0);
    assert(i3 == 0 and x3 == big / 4);
    auto [i33, x33] = dobj3.val_pair(big - 1, 0);
    assert(i33 == 3 and x33 == x3 - 1);
    auto [i4, x4] = dobj3.val_pair(big - 1, 2);
    assert(i4 == 1 and x4 == big / 4);
    assert(dobj3.f_val(big - 1, 2) == 1 and dobj3.t_val(big - 1, 2) == big / 4);
  }

  { // ABC179-E  
    using vll = vector<ll>;
    auto func = [&](ll N, ll K, const vll& A) -> ll {
      vector<int> vf(N);
      REP(i, 0, N) vf[i] = (i + A[i]) % N;
      auto dobj = make_doubling(K, vf, A, 0LL, plus<ll>());
      return dobj.t_val(K, 0);
    };
    assert(func(5, 3, vll({2, 1, 6, 3, 1})) == 11);
    assert(func(10, 1000000000000, vll({260522, 914575, 436426, 979445, 648772, 690081, 933447, 190629, 703497, 47202})));
  }

  { // Non-commutative product.  AWC-0060E 
    
    using Fp = FpG<0>;
    auto solve = [&](ll M, const auto& D, const auto& P, const auto& S, const auto& K) {
      Fp::setMod(M);
      ll lim = 1LL << 60;
      using sta = pair<Fp, Fp>;
      vector<sta> DD(ssize(D));
      REP(i, 0, ssize(D)) DD[i] = sta(Fp(D[i]), Fp(10));
      auto prod = [&](sta x, sta y) -> sta {
        auto [tx, lx] = x;
        auto [ty, ly] = y;
        return sta(tx * ly + ty, lx * ly);
      };
      auto PP = P;
      for (int& p : PP) p--;
      auto dobj = make_doubling(lim, PP, DD, sta(Fp(0), Fp(1)), prod);
      vector<Fp> ret(ssize(S));
      REP(i, 0, ssize(S)) ret[i] = dobj.t_val(K[i], S[i] - 1).first;
      return ret;
    };
    auto a1 = solve(1000, vector{1, 2, 3, 4}, vector{2, 3, 4, 2}, vector{1, 2, 4, 3}, vector{3, 5, 4, 0});
    auto e1 = vector<Fp>{123, 423, 234, 0};
    auto a2 = solve(13, vector{0, 5, 9}, vector{2, 2, 1}, vector{1, 1, 3, 2, 3}, vector{1, 3, 2, 4, 0});
    auto e2 = vector<Fp>{0, 3, 12, 4, 0};
    auto a3 = solve(257, vector{6,2,0,7,5,1,9,3,4}, vector{2,3,4,2,3,5,8,9,7},
                    vector{1,1,6,5,7,8,9,3}, vector{4,10,6,0,5,12,1,100});
    auto e3 = vector<Fp>{39,58,118,0,202,176,4,110};
    assert(a1 == e1);
    assert(a2 == e2);
    assert(a3 == e3);
  }

  {
    ll n0 = 10;
    ll K0 = 20;
    ll rep = 10000;

    { // function only
      REP(_r, 0, rep) {
        ll n = rand.range(1, n0);
        ll K = rand.range(1, K0);
        vector<ll> A(n);
        REP(i, 0, n) A[i] = rand.range(0, n);
        auto d = make_doubling(K, A);
        auto naive_f = [&](ll r, ll x) -> ll {
          REP(i, 0, r) x = A[x];
          return x;
        };
        REP(i, 0, n) {
          ll r = rand.range(0, K + 1);
          assert(d.f_val(r, i) == naive_f(r, i));
        }
      }
    }

    { // monoid: Fp
      using Fp = FpB;
      REP(_r, 0, rep) {
        ll n = rand.range(1, n0);
        ll K = rand.range(1, K0);
        vector<ll> A(n);
        REP(i, 0, n) A[i] = rand.range(0, n);
        vector<Fp> B(n);
        REP(i, 0, n) B[i] = Fp(rand.range(0, (ll)1e9));
        auto naive_m = [&](ll r, ll x) -> pair<ll, Fp> {
          Fp v = 0;
          REP(i, 0, r) {
            v += B[x];
            x = A[x];
          }
          return {x, v};
        };
        auto d = make_doubling(K, A, B, Fp(0), plus<Fp>());
        REP(i, 0, n) {
          ll r = rand.range(0, K + 1);
          assert(d.val_pair(r, i) == naive_m(r, i));
        }
      }
    }

    { // monoid: min
      REP(_r, 0, rep) {
        ll n = rand.range(1, n0);
        ll K = rand.range(1, K0);
        vector<int> A(n);
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
        auto d = make_doubling(K, A, B, (ll)1e9, mymin);
        REP(i, 0, n) {
          ll r = rand.range(0, K + 1);
          assert(d.val_pair(r, i) == naive_m(r, i));
        }
      }
    }

    { // binsearch

      struct S {
        ll left;
        ll right;
        ll numinc;
        ostream& operator<<(ostream& ostr) const {
          return ostr << "(" << this->left << ", " << this->right << ", " << this->numinc << ")";
        }
        auto operator<=>(const S&) const = default;
      };

      REP(_r, 0, rep) {
        ll n = rand.range(1, 10);
        ll R = rand.range(1, 20);
        ll K = rand.range(1, 10);
        vector<int> A(n);
        REP(i, 0, n) A[i] = rand.range(0, n);
        S unit_S{-1, -1, 0};
        vector<S> M(n);
        REP(i, 0, n) M[i] = S{i, i, 0};
        auto prod = [&](const S& a, const S&b) -> S {
          if (a == unit_S) return b;
          if (b == unit_S) return a;
          return S{a.left, b.right, a.numinc + b.numinc + (a.right < b.left ? 1 : 0)};
        };
        auto naive_1 = [&](ll x, ll lo, ll hi) -> ll {
          auto op = [&](int i, const S& a) -> pair<int, S> {
            return make_pair(A[i], prod(a, S{i, i, 0}));
          };
          int idx = x;
          S acc = unit_S;
          REP(i, 0, lo) {
            tie(idx, acc) = op(idx, acc);
            DLOGKL("  ", i, idx, acc.numinc);
          }
          if (acc.numinc >= K) return lo;
          REP(i, lo, hi) {
            tie(idx, acc) = op(idx, acc);
            DLOGKL("  ", i, idx, acc.numinc);
            if (acc.numinc >= K) return i + 1;
          }
          return hi + 1;
        };
        auto naive_2 = [&](ll x, ll lo, ll hi) -> ll { return naive_1(x, lo, hi) - 1; };
        auto d = make_doubling(R, A, M, unit_S, prod);
        DLOGK(A, R, K);
        REP(_rr, 0, 5) {
          auto [lo1, hi1] = rand.select2(0, R + 2); hi1--;
          ll x1 = rand.range(0, n);
          ll a1 = naive_1(x1, lo1, hi1);
          ll b1 = d.binsearch_hi([&](const S& s) { return s.numinc >= K; }, x1, lo1, hi1);
          DLOGK(a1, b1, x1, lo1, hi1);
          assert(a1 == b1);
          auto [lo2, hi2] = rand.select2(0, R + 2); hi2--;
          ll x2 = rand.range(0, n);
          ll a2 = naive_2(x2, lo2, hi2);
          ll b2 = d.binsearch_lo([&](const S& s) { return s.numinc < K; }, x2, lo2, hi2);
          DLOGK(a2, b2, x2, lo2, hi2);
          assert(a2 == b2);
          ll x3 = rand.range(0, n);
          ll a3 = naive_1(x3, 0, R);
          ll b3 = d.binsearch_hi([&](const S& s) { return s.numinc >= K; }, x3);
          DLOGK(a3, b3, x3);
          assert(a3 == b3);
        }
      }
    }

  }

  
  cout << "ok" << endl;
  return 0;
}

