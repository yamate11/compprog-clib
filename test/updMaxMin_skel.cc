#include <bits/stdc++.h>
#include <cassert>
using namespace std;
using ll = long long int;
using u64 = unsigned long long;
using pll = pair<ll, ll>;
#define REP(i, a, b) for (ll i = (a); i < (b); i++)
#define REPrev(i, a, b) for (ll i = (a); i >= (b); i--)
#define ALL(coll) (coll).begin(), (coll).end()

// @@ !! LIM(updMaxMin)

int main() {

  {
    ll x1 = 5, y1 = 10;
    bool b1 = updMax(x1, y1);
    assert(b1 and x1 == 10);
    ll x2 = 5, y2 = 0;
    bool b2 = updMax(x2, y2);
    assert(not b2 and x2 == 5);
  }
  {
    ll x1 = 5, y1 = 10;
    bool b1 = updMin(x1, y1);
    assert(not b1 and x1 == 5);
    ll x2 = 5, y2 = 0;
    bool b2 = updMin(x2, y2);
    assert(b2 and x2 == 0);
  }

  cerr << "ok\n";
  return 0;
}
