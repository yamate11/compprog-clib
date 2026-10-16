#include <bits/stdc++.h>
#include <cassert>
using namespace std;
using ll = long long int;
using u64 = unsigned long long;
using pll = pair<ll, ll>;
#define REP(i, a, b) for (ll i = (a); i < (b); i++)
#define REPrev(i, a, b) for (ll i = (a); i >= (b); i--)
#define ALL(coll) (coll).begin(), (coll).end()

// @@ !! LIM(random intDiv floorSum)

int main() {

  {
    Random myrand;
    ll _rep = 10000;
    REP(_r, 0, _rep) {
      ll n = myrand.range(0, 10);
      ll m = myrand.range(1, 10);
      ll a = myrand.range(-9, 10);
      ll b = myrand.range(-9, 10);
      ll sum = 0;
      REP(i, 0, n) sum += divFloor(a * i + b, m);
      assert(sum == floor_sum(n, m, a, b));
    }
  }

  cerr << "ok\n";
  return 0;
}
