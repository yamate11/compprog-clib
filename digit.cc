/*
  Digit Util   (Kuraidori Kisuho)

  digit_util du;      // base == 10
  digit_util du1(3);  // base == 3
  digit_util du2(16); // base == 16
  
  du.pow(3)          // 1000
  du.pow(18)         // 1000000000000000000
  du.pow_size()      // 19, meaning du.pow(i) is valid for i from 0 to 18
  du.width(5678)     // 4, meaning 5678 has 4 digits
  du.width(0)        // 0.  so that "du.width(x) = n <=> du.pow(n - 1) <= x < du.pow(n)" holds.
  du.nd_min(3)       // 100, the least positive number whose width is 3.
  du.nd_min(1)       // 1.  the least POSITIVE numbr whose width is 1.
  su.nd_max(4)       // 9999, the maximum number whose width is 4.
  du.floor(1234)     // 1000, the largest power of 10 that does not exceed 1234
  du.ceil(1234)      // 10000, the smallest power of 10 that does not fall below 1234
  du.log(1234)       // 3,  du.pow(du.log(x)) <= x < du.pow(du.log(x) + 1)

  // Values at a position or a range of positions
  //   Positions: 0, 1, 2, ... : from right to left.
  //              -1, -2, .... : from left to right.
  //   Ranges: (x, t)  for t > 0:  a range from position x (incl) to x + t (not incl)
  du.d_at( 94283,  1) // 8
  du.d_at( 94283, -2) // 4
  du.d_sub(94283,  1,  3) // 428
  du.d_sub(94283,  1, -2) // 83
  du.d_sub(94283, -2,  2) // 94
  du.d_sub(94283, -2, -4) // 4283
  du.d_sub(94283,  1,  100) // 9428
  du.d_sub(94283,  1, -100) // 83

  // Warning: to_vector() and to_string() are handy, but they may be slow, especially in a loop.
  //   When performance is important, use d_at() and d_sub() instead where possible.
  du.to_vector(1234) // vector{4, 3, 2, 1}
  du.to_string(1234) // "1234"
  du.from_vector(vector{4, 3, 2, 1}) // 1234
  du.from_string("1234")             // 1234

  du2.to_string(255)       // "ff"
  du2.to_string(255, true) // "FF"

 */

//////////////////////////////////////////////////////////////////////
// See help of libins command for dependency spec syntax.
// @@ !! BEGIN() ---- digit.cc

struct digit_util {
  const ll base;
  const vector<ll> _pow;

  static vector<ll> _make_pow(ll b) {
    vector<ll> ret;
    ll t = 1;
    while (true) {
      ret.push_back(t);
      if (__builtin_smulll_overflow(t, b, &t)) break;
    }
    return ret;
  };

  digit_util(ll base_ = 10) : base(base_), _pow(_make_pow(base_)) {}
    
  ll pow_size() const { return _pow.size(); }
  ll pow(ll i) const {
    if (i < 0 or ssize(_pow) <= i) return -1;
    return _pow[i];
  }

  ll width(ll x) const {
    if (x < 0) return -1;
    if (base == 2) return bit_width((unsigned long long)x);
    if (x == 0) return 0;
    ll ret = 0;
    for (; x != 0; x /= base) ret++;
    return ret;
  }

  ll nd_min(ll i) const { return i < 0 ? -1 : i == 0 ? 0 : pow(i - 1); }
  ll nd_max(ll i) const { return i < 0 ? -1 : i == 0 ? 0 : nd_min(i + 1) - 1; }

  ll floor(ll x) const { return (x < 0) ? -1 : x == 0 ? 0 : _pow[width(x) - 1]; }

  ll ceil(ll x) const {
    if (x < 0) return -1;
    if (x == 0) return 0;
    ll p = _pow[width(x) - 1];
    return (x == p) ? p : (p * base);
  }

  ll log(ll x) const { return (x <= 0) ? -1 : width(x) - 1; }

  ll d_at(ll x, ll i) const {
    if (x < 0) return -1;
    if (x == 0) return 0;
    if (i < 0) i += width(x);
    return (x / pow(i)) % base;
  }

  ll d_sub(ll x, ll pos, ll len) const {
    if (x < 0) return -1;
    if (x == 0) return 0;
    ll w = width(x);
    if (pos < 0) pos += w;
    if (len < 0) { len = -len; pos = pos - len + 1; }
    if (pos < 0) { len += pos; pos = 0; }
    if (pos + len > w) len = w - pos;
    return (x % pow(pos + len)) / pow(pos);
  }

  vector<ll> to_vector(ll x) const {
    if (x < 0) return vector<ll>{};
    if (x == 0) return vector<ll>{0};
    vector<ll> ret;
    ret.reserve(width(x));
    for ( ; x != 0; x /= base) { ret.push_back(x % base); }
    return ret;
  }

  string to_string(ll x, bool upcase = false) const {
    if (x < 0) return string();
    if (x == 0) return string("0");
    char ten = upcase ? 'A' : 'a';
    ll w = width(x);
    string ret(w, ' ');
    for (ll i = w - 1; x != 0; x /= base, i--) {
      ll y = x % base;
      ret[i] = y < 10 ? '0' + y : ten + (y - 10);
    }
    return ret;
  }

  ll from_vector(const vector<ll>& vec) const {
    ll ret = 0;
    for (ll i = 0; i < ssize(vec); i++) ret += vec[i] * pow(i);
    return ret;
  }

  static ll _get_digit_char(char c) {
    if ('0' <= c and c <= '9')      return c - '0';
    else if ('a' <= c and c <= 'z') return c - 'a' + 10;
    else if ('A' <= c and c <= 'Z') return c - 'A' + 10;
    else throw runtime_error("_get_digit_char: unknown letter");
  }

  ll from_string(string s) const {
    ll ret = 0;
    for (ll i = 0; i < ssize(s); i++) ret += _get_digit_char(s[i]) * pow(ssize(s) - 1 - i);
    return ret;
  }


};

// @@ !! END ---- digit.cc
