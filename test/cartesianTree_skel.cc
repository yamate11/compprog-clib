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

// @@ !! LIM(cartesianTree random debug)

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << setprecision(20);

  {
    vector<ll> vec{80, 90, 20, 50, 50, 40};
    auto cp = make_cartesian_tree(vec);
    assert(cp.root == 2);
    assert(cp.left[2] == 0);
    assert(cp.left[0] == -1);
    assert(cp.right[2] == 5);
    assert(cp.left[5] == 3 or cp.left[5] == 4);

    auto cp1 = make_cartesian_tree(vec, less<ll>());
    assert(cp1.root == 2);

    auto cp2 = make_cartesian_tree(vec, [](ll a, ll b) { return a < b; });
    assert(cp2.root == 2);

    CartesianTree<vector<ll>> cp3;
    cp3.build(vec);
    assert(cp3.root == 2);
    
    auto cp4 = make_cartesian_tree(vec, greater<ll>());
    assert(cp4.root == 1);
    assert(cp4.right[1] == 3 or cp4.right[1] == 4);

    auto cp5 = make_cartesian_tree(vector<ll>{});
    assert(cp5.root == -1);
    assert(cp5.left.empty() and cp5.right.empty());
  }
  
  {
    vector<ll> vec{100, 10, 50, 10, 40, 10, 100};
    auto comp1 = [&](ll i, ll j) -> bool {
      if (vec[i] != vec[j]) return vec[i] < vec[j];
      return i < j;
    };
    auto comp2 = [&](ll i, ll j) -> bool {
      if (vec[i] != vec[j]) return vec[i] < vec[j];
      return i > j;
    };
    auto comp3 = [&](ll i, ll j) -> bool {
      if (vec[i] != vec[j]) return vec[i] > vec[j];
      return i < j;
    };
    auto comp4 = [&](ll i, ll j) -> bool {
      if (vec[i] != vec[j]) return vec[i] > vec[j];
      return i > j;
    };
    auto ct1 = make_cartesian_tree_comp_index(vec, comp1);
    assert(ct1.root == 1);
    auto ct2 = make_cartesian_tree_comp_index(vec, comp2);
    assert(ct2.root == 5);
    auto ct3 = make_cartesian_tree_comp_index(vec, comp3);
    assert(ct3.root == 0);
    auto ct4 = make_cartesian_tree_comp_index(vec, comp4);
    assert(ct4.root == 6);
  }

  {
    auto check = [&](const auto& cp, const auto& vec, auto comp) -> void {
      auto sub = [&](auto rF, ll lo, ll hi, ll x) -> void {
        ll a = *min_element(vec.begin() + lo, vec.begin() + hi, comp);
        assert(a == vec[x]);
        if (cp.left[x] == -1) assert(lo == x);
        else rF(rF, lo, x, cp.left[x]);
        if (cp.right[x] == -1) assert(x + 1 == hi);
        else rF(rF, x + 1, hi, cp.right[x]);
      };
      sub(sub, 0, ssize(vec), cp.root);
    };

    Random rand;
    ll repeat = 10000;
    REP(_r, 0, repeat) {
      ll n = rand.range(1, 10);
      vector<ll> vec(n);
      REP(i, 0, n) vec[i] = rand.range(10, 10 + n);
      ll t = rand.range(0, 3);
      if (t == 0) {
        CartesianTree cp(vec);
        check(cp, vec, less<ll>());
      }else if (t == 1) {
        CartesianTree cp(vec, [&](ll a, ll b) { return a < b; });
        check(cp, vec, less<ll>());
      }else if (t == 2) {
        CartesianTree cp(vec, greater<ll>());
        check(cp, vec, greater<ll>());
      }
      
    }
  }


  cout << "ok\n";
}
