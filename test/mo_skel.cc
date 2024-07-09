#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;
#define REP(i, a, b) for (ll i = (a); i < (b); i++)
#define REPrev(i, a, b) for (ll i = (a); i >= (b); i--)
#define ALL(coll) (coll).begin(), (coll).end()
#define SIZE(v) ((ll)((v).size()))
#define REPOUT(i, a, b, exp, sep) REP(i, (a), (b)) cout << (exp) << (i + 1 == (b) ? "" : (sep)); cout << "\n"
using pll = pair<ll, ll>;

// @@ !! LIM(mo debug)

random_device rd;
mt19937 rng(rd());
ll randrange(ll i, ll j) {
  uniform_int_distribution<ll> dist(i, j - 1);
  return dist(rng);
}

// For confirmation testing:
// - ABC 242G Range Pairing Query
  
void fromABC242G() {

  ll T = 100;
  REP(_t, 0, T) {
    ll N = randrange(30, 50);
    vector<ll> A(N);
    ll nc = randrange(1, N + 1);
    REP(j, 0, N) A[j] = randrange(0, nc);
    ll Q = randrange(30, 50);
    vector<pll> qs(Q);
    REP(j, 0, Q) {
      ll a = randrange(0, N);
      ll b = randrange(a + 1, N + 1);
      qs[j] = pll(a, b);
    }

    vector<ll> cnt(N);
    ll cans = 0;
    vector<ll> ans(Q);

    auto f_in  = [&](ll i) -> void { if (++cnt[A[i]] % 2 == 0) cans++; };
    auto f_out = [&](ll i) -> void { if (cnt[A[i]]-- % 2 == 0) cans--; };
    auto calc = [&](ll i) -> void { ans[i] = cans; };
    Mo mo(N);
    REP(i, 0, Q) mo.add_query(qs[i].first, qs[i].second);
    mo.run(f_in, f_out, calc);

    // naive
    REP(i, 0, Q) {
      auto [a, b] = qs[i];
      vector<ll> ncnt(N);
      REP(j, a, b) ncnt[A[j]]++;
      ll nans = 0;
      REP(k, 0, nc) nans += ncnt[k] / 2;
      assert(nans == ans[i]);
    }
  }
}

void from_past_202212_N() {   // PAST 2022.12 N
  auto sq = [&](ll x) -> ll { return x * x; };

  auto sub = [&]() -> void {
    ll N = randrange(10, 20);
    vector<ll> A(N);
    REP(i, 0, N) A[i] = randrange(1, 20);
    ll Q = randrange(10, 20);
    vector<ll> L(Q), R(Q);
    REP(_q, 0, Q) {
      ll x = randrange(1, N + 1);
      ll y = randrange(1, N + 1);
      if (x > y) swap(x, y);
      L[_q] = x - 1;
      R[_q] = y;
    }

    Mo mo(N);
    multiset<ll> B;
    ll cur = 0;
    vector<ll> ans(Q);

    auto modif = [&](auto it_hi, ll x, ll sign) -> void {
      if (not B.empty()) {
        if (it_hi == B.begin()) cur += sign * sq(*it_hi - x);
        else {
          auto it_lo = prev(it_hi);
          if (it_hi == B.end()) cur += sign * sq(x - *it_lo);
          else                  cur += sign * (sq(x - *it_lo) + sq(*it_hi - x) - sq(*it_hi - *it_lo));
        }
      }
    };
    REP(i, 0, Q) mo.add_query(L[i], R[i]);
    auto in_func = [&](ll i) -> void {
      modif(B.lower_bound(A[i]), A[i], +1);
      B.insert(A[i]);
    };
    auto out_func = [&](ll i) -> void {
      auto it_hi = B.erase(B.find(A[i]));
      modif(it_hi, A[i], -1);
    };
    auto calc = [&](ll q) -> void { ans[q] = cur; };
    mo.run(in_func, out_func, calc);

    // naive
    REP(_q, 0, Q) {
      vector<ll> C;
      REP(i, L[_q], R[_q]) C.push_back(A[i]);
      sort(ALL(C));
      ll val = 0;
      REP(i, 0, ssize(C) - 1) val += (C[i + 1] - C[i]) * (C[i + 1] - C[i]);
      assert(val == ans[_q]);
    }
  };
  REP(rep, 0, 1000) sub();
}


int main(int argc, char* argv[]) {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << setprecision(20);

  fromABC242G();
  from_past_202212_N();

  cerr << "ok\n";
}
