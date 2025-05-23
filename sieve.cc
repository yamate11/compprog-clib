#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;

/*
  sieve.cc -- prime numbers and factorization

  vector<int> primes = sieve(n);
    // Returns the list of prime numbers upto n.
    // Performance:
    //   Size: upto(1e8).size() ... 5.7e6,  upto(1e9).size() ... 5.1e7
    //   Speed: upto(1e8) ... 0.7sec,   upto(1e9) ... 9.2sec

  vector<int> divSieve = divisorSieve(n);
    // Returns the list of the least prime divisors upto n.
    // E.g. divSieve[2] == 2, divSieve[15] == 3, divSieve[77] == 7, etc.

  vector<pair<ll, int>>  pr = prfac(m);
  vector<pair<ll, int>>  pr = prfac(m, primes);
  vector<pair<int, int>> pr = prfacDivSieve(m, divSieve);
    // Returns the prime factorization results.
    // The latter is naturally faster.
    // primes should contain prime numbers at least up to sqrt(m)
    // divSieve should contain divisors upto m.
    // Performance of `` REP(i, 1, N) prfac(i, primes) ''
    //   i = 1<<17 (1.3e5) ... 40 ms,  i = 1<<20 (1.0e6) ... 510 ms,   i = 1<<22 (4.2e6) ... 2900 ms
    // Performance of `` REP(i, 1, N) prfacDivSieve(i, divSieve) ''
    //   i = 1<<17 (1.3e5) ... 13 ms,  i = 1<<20 (1.0e6) ... 110 ms,   i = 1<<22 (4.2e6) ... 450 ms

  vector<ll>  divs = getDivisors(m);
  vector<ll>  divs = getDivisors(m, primes);
  vector<ll>  divs = getDivisorsDivSieve(m, divSieve);
    // Returns a list of all divisors of m.
    // NOTE: the results are *** NOT SORTED ***.
    // primes should contain prime numbers at least up to sqrt(m)
    // divSieve should contain divisors upto m.

  bool is_prime_MR(uint64_t n);
    // Miller Rabin prime judgement.

  vector<pair<ll, int>>> prfacPollardsRho(ll n);
    // Returns the prime factorization results, using Pollard's rho algorithm.    

*/

//////////////////////////////////////////////////////////////////////
// See help of libins command for dependency spec syntax.
// @@ !! BEGIN(mod random power) ---- sieve.cc

// sieve(upto) returns the list of prime numbers up to upto.
//   Size: upto(1e8).size() ... 5.7e6,  upto(1e9).size() ... 5.1e7
//   Performance: upto(1e8) ... 0.7sec,   upto(1e9) ... 9.2sec
vector<int> sieve(int upto) {
  vector<int> res;
  vector<bool> tbl(upto+1);
  ll lim = (int)(sqrt((double)upto)) + 1;
  for (int x = 2; x <= upto; x++) {
    if (tbl[x]) continue;
    res.push_back(x);
    if (x <= lim) {
      for (int y = x * x; y <= upto; y += x) tbl[y] = true;
    }
  }
  return res;
}

vector<int> divisorSieve(int upto) {
  vector<int> tbl(upto+1);
  iota(tbl.begin(), tbl.end(), 0);
  ll lim = (int)(sqrt((double)upto)) + 1;
  for (int x = 2; x <= lim; x++) {
    if (tbl[x] < x) continue;
    for (int y = x * x; y <= upto; y += x) if (tbl[y] == y) tbl[y] = x;
  }
  return tbl;
}

/*
  Prime Factorization
    Two versions:
      - prfac(n)
      - prfac(n, primes)
            primes should contain prime numbers at least up to sqrt(n)
 */

struct myiter_primes {
  const vector<int>& primes;
  int i;
  myiter_primes(const auto& primes_) : primes(primes_), i(0) {}
  int next() {
    if (i == ssize(primes)) return -1;
    else return primes[i++];
  }
};

struct myiter_int {
  int x;
  myiter_int() : x(0) {}
  int next() {
    x++;
    if (x == 1) { return 2; }
    if (x == 2) { return 3; }
    if (x % 2 == 1) { return 3 * x - 4; }
    return 3 * x - 5;
  }
};

vector<pair<ll, int>> _prfac_sub(ll n, auto& mit) {
  vector<pair<ll, int>> res;
  ll x = n;
  while (x > 1) {
    ll p = mit.next();
    if (p < 0) {
      throw runtime_error("_prfac_sub: prime range too small");
    }
    if (p * p > x) break;
    int r = 0;
    while (x % p == 0) {
      x /= p;
      r++;
    }
    if (r > 0) res.push_back(make_pair(p, r));
  }
  if (x > 1) res.push_back(make_pair(x, 1));
  return res;
}

vector<pair<ll, int>> prfac(ll n) {
  myiter_int mit;
  return _prfac_sub(n, mit);
}
vector<pair<ll, int>> prfac(ll n, const vector<int>& primes) {
  myiter_primes mit(primes);
  return _prfac_sub(n, mit);
}

vector<pair<int, int>> prfacDivSieve(int n, const vector<int>& divSieve) {
  vector<pair<int, int>> ret;
  int p = -1;
  int r = 0;
  while (n > 1) {
    int q = divSieve[n];
    if (p == q) {
      r++;
    }else {
      if (p > 0) ret.emplace_back(p, r);
      p = q;
      r = 1;
    }
    n /= p;
  }
  if (p > 0) ret.emplace_back(p, r);
  return ret;
}

/*
    List of divisors
      - getDivisors(n)
      - getDivisors(n, primes)
            primes should contain prime numbers at least up to sqrt(n)
    Note: the results are NOT sorted
 */

// _gdsub ... aux function used in getDivisors()
vector<ll> _gdsub(int i, const auto& fs) {
  if (i == (int)fs.size()) { return vector<ll>({1}); }
  auto part = _gdsub(i+1, fs);
  auto [p, r] = fs[i];
  ll pp = 1;    // pp = p^m, for m \in [1, r]
  int partOrigLen = part.size();
  for (int m = 1; m <= r; m++) {
    pp *= p;
    for (int j = 0; j < partOrigLen; j++) part.push_back(pp * part[j]);
  }
  return part;
}

vector<ll> getDivisors(ll n) { return _gdsub(0, prfac(n)); }
vector<ll> getDivisors(ll n, const vector<int>& primes) { return _gdsub(0, prfac(n, primes)); }
vector<ll> getDivisorsDivSieve(ll n, const vector<int>& divSieve) { return _gdsub(0, prfacDivSieve(n, divSieve)); }

// Miller-Rabin prime judgement
// is_prime_MR(n) returns true iff n is prime for u64 n.
//     The original code is on https://zenn.dev/mizar/articles/791698ea860581

#include <cstdbool>
#include <cstdint>
uint64_t modmul(uint64_t a, uint64_t b, uint64_t n) {
  return (uint64_t)(((__uint128_t)a) * ((__uint128_t)b) % ((__uint128_t)n));
}
uint64_t modpow(uint64_t a, uint64_t b, uint64_t n) {
  uint64_t t = ((b & 1) == 0) ? 1 : a;
  for (b >>= 1; b != 0; b >>= 1) {
    a = modmul(a, a, n);
    if ((b & 1) == 1) { t = modmul(t, a, n); } 
  }
  return t;
}
const uint64_t bases[] = {2,325,9375,28178,450775,9780504,1795265022};
bool is_prime_MR(uint64_t n) {
  if (n == 2) { return true; }
  if (n < 2 || (n & 1) == 0) { return false; }
  uint64_t n1 = n - 1, d = n - 1;
  uint32_t s = 0;
  for (; (d & 1) == 0; d >>= 1) { s += 1; }
  for (const auto& base : bases) {
    uint64_t a = base;
    if (a >= n) {
      a %= n;
      if (a == 0) { continue; }
    }
    uint64_t t = modpow(a, d, n);
    if (t == 1) { continue; }
    for (uint32_t j = 1; t != n1; ++j) {
      if (j >= s) { return false; }
      t = modmul(t, t, n);
    }
  }
  return true;
}

ll findPrimeFactor(ll n) {
  if (n % 2 == 0) return 2;
  ll m = llround(ceil(pow((double)n, 0.125)));
  auto loop_c = [&]() -> ll {
    auto sub_c = [&](int c) -> ll {
      ll y = 0;
      auto sub_r = [&](int r) -> ll {
        auto next_rand = [&](__int128 a) -> ll { return (a * a + c) % n; };

        ll x = y;
        int th1 = 3 * r / 4;
        for (int k = 0; k < th1; k++) y = next_rand(y);
        for (int j = 0; th1 + j * m < r; j++) {
          auto sub_range_all = [&](ll st, ll len) -> ll {
            ll en = st + len > r ? r : st + len;
            ll q = 1;
            for (int k = st; k < en; k++) {
              y = next_rand(y);
              q = (__int128)q * abs(x - y) % n;
            }
            return gcd(q, n);
          };

          auto sub_range_each = [&](ll st, ll len) -> ll {
            ll en = st + len > r ? r : st + len;
            for (int k = st; k < en; k++) {
              y = next_rand(y);
              if (ll g = gcd(abs(x - y), n); g != 1) return g;
            }
            assert(0);
          };

          ll y_save = y;
          auto g = sub_range_all(th1 + j * m, m);
          if (g == n) {
            y = y_save;
            return sub_range_each(th1 + j * m, m);
          }else if (g != 1) return g;
        }
        return 1;
      };

      for (int r = 1; true; r *= 2) { if (ll res = sub_r(r); res != 1) return res; }
    };

    for (int c = 1; true; c++) { if (ll res = sub_c(c); res != n) return res; }
  };

  ll v = loop_c();
  if      (is_prime_MR(v    )) return v;
  else if (is_prime_MR(n / v)) return n / v;
  else return findPrimeFactor(v);
}

vector<pair<ll, int>> prfacPollardsRho(ll n) {
  vector<pair<ll, int>> ret;
  while (n > 1 and not is_prime_MR(n)) {
    ll p = findPrimeFactor(n);
    int r = 0;
    while (n % p == 0) {
      n /= p;
      r++;
    }
    ret.emplace_back(p, r);
  }
  if (n > 1) ret.emplace_back(n, 1);
  sort(ret.begin(), ret.end());
  return ret;
}

// @@ !! END ---- sieve.cc
