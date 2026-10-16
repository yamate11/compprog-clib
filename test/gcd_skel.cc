#include <bits/stdc++.h>
#include <cassert>
using namespace std;
using ll = long long int;
using u64 = unsigned long long;
using pll = pair<ll, ll>;
#define REP(i, a, b) for (ll i = (a); i < (b); i++)
#define REPrev(i, a, b) for (ll i = (a); i >= (b); i--)
#define ALL(coll) (coll).begin(), (coll).end()

// @@ !! LIM(gcd debug)

int main() {

  random_device rd;
  mt19937 rng(rd());

  {
    vector<pair<ll, ll>> v({
	{5,8},{540,150},{-70,105},{14,-30},{-33,24},{-35,-99}
        ,{-70,105},{105,-70},{70,-105},{-105,70},{70,105},{105,70},{-70,-105},{-105,-70}
	,{0,6},{6,0},{0,-6},{-6,0},{0,0}});
    for (auto [a, b] : v) {
      auto [g, s, t] = eGCD(a, b);
      assert(g == gcd(a, b) && a * s + b * t == g);
    }
  }

  {
    for (ll x1 = 1; x1 <= 12; x1++) {
      for (ll x2 = 1; x2 <= 12; x2++) {
	vector rr(x1, vector(x2, -1));
	ll lcm = x1 * x2 / gcd(x1, x2);
	for (ll r = 0; r < lcm; r++) rr[r % x1][r % x2] = (int)r;
	for (ll a1 = 0; a1 < x1; a1++) {
	  for (ll a2 = 0; a2 < x2; a2++) {
	    for (ll c1 = -2; c1 <= 2; c1++) {
	      for (ll c2 = -2; c2 <= 2; c2++) {
		ll aa1 = a1 + c1 * x1;
		ll aa2 = a2 + c2 * x2;

		assert(crt(aa1, x1, aa2, x2) == rr[a1][a2]);
	      }
	    }
	  }
	}
      }
    }
    
    vector<ll> rcs({0,1,53,37193,409287402,32123784915});
    vector<ll> xs({  2,4,  8,  3,5, 6,15,24,30});
    for (ll rc : rcs) {
      vector<ll> as;
      for (ll x : xs) as.push_back(rc % x);
      ll r = crt(as, xs);
      // DLOGK(as, xs);
      assert(r >= 0);
      for (ll i = 0; i < (ll)as.size(); i++) {
	assert((r - as[i]) % xs[i] == 0);
      }
    }
    vector<ll> as1({1,1,1,1,1,4,1,1,1});
    assert(crt(as1, xs) == -1);
  }

  {
    uniform_int_distribution<ll> dist9(1, (ll)1e9);
    uniform_int_distribution<ll> dist9z(-(ll)1e9, (ll)1e9);
    uniform_int_distribution<ll> dist18(1, (ll)1e18);
    for (ll i = 0; i < 1000; i++) {
      ll r0 = dist18(rng);
      ll x1 = dist9(rng);
      ll x2 = dist9(rng);
      ll a1 = r0 % x1;
      ll a2 = r0 % x2;
      a1 += dist9z(rng) * x1;
      a2 += dist9z(rng) * x2;
      ll r = crt(a1, x1, a2, x2);
      bool b = r >= 0 && (r - a1) % x1 == 0 && (r - a2) % x2 == 0;
      if (!b) {
	DLOGK(r, a1, x1, a2, x2);
	assert(false);
      }
    }
  }

  cerr << "ok\n";
  return 0;
}
