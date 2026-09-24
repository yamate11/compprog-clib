#include <bits/stdc++.h>
#include <cassert>
using namespace std;
using ll = long long int;
using pll = pair<ll, ll>;
using ull = unsigned long long;

#if DEBUG
#define LIB_DEBUG 1
#endif

// @@ !! LIM(debug random LiChao)

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << setprecision(20);
  Random rand;

  {
    LiChaoDyn lctL(-200, 200);
    lctL.add_line(-1, 10);
    lctL.add_line(1, 0);
    lctL.add_line(2, 20);
    // DLOG(lctL.innerstr());
    assert(lctL.query(-100) == -180);
    assert(lctL.query(0) == 0);
    assert(lctL.query(100) == -90);
    LiChaoDyn<ll, greater<ll>> lctG(-200, 200);
    lctG.add_line(-1, 10);
    lctG.add_line(1, 0);
    lctG.add_line(2, 20);
    assert(lctG.query(-100) == 110);
    assert(lctG.query(0) == 20);
    assert(lctG.query(100) == 220);
  }

  {
    auto func = [&rand](auto comp) -> void {
#if DEBUG
      ll rep1 = 100, rep2 = 100;
#else
      ll rep1 = 500, rep2 = 500;
#endif
      for (int _r = 0; _r < rep1; _r++) {
        ll rngmin = rand.range(-rep2 * 2, rep2 * 2);
        ll rngmax = rngmin + rand.range(1, rep2 * 2);
        LiChaoDyn<ll, decltype(comp)> lct(rngmin, rngmax, comp);
        vector<ll> va, vb;
        for (int _rr = 0; _rr < rep2; _rr++) {
          ll a = rand.range(-15, 16);
          ll b = rand.range(-100, 100);
          lct.add_line(a, b);
          va.push_back(a);
          vb.push_back(b);
          ll t = rand.range(rngmin, rngmax + 1);
          ll nv = va[0] * t + vb[0];
          for (int i = 1; i < ssize(va); i++) {
            ll w = va[i] * t + vb[i];
            if (comp(w, nv)) nv = w;
          }
          assert(lct.query(t) == nv);
        }
      }
    };
    func(less<ll>());
    func(greater<ll>());
  }


  cerr << "ok\n";

  return 0;
}
