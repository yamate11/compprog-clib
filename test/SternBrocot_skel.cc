#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;
using pll = pair<ll, ll>;

// @@ !! LIM(SternBrocot random)

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << setprecision(20);

  Random rand;

  {
    SternBrocotNode n1;
    SternBrocotNode n2(3, 5);
    SternBrocotNode n3({0, 1, 1, 2});
    assert(n2 == n3);
    n2.coeff == vector<ll>{0, 1, 1, 2};
    assert(not (n1 == n2));
    assert(n1 != n2);
    assert(n2 < n1);
    assert(n2 <= n1);
    assert(n1 > n2);
    assert(n1 >= n2);
    n1.set(3, 5);
    assert(n1 == n2);
    n1.set({0, 1, 1, 2});
    assert(n1 == n3);
    assert(n3.numden() == pll(3, 5));
    assert(not n2.is_root());
    assert(SternBrocotNode().is_root());
    assert(n3.depth() == 3);
    SternBrocotNode n4(vector<ll>{0, 3});
    SternBrocotNode n5(vector<ll>{0, 2});
    assert(n3.lca(n4) == n5);
    assert(SternBrocotNode::lca(n3, n4) == n5);
    assert(n4.parent() == n5);
    assert(SternBrocotNode(3, 4).big_step_parent() == n5);
    SternBrocotNode n6({0, 1, 2});
    assert(n5.child_lo() == n4);
    assert(n5.child_hi() == n6);
    SternBrocotNode n7(3, 5);
    assert(n7.ancestor(1) == n5);
    auto p = n5.range();
    assert(not p.first and p.second and *p.second == SternBrocotNode());
  }

  {
    SternBrocotNode n1;
    SternBrocotNode n2(1, 1);
    assert(n1 == n2);
    SternBrocotNode n3(4, 6);
    SternBrocotNode n4(2, 3);
    assert(n3 == n4);
  }
  {
    ll rep = 1000;
    for (ll _t = 0; _t < rep; _t++) {
      ll i = rand.range(1, 10);
      ll j = rand.range(1, 10);
      SternBrocotNode n1(i, j);
      vector<ll> vec1 = n1.coeff;
      SternBrocotNode n2(vec1);
      assert(n1 == n2);
      auto [x, y] = n1.numden();
      assert(i * y == j * x);
      assert((i == j) == n1.is_root());

      auto nl = n1.child_lo();
      auto nh = n1.child_hi();
      assert(nl < n1 and n1 < nh);
      assert(n1.depth() == nl.depth() - 1 and n1.depth() == nh.depth() - 1);
      assert(nl.parent() == n1 and nh.parent() == n1);

      if (not n1.is_root()) {
        vector<SternBrocotNode> vec2{n1};
        auto xx = n1;
        while (not xx.is_root()) {
          auto np = xx.parent();
          vec2.push_back(np);
          xx = np;
        }
        assert(vec2.back().is_root());
        ll k0 = 0;
        for (ll ii = 0; ii < ssize(vec2); ii++) {
          assert(vec2[0].ancestor(vec2[0].depth() - ii) == vec2[ii]);
          if (ii == ssize(vec2) - 1 or (ii > 0 and (vec2[ii - 1] < vec2[ii]) != (vec2[ii] < vec2[ii + 1]))) {
            for (ll kk = k0; kk < ii; kk++) assert(vec2[kk].big_step_parent() == vec2[ii]);
            k0 = i;
          }
        }
      }
      auto [lo, hi] = n1.range();
      if (n1.is_root()) {
        assert(not lo and not hi);
      }else {
        auto pt = n1.parent();
        if (pt < n1) assert(*lo == pt and hi == pt.range().second);
        else         assert(*hi == pt and lo == pt.range().first);
      }

      ll k = rand.range(1, 10);
      ll l = rand.range(1, 10);
      SternBrocotNode m1(k, l);
      if (i * l == j * k) {
        assert(n1 == m1 and n1 <= m1 and n1 >= m1 and m1 <= n1 and m1 >= n1);
        assert(n1.lca(m1) == n1);
      }else {
        if (i * l < j * k) {
          assert(n1 < m1 and n1 <= m1 and m1 > n1 and m1 >= n1 and n1 != m1 and m1 != n1);
        }else {
          assert(m1 < n1 and m1 <= n1 and n1 > m1 and n1 >= m1 and m1 != n1 and n1 != m1);
        }
        auto o1 = SternBrocotNode::lca(n1, m1);
        ll d1 = o1.depth();
        assert(n1.ancestor(d1) == o1 and m1.ancestor(d1) == o1);
        if (n1 != o1 and m1 != o1) assert(n1.ancestor(d1 + 1) != m1.ancestor(d1 + 1));
      }
    }
  }

  return 0;
}

