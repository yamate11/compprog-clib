#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;

/* 
   (1)
   Power function.   power<T>(a, b)   ... $a^{b}$
   Time complexity is O(log b).

   The implementation refers to one<T>().  Thus, basic types such as int and double are supported, as well as
   mod classes.  To use this for your own class, define it using MyAlg<Op> in algOp.cc.

   It holds that power(t, 0) == one<T>(), even for t == 0.


   (2)
   sqrt, root, power, log functions for long long
   llpower(a, b)   ...  a^{b} or -1 if overflow   for a >= 0, b >= 0
   llsqrt(a)       ...  returns x s.t. x*x <= a < (x+1)*(x+1)     for a >= 0
   llroot(a, m)    ...  returns x s.t. x^{1/m} <= a < (x+1)^{1/m} for a >= 0, m >= 2
   lllog(b, a)     ...  returns x s.t. b^{x} <= a < b^{x+1}       for b >= 2, a >= 1

 */

//////////////////////////////////////////////////////////////////////
// See help of libins command for dependency spec syntax.
// @@ !! BEGIN(algOp) ---- power.cc

template<typename T>
T power(const T& a, ll b) {
  auto two_pow = a;
  auto ret = one<T>(a);
  while (b > 0) {
    if (b & 1LL) ret *= two_pow;
    two_pow *= two_pow;
    b >>= 1;
  }
  return ret;
}

// a >= 0, b >= 0;  If overflow, returns -1.
ll llpower(ll a, ll b) {  
  if (b == 0) return 1;   // 0^0 == 1
  if (b == 1) return a;
  if (a == 0) return 0;
  if (a == 1) return 1;
  if (a == 2) {
    if (b >= 63) return -1;
    else return 1LL << b;
  }
  if (b == 2) {
    ll ret;
    if (__builtin_smulll_overflow(a, a, &ret)) return -1;
    return ret;
  }
  ll two_pow = a;
  ll ret = 1;
  assert(b > 0);
  while (true) {
    if (b & 1LL) {
      if (__builtin_smulll_overflow(ret, two_pow, &ret)) return -1;
    }
    b >>= 1;
    if (b == 0) break;
    if (__builtin_smulll_overflow(two_pow, two_pow, &two_pow)) return -1;
  }
  return ret;
}

// a >= 0;   Returns x s.t. x*x <= a < (x+1)*(x+1)
ll llsqrt(ll a) {
  ll x = llround(sqrt((double)a));
  ll y;
  if (__builtin_smulll_overflow(x, x, &y) or a < y) return x - 1;
  else return x;
}

// a >= 0, m >= 2;  Returns x s.t. x^m <= a < (x + 1)^m
ll llroot(ll a, ll m) {
  ll x = llround(pow(a, 1.0 / m));
  ll y = llpower(x, m);
  if (y == -1 or a < y) return x - 1;
  else return x;
}

//  base >= 2, a >= 1;  Returns x s.t. base^{x} <= a < base^{x + 1}
ll lllog(ll base, ll a) {
  ll x = llround(log(a) / log(base));
  ll y = llpower(base, x);
  if (y == -1 or a < y) return x - 1;
  else return x;
}


// @@ !! END ---- power.cc
