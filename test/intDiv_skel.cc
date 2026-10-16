#include <bits/stdc++.h>
#include <cassert>
using namespace std;
using ll = long long int;
using u64 = unsigned long long;
using pll = pair<ll, ll>;
#define REP(i, a, b) for (ll i = (a); i < (b); i++)
#define REPrev(i, a, b) for (ll i = (a); i >= (b); i--)
#define ALL(coll) (coll).begin(), (coll).end()

// @@ !! LIM(intDiv)

int main() {

  assert(divFloor( 18,  3) ==  6);
  assert(divCeil(  18,  3) ==  6);
  assert(divFloor(-18,  3) == -6);
  assert(divCeil( -18,  3) == -6);
  assert(divFloor( 18, -3) == -6);
  assert(divCeil(  18, -3) == -6);
  assert(divFloor(-18, -3) ==  6);
  assert(divCeil( -18, -3) ==  6);

  assert(divFloor( 19,  3) ==  6);
  assert(divCeil(  19,  3) ==  7);
  assert(divFloor(-19,  3) == -7);
  assert(divCeil( -19,  3) == -6);
  assert(divFloor( 19, -3) == -7);
  assert(divCeil(  19, -3) == -6);
  assert(divFloor(-19, -3) ==  6);
  assert(divCeil( -19, -3) ==  7);

  assert(divFloor( 20,  3) ==  6);
  assert(divCeil(  20,  3) ==  7);
  assert(divFloor(-20,  3) == -7);
  assert(divCeil( -20,  3) == -6);
  assert(divFloor( 20, -3) == -7);
  assert(divCeil(  20, -3) == -6);
  assert(divFloor(-20, -3) ==  6);
  assert(divCeil( -20, -3) ==  7);

  {
    assert(imod( 3,  7) ==  3);
    assert(imod(-3,  7) ==  4);
    assert(imod( 3, -7) == -4);
    assert(imod(-3, -7) == -3);
    assert(imod( 100,  7) ==  2);
    assert(imod(-100,  7) ==  5);
    assert(imod( 100, -7) == -5);
    assert(imod(-100, -7) == -2);
    assert(imod(0, 5) == 0);
    assert(imod(0, -5) == 0);
    assert(imod( 9, 1) == 0);
    assert(imod( 9, -1) == 0);
    assert(imod(-9, 1) == 0);
    assert(imod(-9, -1) == 0);
    assert(imod( 22,  11) == 0);
    assert(imod( 22, -11) == 0);
    assert(imod(-22,  11) == 0);
    assert(imod(-22, -11) == 0);
  }

  cerr << "ok\n";
  return 0;
}
