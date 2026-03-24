#include <bits/stdc++.h>
#include <cassert>
using namespace std;
using ll = long long int;

/*
  Random

  Usage:

    Random rand;   // non-deterministic seed
    ll x = rand.range(-3, 4);   // uniform distribution on {-3, -2, -1, 0, 1, 2, 3}
    vector<ll> v = ...;
    rand.shuffle(ALL(v));  // reorder v

    Random randA(314159);  // specified seed
    ll x = rand.range(-3, 4);   // same

*/


//////////////////////////////////////////////////////////////////////
// See help of libins command for dependency spec syntax.
// @@ !! BEGIN()    ---- random.cc


struct Random {
  static random_device the_random_device;
  mt19937_64 rng;

  Random() : rng(the_random_device()) {}
  Random(unsigned seed) : rng(seed) {}
  
  ll range(ll i, ll j) {
    if (i >= j) {
      throw runtime_error("Random.range: invalid range");
    }
    uniform_int_distribution<ll> dist(i, j - 1);
    return dist(rng);
  }

  double rand(double a, double b) {
    if (!(a <= b)) {
      throw std::runtime_error("Random.rand: invalid range");
    }
    std::uniform_real_distribution<double> dist(a, b);
    return dist(rng);
  }

  pair<ll, ll> select2(ll i, ll j) {
    if (j - i < 2) {
      throw std::runtime_error("Random.select2: range too small");
    }
    ll x = range(i, j);
    ll y = range(i, j - 1);
    if (y >= x) ++y;
    if (x > y) swap(x, y);
    return {x, y};
  }

  template<typename RandomIt>
  void shuffle(RandomIt first, RandomIt last) { std::shuffle(first, last, rng); }

};

random_device Random::the_random_device;

// @@ !! END ---- random.cc

