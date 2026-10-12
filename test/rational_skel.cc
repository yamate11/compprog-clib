#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;

// @@ !! LIM(rational debug)

int main() {

  {
    using Rat = Rational<ll>;
    Rat a(1, 3);
    Rat b(2, 3);
    Rat c(1);
    Rat d;
    assert(a < b and a <= b and c > b and b >= b);
    assert(c != d);
    assert(a + b == c);
    assert(b - a == a);
    assert(d + 1 == c and 1 + d == c);
    assert(a * 2 == b and 2 * a == b);
    assert(c / 3 == a);
    assert(c == 1);
    assert(2 / b == 3);
    auto x = a;
    auto y = c;
    x += a;
    y -= a;
    assert(x == y);
    assert(g_show(-b) == "-2/3");
    // Note that operator<< is not supported.  Use g_show if needed.
  }
  {
    auto f = [&](auto x, ll n) -> decltype(x) {
      for (int i = 0; i < n; i++) x /= 2;
      for (int i = 0; i < n; i++) x *= 2;
      return x;
    };
    auto a = f(Rational<__int128>(1), 120);
    assert(a == 1);
  }
  {
    using Rat = Rational<ll>;
    for (int i = -2; i < 4; i += 2) {
      try {
        Rat a(i, 0);
        a += 0;
        assert(0);
      }catch(runtime_error& e) {
      }
    }
    Rat a(0);
    Rat b;
    try {
      b = 1 / a;
      assert(0);
    }catch(runtime_error& e) {
    }
  }
  {
    using Rat = Rational<ll>;
    const double err = 1e-9;
    auto near = [&](double x, double y) -> bool {
      return abs(x - y) < err or (abs(x) >= 1 and abs(1 - y/x) < err);
    };
    auto g = [&](const Rat& r, double x) -> void {
      assert(near((double)r.num, x * (double)r.den));
    };
    auto f = [&](ll op, ll a, ll b, ll c, ll d) -> void {
      if (b == 0 or d == 0) return;
      Rat r1(a, b);
      Rat r2(c, d);
      double x1 = (double)a / (double)b;
      double x2 = (double)c / (double)d;
      if      (op == 0) g(r1 + r2, x1 + x2);
      else if (op == 1) g(r1 - r2, x1 - x2);
      else if (op == 2) g(r1 * r2, x1 * x2);
      else if (op == 3 and c != 0) g(r1 / r2, x1 / x2);
      else if (op == 4) assert((r1 == r2) == near(x1, x2));
      else if (op == 5) assert((r1 != r2) == (not near(x1, x2)));
      else if (op == 6) assert((r1 < r2) == (r1 < r2 and not near(x1, x2)));
      else if (op == 7) assert((r1 <= r2) == (r1 < r2 or near(x1, x2)));
      else if (op == 8) assert((r1 > r2) == (r1 > r2 and not near(x1, x2)));
      else if (op == 9) assert((r1 >= r2) == (r1 > r2 or near(x1, x2)));
    };
    ll lim = 10;
    for (ll a = -lim; a <= lim; a++) {
      for (ll b = -lim; b <= lim; b++) {
        for (ll c = -lim; c <= lim; c++) {
          for (ll d = -lim; d <= lim; d++) {
            for (ll op = 0; op <= 9; op++) {
              f(op, a, b, c, d);
            }
          }
        }
      }
    }
  }









  cerr << "ok\n";

  return 0;

}
