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
    ll big = 1LL << 60;
    LiChaoDyn lct(-200, 200);
    // DLOGKL("just before calling set_unit", big);
    lct.set_unit(big);
    lct.add_segment(1, 0, -100, 100);
    lct.add_segment(1, -1, 20, 30);
    lct.add_segment(-1, -10, -6, -4);
    // DLOG(lct.innerstr());
    assert(lct.query(100) == 100);
    assert(lct.query(101) == big);
    // DLOGK(lct.query(0));
    assert(lct.query(0) == 0);
    assert(lct.query(19) == 19);
    assert(lct.query(20) == 19);
    assert(lct.query(25) == 24);
    assert(lct.query(30) == 29);
    assert(lct.query(31) == 31);
    assert(lct.query(-4) == -6);
    assert(lct.query(-5) == -5);
    assert(lct.query(-6) == -6);
  }
  {
    LiChaoDyn lct(-100, 100);
    try {
      lct.add_segment(1, 0, -10, 10);
      // calling add_segment without set_unit should be an error.
      assert(0);
    }catch (runtime_error& e) {
    }
  }

  {
    ll big = 1LL << 60;
    auto func = [&rand](auto comp, ll unit) -> void {
#if DEBUG
      ll rep1 = 100, rep2 = 100;
#else
      ll rep1 = 400, rep2 = 400;
      // ll rep1 = 2000, rep2 = 2000;
#endif
      for (int _r = 0; _r < rep1; _r++) {
        ll rngmin = rand.range(-rep2 * 2, rep2 * 2);
        ll rngmax = rngmin + rand.range(1, rep2 * 2);
        LiChaoDyn<ll, decltype(comp)> lct(rngmin, rngmax, comp);
        lct.set_unit(unit);
        vector<ll> va, vb;
        vector<ll> ra, rb;
        for (int _rr = 0; _rr < rep2; _rr++) {
          ll a = rand.range(-15, 16);
          ll b = rand.range(-100, 100);
          va.push_back(a);
          vb.push_back(b);
          if (rand.range(0, 2) == 0) {
            lct.add_line(a, b);
            ra.push_back(rngmin);
            rb.push_back(rngmax);
          }else {
            ll ta = rngmin + rand.range(0, rngmax - rngmin + 1);
            ll tb = ta + rand.range(0, rngmax - ta + 1);
            lct.add_segment(a, b, ta, tb);
            ra.push_back(ta);
            rb.push_back(tb);
          }
          ll t = rand.range(rngmin, rngmax + 1);
          ll nv = unit;
          for (int i = 0; i < ssize(va); i++) {
            if (ra[i] <= t and t <= rb[i]) {
              ll w = va[i] * t + vb[i];
              if (comp(w, nv)) nv = w;
            }
          }
          assert(lct.query(t) == nv);
        }
      }
    };
    func(less<ll>(), big);
    func(greater<ll>(), -big);
  }


  cerr << "ok\n";

  return 0;
}
