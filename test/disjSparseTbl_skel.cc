#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;
using pll = pair<ll, ll>;
#define REP(i, a, b) for (ll i = (a); i < (b); i++)
#define REPrev(i, a, b) for (ll i = (a); i >= (b); i--)
#define ALL(coll) (coll).begin(), (coll).end()
#define SIZE(v) ((ll)((v).size()))
#define REPOUT(i, a, b, exp, sep) REP(i, (a), (b)) cout << (exp) << (i + 1 == (b) ? "" : (sep)); cout << "\n"

// @@ !! LIM(debug random disjSparseTbl)

ll xorll(ll a, ll b) { return a ^ b; };

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << setprecision(20);

  Random myrand;

  {
    vector<ll> A{5, 3, -7, -3, 0, 4};
    DisjSparseTbl dst(0LL, plus<ll>(), A);
    assert(dst.query(0, 6) == 2);
    assert(dst.query(2, 4) == -10);
    assert(dst.query(1, 3) == -4);
    assert(dst.query(3, 5) == -3);
    assert(dst.query(1, 1) == 0);
    assert(dst.query(1, 2) == 3);
  }
  { // non-commutative operation
    vector<string> A{"abc", "d", "efg", "", "hij", "kl", "mno"};
    DisjSparseTbl dst(string(), plus<string>(), A);
    assert(dst.query(0, 7) == "abcdefghijklmno");
    assert(dst.query(2, 4) == "efg");
    assert(dst.query(5, 7) == "klmno");
    assert(dst.query(3, 5) == "hij");
  }
  {
    // size == 0
    vector<ll> A{};
    DisjSparseTbl dst(0LL, plus<ll>(), A);
    assert(dst.query(0, 0) == 0);
  }
  {
    // size == 1
    vector<ll> A{8};
    DisjSparseTbl dst(0LL, plus<ll>(), A);
    assert(dst.query(0, 0) == 0);
    assert(dst.query(0, 1) == 8);
    assert(dst.query(1, 1) == 0);
  }
  {
    // operation defined by a lambda
    vector<int> A{5, 7, 2};
    int big = 1000;
    auto mymin = [&](int a, int b) -> int { return min(a, b); };
    DisjSparseTbl dst(big, mymin, A);
    assert(dst.query(0, 3) == 2);
    assert(dst.query(0, 2) == 5);
    assert(dst.query(1, 1) == big);
  }
  {
    // operation defined by function pointer
    vector<ll> A{0, 1, 2, 3, 4, 5, 6, 7};
    DisjSparseTbl dst(0LL, xorll, A);
    assert(dst.query(0, 8) == 0);
    assert(dst.query(2, 4) == 1);
    assert(dst.query(4, 7) == 7);
  }
  {
    // random test
    ll rep1 = 300;
    ll rep2 = 300;
    REP(_r, 0, rep1) {
      ll sz = myrand.range(1, 17);
      vector<ll> A(sz);
      REP(i, 0, sz) A[i] = myrand.range(0, 10);
      vector<ll> SA(sz + 1);
      REP(i, 0, sz) SA[i + 1] = SA[i] + A[i];
      DisjSparseTbl dst(0LL, plus<ll>(), A);
      REP(_r2, 0, rep2) {
        auto [a, b] = myrand.select2(0, sz + 2);
        b--;
        assert(dst.query(a, b) == SA[b] - SA[a]);
      }
    }
  }

  cerr << "ok" << endl;
  return 0;
}


