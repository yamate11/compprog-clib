#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;

/*
  Rolling Hash
    https://yamate11.github.io/blog/posts/2023/12-09-rolling-hash/
 */

// @@ !! LIM()


//////////////////////////////////////////////////////////////////////
// See help of libins command for dependency spec syntax.
// @@ !! BEGIN(random unordered_map) ---- rollingHash.cc

// By keymoon
//     https://qiita.com/keymoon/items/11fac5627672a6d6a9f6

using u64 = unsigned long long;

constexpr u64 rh_prime = (1ULL << 61) - 1;
constexpr u64 rh_mask30 = (1ULL << 30) - 1;
constexpr u64 rh_mask31 = (1ULL << 31) - 1;
constexpr u64 rh_mask61 = (1ULL << 61) - 1;
constexpr u64 rh_positivizer = rh_prime * 4;

u64 rh_mul_nomod(u64 a, u64 b) {
  u64 au = a >> 31;
  u64 ad = a & rh_mask31;
  u64 bu = b >> 31;
  u64 bd = b & rh_mask31;
  u64 c = ad * bu + au * bd;
  u64 cu = c >> 30;
  u64 cd = c & rh_mask30;
  return au * bu * 2 + cu + (cd << 31) + ad * bd;
}

u64 rh_calc_mod(u64 x) {
  u64 xu = x >> 61;
  u64 xd = x & rh_mask61;
  u64 res = xu + xd;
  if (res >= rh_prime) res -= rh_prime;
  return res;
}

u64 rh_add(u64 a, u64 b) {
  u64 x = a + b;
  return x >= rh_prime ? x - rh_prime : x;
}
u64 rh_subt(u64 a, u64 b) { return (a < b) ? rh_prime + a - b : a - b; }
u64 rh_mul(u64 a, u64 b) { return rh_calc_mod(rh_mul_nomod(a, b)); }

struct RHVal {
  u64 u64v;
  RHVal(u64 t = 0) : u64v(t) {
#if DEBUG
    if (t >= rh_prime) throw runtime_error("out of range for RHVal");
#endif
  }
  RHVal(const RHVal& o) : u64v(o.u64v) {}
  RHVal& operator =(const RHVal& o) { u64v = o.u64v;  return *this; }
  RHVal& operator =(u64 t) {
#if DEBUG
    if (t >= rh_prime) throw runtime_error("out of range for RHVal");
#endif
    u64v = t;
    return *this;
  }

  explicit operator u64() const { return u64v; }

  bool operator ==(const RHVal& o) const { return u64v == o.u64v; }
  bool operator !=(const RHVal& o) const { return not (*this == o); }

  RHVal& operator +=(const RHVal& o) { u64v = rh_add(u64v, o.u64v); return *this; }
  RHVal& operator -=(const RHVal& o) { u64v = rh_subt(u64v, o.u64v); return *this; }
  RHVal& operator *=(const RHVal& o) { u64v = rh_mul(u64v, o.u64v); return *this; }

  friend RHVal operator +(RHVal a, const RHVal& b) { return a += b; }
  friend RHVal operator -(RHVal a, const RHVal& b) { return a -= b; }
  friend RHVal operator *(RHVal a, const RHVal& b) { return a *= b; }
  friend RHVal operator -(const RHVal& b) { return RHVal(0ULL) - b; }

  friend ostream& operator<< (ostream& os, const RHVal& t) {
    os << t.u64v;
    return os;
  }
};

template<typename elem_t = char, typename conv_t = nullptr_t>
struct RollingHashGen {
  
  vector<RHVal> pow_memo;
  RHVal base;
  conv_t conv;

  void _initialize(RHVal base_, RHVal min_base_) {
    pow_memo = vector<RHVal>{1};
    if (base_ == 0) base = Random().range((u64)min_base_, 1ULL << 59);
    else            base = base_;
  }

  RollingHashGen(RHVal base_ = 0, RHVal min_base_ = 1000) { _initialize(base_, min_base_); }
  RollingHashGen(RHVal base_, RHVal min_base_, conv_t conv_) : conv(conv_) { _initialize(base_, min_base_); }

  void set_conv(conv_t conv_) { conv = conv_; }

  struct HashValues {
    RollingHashGen* rh;
    vector<RHVal> vs;

    HashValues() {}
    HashValues(RollingHashGen* rh_, const vector<RHVal>& s) : rh(rh_), vs(s) {}

    RHVal get(int start = 0, int len = -1) const {
      if (len == -1) len = vs.size() - 1 - start;
      if (start == 0) return vs[len];
      return rh_calc_mod((u64)vs[start + len] + rh_positivizer
                         - rh_mul_nomod((u64)vs[start], (u64)rh->base_power(len)));
    }
  };

  vector<RHVal> _hashes(const auto& s) const {
    vector<RHVal> vs(s.size() + 1, RHVal(0ULL));
    for (int i = 0; i < ssize(s); i++) {
      RHVal e;
      if constexpr (is_same<conv_t, nullptr_t>::value) {
        e = static_cast<RHVal>(s[i]);
      } else {
        e = conv(s[i]);
      }
      vs[i+1] = rh_calc_mod(rh_mul_nomod((u64)vs[i], (u64)base) + (u64)e);
    }
    return vs;
  }
  vector<RHVal> _hashes(const char* p) const { return _hashes(string(p)); }

  HashValues precomp(const auto& s) { return HashValues(this, _hashes(s)); }
  RHVal hashvalue(const auto& s) { return HashValues(this, _hashes(s)).vs.back(); }

  RHVal base_power(ll n) {
    while ((int)pow_memo.size() < n + 1) {
      pow_memo.push_back(pow_memo[pow_memo.size() - 1] * base);
    }
    return pow_memo[n];
  }

  RHVal hash_concat(RHVal hash1, RHVal hash2, int len2) {
    return hash1 * base_power(len2) + hash2;
  }

};


using RollingHash = RollingHashGen<char, nullptr_t>;

template<typename T>
auto make_rolling_hash_gen(ll base, ll min_base, auto conv) {
  return RollingHashGen<T, decltype(conv)>(base, min_base, conv);
}

template <>
struct safe_custom_hash<RHVal, void> {
  size_t operator()(const RHVal& x) const { return safe_custom_hash<u64>{}((u64)x); }
};


// @@ !! END ---- rollingHash.cc


