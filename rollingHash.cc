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
// @@ !! BEGIN(random) ---- rollingHash.cc

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

template<typename elem_t = char, typename conv_t = nullptr_t>
struct RollingHashGen {
  
  vector<u64> pow_memo;
  u64 base;
  conv_t conv;

  void _initialize(u64 base_, u64 min_base_) {
    pow_memo = vector<u64>({1});
    if (base_ == 0) base = Random().range(min_base_, 1ULL << 59);
    else            base = base_;
    if (base >= rh_prime) { throw runtime_error("RollingHashGen::_initialize: invalid base"); }
  }

  RollingHashGen(u64 base_ = 0, u64 min_base_ = 1000) { _initialize(base_, min_base_); }
  RollingHashGen(u64 base_, u64 min_base_, conv_t conv_) : conv(conv_) { _initialize(base_, min_base_); }

  void set_conv(conv_t conv_) { conv = conv_; }

  vector<u64> hashes(const auto& s) {
    int n = s.size();
    vector<u64> ret(n+1);
    for (int i = 0; i < n; i++) {
      u64 e;
      if constexpr (is_same<conv_t, nullptr_t>::value) {
        e = static_cast<u64>(s[i]);
      } else {
        e = conv(s[i]);
      }
      ret[i+1] = rh_calc_mod(rh_mul_nomod(ret[i], base) + e);
    }
    return ret;
  }

  vector<u64> hashes(const char* p) { return hashes(string(p)); }

  u64 base_power(ll n) {
    while ((int)pow_memo.size() < n + 1) {
      pow_memo.push_back(rh_mul(pow_memo[pow_memo.size() - 1], base));
    }
    return pow_memo[n];
  }

  u64 get(const vector<u64>& hashes, int start = 0, int len = -1) {
    if (len == -1) len = hashes.size() - 1 - start;
    if (start == 0) return hashes[len];
    return rh_calc_mod(hashes[start + len] + 
                       rh_positivizer - rh_mul_nomod(hashes[start], base_power(len)));
  }

  u64 hashvalue(const auto& s) { return hashes(s).back(); }

  u64 hash_concat(u64 hash1, u64 hash2, int len2) {
    return rh_add(rh_mul(hash1, base_power(len2)), hash2);
  }

};

using RollingHash = RollingHashGen<char, nullptr_t>;

template<typename T>
auto make_rolling_hash_gen(ll base, ll min_base, auto conv) {
  return RollingHashGen<T, decltype(conv)>(base, min_base, conv);
}

// @@ !! END ---- rollingHash.cc


