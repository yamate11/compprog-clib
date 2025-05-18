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

// @@ !! LIM(sqrtDecomp f:<< random debug)

int main() {

  Random rand;

  { // defined interface
    struct S {
      ll a;
      ll b;
      S() : a(0), b(0) {}
    };
    SRD<S> srd1(10);
    assert(srd1.tot_size == 10);
    assert(srd1.bsize == 3);
    assert(srd1.numb == 4);
    assert(srd1.idx2pos(7).first == 2);
    assert(srd1.idx2pos(7).second == 1);
    assert(srd1.pos2idx(1, 2) == 5);
    using rpr_t = decltype(srd1.range_pos(0, 0));
    assert(srd1.range_pos(4, 5) == rpr_t(1, 1, -1, -1, 4, 5));
    assert(srd1.range_pos(4, 6) == rpr_t(2, 2, 4, 6, -1, -1));
    assert(srd1.range_pos(4, 7) == rpr_t(2, 2, 4, 6, 6, 7));
    assert(srd1.range_pos(6, 7) == rpr_t(2, 2, -1, -1, 6, 7));
    assert(srd1.range_pos(6, 9) == rpr_t(2, 3, -1, -1, -1, -1));
    assert(srd1.range_pos(6, 10) == rpr_t(2, 3, -1, -1, 9, 10));
    S& s = srd1.data(1);
    s.a = 100;
    assert(srd1.data(1).a == 100);
    assert(srd1.block_size(2) == 3);
    assert(srd1.block_size(3) == 1);
  }
  { // constructor arguments
    SRD<ll> srd1(100, 7);
    assert(srd1.bsize == 7);
    assert(srd1.numb == 15);
    SRD<ll> srd2(100, 1);
    assert(srd2.bsize == 1 and srd2.numb == 100);
    SRD<ll> srd3(100, 100);
    assert(srd3.bsize == 100 and srd3.numb == 1);
    SRD<ll> srd4(100, 1000);
    assert(srd4.bsize == 1000 and srd4.numb == 1 and srd4.block_size(0) == 100);
  }
  { // range sum query
    ll rep_num_1 = 200;
    ll rep_num_2 = 100;
    REP(_r1, 0, rep_num_1) {
      ll a_size = rand.range(1, 17);
      vector<ll> A(a_size);
      REP(i, 0, a_size) A[i] = rand.range(-10, 11);
      auto NA = A;
      struct S {
        ll shift;
        ll sum;
        S() : shift(0), sum(0) {}
      };
      SRD<S> srd(a_size);
      REP(i, 0, a_size) {
        auto [b, _j] = srd.idx2pos(i);
        srd.data(b).sum += A[i];
      }
      auto modify = [&](ll lo, ll hi, ll a) {  // A[i] += a for i \in [lo, hi)
        auto fe = [&](ll idx, ll b) {
          A[idx] += a;
          srd.data(b).sum += a;
        };
        auto fb = [&](ll b) {
          S& s = srd.data(b);
          s.sum += a * srd.block_size(b);
          s.shift += a;
        };
        srd.exec(lo, hi, fe, fb);
      };
      auto get_sum = [&](ll lo, ll hi) -> ll { // returns sum { A[i] | i \in [lo, hi) }
        ll ret = 0;
        auto fe = [&](ll idx, ll b) { ret += srd.data(b).shift + A[idx]; };
        auto fb = [&](ll b) { ret += srd.data(b).sum; };
        srd.exec(lo, hi, fe, fb);
        return ret;
      };
      REP(_r2, 0, rep_num_2) {
        ll op = rand.range(0, 2);
        ll lo, hi;
        while (true) {
          lo = rand.range(0, a_size + 1);
          hi = rand.range(0, a_size + 1);
          if (lo == hi) continue;
          if (lo > hi) swap(lo, hi);
          break;
        }
        if (op == 0) {
          ll a = rand.range(-10, 10);
          modify(lo, hi, a);
          REP(i, lo, hi) NA[i] += a;
        }else if (op == 1) {
          ll ans = get_sum(lo, hi);
          ll naive_ans = 0;
          REP(i, lo, hi) naive_ans += NA[i];
          assert(ans == naive_ans);
        }
      }
    }
  }


  cerr << "ok\n";
  
  return 0;
}
