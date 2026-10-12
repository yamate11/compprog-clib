#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;

/* 
   Rational

   Template parameter INT is mandatory (design choice).
   Something like "use Rat = Rational<ll>;" or "use Rat = Rational<__int128>;" is recommended.
 */

//////////////////////////////////////////////////////////////////////
// See help of libins command for dependency spec syntax.
// @@ !! BEGIN() ---- rational.cc

template<class T> string g_show(const T& x);

template<typename INT>
struct Rational {
  INT num;
  INT den;

  Rational(type_identity_t<INT> num_ = 0, type_identity_t<INT> den_ = 1) : num(num_), den(den_) { reduction(); }
  void reduction() {
    if (den == 0) { throw runtime_error("Rational.reduction: division by zero"); }
    if (den < 0) {
      num = -num;
      den = -den;
    }
    INT g = gcd(num, den);
    num /= g;
    den /= g;
  }

  bool operator==(const Rational& o) const = default;
  strong_ordering operator<=>(const Rational& o) const {
    INT g = gcd(den, o.den);
    // return num * (o.den / g) <=> (den / g) * o.num;
    return num * (o.den / g) <=> (den / g) * o.num;
  }

  static Rational _add(const Rational& a, const Rational& b) {
    Rational ret;
    INT g = gcd(a.den, b.den);
    INT aa = a.den / g;
    INT bb = b.den / g;
    ret.num = a.num * bb + aa * b.num;
    ret.den = aa * b.den;
    ret.reduction();
    return ret;
  }
  static Rational _mult(const Rational& a, const Rational& b) {
    Rational ret;
    INT g1 = gcd(a.num, b.den);
    INT g2 = gcd(a.den, b.num);
    ret.num = (a.num / g1) * (b.num / g2);
    ret.den = (a.den / g2) * (b.den / g1);
    // need not call ret.reduction()
    return ret;
  }

  Rational operator+(const Rational& o) const { return _add(*this, o); }
  Rational operator-() const { return Rational(-num, den); }
  Rational operator-(const Rational& o) const { return *this + (-o); }
  Rational operator*(const Rational& o) const { return _mult(*this, o); }
  Rational inverse() const { return Rational(den, num); }
  Rational operator/(const Rational& o) const { return *this * o.inverse(); }
    
  Rational& operator+=(const Rational& o) { return *this = *this + o; }
  Rational& operator-=(const Rational& o) { return *this = *this - o; }
  Rational& operator*=(const Rational& o) { return *this = *this * o; }
  Rational& operator/=(const Rational& o) { return *this = *this / o; }

  friend Rational operator+(INT a, const Rational& b) { return Rational(a) + b; }
  friend Rational operator-(INT a, const Rational& b) { return Rational(a) - b; }
  friend Rational operator*(INT a, const Rational& b) { return Rational(a) * b; }
  friend Rational operator/(INT a, const Rational& b) { return Rational(a) / b; }

  // for debug
  string show() const { return g_show(num) + "/" + g_show(den); }

};



// @@ !! END ---- rational.cc
