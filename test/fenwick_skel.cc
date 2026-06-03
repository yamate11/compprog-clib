#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;
using pll = pair<ll, ll>;
using u64 = unsigned long long;

// @@ !! LIM(f:<< debug random fenwick)

Random myrand;

template<typename T>
void mytest1(ll rep1, ll rep2, T unit, auto f_add, auto datagen) {
  for (ll r = 0; r < rep1; r++) {
    ll n = myrand.range(1, 15);
    vector<T> vec(n);
    for (ll i = 0; i < n; i++) vec[i] = datagen();
    auto fwt = make_fenwick_tree(unit, f_add, vec);
    for (ll r2 = 0; r2 < rep2; r2++) {
      if (myrand.range(0, 2) == 0) {
        ll i = myrand.range(0, n);
        T x = datagen();
        vec[i] = f_add(vec[i], x);
        fwt.add(i, x);
      }else {
        ll k = myrand.range(0, n + 1);
        T c = unit;
        for (ll i = 0; i < k; i++) c = f_add(c, vec[i]);
        T y = fwt.query(k);
        assert(y == c);
      }
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << setprecision(20);

  {
    vector<ll> init{5, 2, 4, -3, 1};
    auto fwt = make_fenwick_tree(0LL, plus<ll>(), init);
    assert(fwt.query(3) == 11);
    fwt.add(1, -3);   // values are: [5, -1, 4, -3, 1]
    assert(fwt.query(4) == 5);
    assert(fwt.show() == "[5, 4, 8, 5, 6]");  // prefix sums
    stringstream ss;
    ss << fwt;
    assert(ss.str() == "[5, 4, 8, 5, 6]");
  }
  {
    auto fwt = make_fenwick_tree(1LL << 60, [](ll x, ll y) { return min(x, y); });
    fwt.set_data(vector<ll>{20, 25, 15, 15, 5, 10, 12, 5, 0});
    assert(fwt.show() == "[20, 20, 15, 15, 5, 5, 5, 5, 0]");
    fwt.add(3, 40);
    assert(fwt.query(4) == 15);
    fwt.add(3, 12);
    assert(fwt.query(4) == 12);
    assert(fwt.query(3) == 15);
  }

  ll rep1 = 500;
  ll rep2 = 500;

  auto llgen = [&]() { return myrand.range(-10, 11); };
  mytest1<ll>(rep1, rep2, 0, plus<ll>(), llgen);
  mytest1<ll>(rep1, rep2, -(1LL << 60), [](ll x, ll y) { return max(x, y); }, llgen);

  pll unit_dplus(0, 0);
  auto dplus = [](pll a, pll b) { return pll(a.first + b.first, a.second + b.second); };
  auto pllgen = [&]() { return pll(myrand.range(-10, 11), myrand.range(-10, 11)); };
  mytest1<pll>(rep1, rep2, unit_dplus, dplus, pllgen);

  cerr << "Test done." << endl;
  return 0;
}

