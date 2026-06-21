#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;

/*
  mod.cc
    defines struct FpG (finite Field for prime p; General version)

  Usually, what you need to do is "using Fp = FpA;" or "using Fp = FpB",
  because of the following definitions:

        constexpr int primeA = 1'000'000'007;
        constexpr int primeB = 998'244'353;
        using FpA = FpG<primeA>;
        using FpB = FpG<primeB>;

  If you need to dynamically change the value of mod.  

        FpG<0>::setMod(primeA);
        FpG<0> x; 
        ....

  FpG<mod>::getMod() is also defined.

  If two or more dynamic mods are required, write something like:
        FpG<0, ll, 1>::setMod(m1);
        FpG<0, ll, 2>::setMod(m2);
        FpG<0, ll, 1> a, b;  cout << a * b;   // mod m1
        FpG<0, ll, 2> c, d;  cout << c * d;   // mod m2

  With Comb<T>, you can do something like:

        Comb<FpA> cb(1000);          // args upto 1000 (=: nMax), inclusive.
        FpA x = cb.fact(1000);       // factorial
        FpA x1 = cb.inv_fact(1000);  // 1 / 1000!
        FpA y = cb.binom(500, 300);  // combination.    binom(int n, int r).  Valid only for 0 <= n <= nMax.
                                     // When 0 < r or n < r, it returns 0 (that conforms to the definition).
        FpA yy = cb.binom_dup(300, 500); // combination with duplicate (== binom(300+500-1, 500))
        FpA z = cb.perm(500, 300);   // permutation

  Generalized version: FpG<0, INT>, where INT is a signed integer type such as ll, __int128 or cpp_int.

 */

//////////////////////////////////////////////////////////////////////
// See help of libins command for dependency spec syntax.
// @@ !! BEGIN(f:gcd) ---- mod.cc

template<int mod=0, typename INT=ll, int dyn_id=0>
struct FpG {   // G for General
  inline static INT dyn_mod = 0;

  static INT getMod() {
    if constexpr (mod == 0) return dyn_mod;
    else                    return (INT)mod;
  }
  
  // Effective only when mod == 0.
  static void setMod(INT _mod) { dyn_mod = _mod; }

  static INT _conv(INT x) {
    if (x >= getMod())  return x % getMod();
    if (x >= 0)         return x;
    if (x >= -getMod()) return x + getMod();
    INT y = x % getMod();
    if (y == 0) return 0;
    return y + getMod();
  }

  INT val;

  FpG(INT t = 0) : val(_conv(t)) {}
  FpG(const FpG& t) : val(t.val) {}
  FpG& operator =(const FpG& t) { val = t.val; return *this; }
  FpG& operator =(INT t) { val = _conv(t); return *this; }

  FpG& operator +=(const FpG& t) {
    val += t.val;
    if (val >= getMod()) val -= getMod();
    return *this;
  }

  FpG& operator -=(const FpG& t) {
    val -= t.val;
    if (val < 0) val += getMod();
    return *this;
  }

  FpG& operator *=(const FpG& t) {
    val = (val * t.val) % getMod();
    return *this;
  }

  FpG inv() const {
    if (val == 0) { throw runtime_error("FpG::inv(): called for zero."); }
    auto [g, u, v] = eGCD(val, getMod());
    if (g != 1) { throw runtime_error("FpG::inv(): not co-prime."); }
    return FpG(u);
  }

  FpG zero() const { return (FpG)0; }
  FpG one() const { return (FpG)1; }
  FpG inverse() const { return inv(); }

  FpG& operator /=(const FpG& t) {
    return (*this) *= t.inv();
  }

  FpG operator +(const FpG& t) const { return FpG(val) += t; }
  FpG operator -(const FpG& t) const { return FpG(val) -= t; }
  FpG operator *(const FpG& t) const { return FpG(val) *= t; }
  FpG operator /(const FpG& t) const { return FpG(val) /= t; }
  FpG operator -() const { return FpG(-val); }

  bool operator ==(const FpG& t) const { return val == t.val; }
  bool operator !=(const FpG& t) const { return val != t.val; }
  
  operator INT() const { return val; }

  friend FpG operator +(INT x, const FpG& y) { return FpG(x) + y; }
  friend FpG operator -(INT x, const FpG& y) { return FpG(x) - y; }
  friend FpG operator *(INT x, const FpG& y) { return FpG(x) * y; }
  friend FpG operator /(INT x, const FpG& y) { return FpG(x) / y; }
  friend bool operator ==(INT x, const FpG& y) { return FpG(x) == y; }
  friend bool operator !=(INT x, const FpG& y) { return FpG(x) != y; }
  friend FpG operator +(const FpG& x, INT y) { return x + FpG(y); }
  friend FpG operator -(const FpG& x, INT y) { return x - FpG(y); }
  friend FpG operator *(const FpG& x, INT y) { return x * FpG(y); }
  friend FpG operator /(const FpG& x, INT y) { return x / FpG(y); }
  friend bool operator ==(const FpG& x, INT y) { return x == FpG(y); }
  friend bool operator !=(const FpG& x, INT y) { return x != FpG(y); }

  /* The following are needed to avoid warnings in cases such as FpG x; x = 5 + x; rather than x = FpG(5) + x; */
  friend FpG operator +(int x, const FpG& y) { return FpG(x) + y; }
  friend FpG operator -(int x, const FpG& y) { return FpG(x) - y; }
  friend FpG operator *(int x, const FpG& y) { return FpG(x) * y; }
  friend FpG operator /(int x, const FpG& y) { return FpG(x) / y; }
  friend bool operator ==(int x, const FpG& y) { return FpG(x) == y; }
  friend bool operator !=(int x, const FpG& y) { return FpG(x) != y; }
  friend FpG operator +(const FpG& x, int y) { return x + FpG(y); }
  friend FpG operator -(const FpG& x, int y) { return x - FpG(y); }
  friend FpG operator *(const FpG& x, int y) { return x * FpG(y); }
  friend FpG operator /(const FpG& x, int y) { return x / FpG(y); }
  friend bool operator ==(const FpG& x, int y) { return x == FpG(y); }
  friend bool operator !=(const FpG& x, int y) { return x != FpG(y); }

  friend istream& operator>> (istream& is, FpG& t) {
    INT x; is >> x;
    t = x;
    return is;
  }

  friend ostream& operator<< (ostream& os, const FpG& t) {
    os << t.val;
    return os;
  }
};

// for std::format()
template<int mod, typename INT, int dyn_id>
struct formatter<FpG<mod, INT, dyn_id>> : formatter<INT>  {
  auto format(const FpG<mod, INT, dyn_id>& x, format_context& ctx) const {
    return formatter<INT>::format(static_cast<INT>(x), ctx);
  }
};

template<typename T>
class Comb {
  int nMax;
  vector<T> vFact;
  vector<T> vInvFact;
public:
  Comb(int nm) : nMax(nm), vFact(nm+1), vInvFact(nm+1) {
    vFact[0] = 1;
    for (int i = 1; i <= nMax; i++) vFact[i] = i * vFact[i-1];
    vInvFact.at(nMax) = (T)1 / vFact[nMax];
    for (int i = nMax; i >= 1; i--) vInvFact[i-1] = i * vInvFact[i];
  }
  T fact(int n) { return vFact[n]; }
  T inv_fact(int n) { return vInvFact[n]; }
  T binom(int n, int r) {
    if (r < 0 || r > n) return (T)0;
    return vFact[n] * vInvFact[r] * vInvFact[n-r];
  }
  T binom_dup(int n, int r) { return binom(n + r - 1, r); }
  // The number of permutation extracting r from n.
  T perm(int n, int r) {
    return vFact[n] * vInvFact[n-r];
  }
};

constexpr int primeA = 1'000'000'007;
constexpr int primeB = 998'244'353;          // '
using FpA = FpG<primeA, ll>;
using FpB = FpG<primeB, ll>;

// @@ !! END ---- mod.cc

