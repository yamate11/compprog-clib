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

  template<typename RandomIt>
  void shuffle(RandomIt first, RandomIt last) { std::shuffle(first, last, rng); }

};

random_device Random::the_random_device;

// @@ !! END ---- random.cc

