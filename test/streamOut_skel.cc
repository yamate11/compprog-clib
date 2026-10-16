#include <bits/stdc++.h>
#include <cassert>
using namespace std;
using ll = long long int;
using u64 = unsigned long long;
using pll = pair<ll, ll>;
#define REP(i, a, b) for (ll i = (a); i < (b); i++)
#define REPrev(i, a, b) for (ll i = (a); i >= (b); i--)
#define ALL(coll) (coll).begin(), (coll).end()

// @@ !! LIM(streamOut streamIn)

template<typename T>
void stream_out_check(const T& x, string s) {
  stringstream ss;
  ss << x;
  assert(ss.str() == s);
}

template<typename T>
void stream_in_check(const T& x, string s) {
  stringstream ss(s);
  T y;
  ss >> y;
  assert(x == y);
}

int main() {

  random_device rd;
  mt19937 rng(rd());

  {
    set<ll> s1({5, 2, 4});
    set<ll, greater<ll>> s2({5, 2, 4});
    string s = g_show(s1) + " " + g_show(s2);
    assert(s == "[2, 4, 5] [5, 4, 2]");
  }

  {
    enum FOO { A, B, C };
    stringstream ss;
    FOO a = A;
    FOO c = C;
    ss << c << a;
    assert(ss.str() == "20");
  }

  { // __int128 input output
    // __int128_t and __uint128_t are aliases for __int128 and unsigned __int128, resp.
    const __uint128_t zero = 0;
    const __uint128_t max_uint128 = ~zero;
    const __int128_t max_int128 = (__int128_t)(max_uint128 ^ ((__uint128_t)1 << 127));
    const __int128_t min_int128 = -max_int128 - 1;
    const string str_max_uint128 = "340282366920938463463374607431768211455";
    const string str_max_int128 =  "170141183460469231731687303715884105727";
    const string str_min_int128 = "-170141183460469231731687303715884105728";
  
    stringstream ss1;
    __int128_t x1 = 0;
    ss1 << x1 << 0;
    assert(ss1.str() == "00");

    stream_out_check<__int128_t>((__int128_t)0, "0");
    stream_out_check<__int128_t>((__int128_t)123, "123");
    stream_out_check<__int128_t>((__int128_t)(-2450), "-2450");
    stream_out_check<__int128_t>(min_int128, str_min_int128);
    stream_out_check<__int128_t>(max_int128, str_max_int128);
    stream_out_check<__uint128_t>((__uint128_t)0, "0");
    stream_out_check<__uint128_t>((__uint128_t)123, "123");
    stream_out_check<__uint128_t>(max_uint128, str_max_uint128);

    stringstream ss2("56 0 -18 2 143");
    __int128_t x10, x11, x12 ; ss2 >> x10 >> x11 >> x12;
    __uint128_t y10, y11; ss2 >> y10 >> y11;
    assert(x10 == 56 and x11 == 0 and x12 == -18 and y10 == 2 and y11 == 143);

    stream_in_check<__int128_t>(min_int128, str_min_int128);
    stream_in_check<__int128_t>(max_int128, str_max_int128);
    stream_in_check<__uint128_t>(max_uint128, str_max_uint128);
    stream_in_check<__uint128_t>((__uint128_t)max_int128, str_max_int128);

    __int128_t x20 = -5100;
    __uint128_t y20 = 365;
    assert(g_show(x20) == "-5100");
    assert(g_show(y20) == "365");
    assert(g_show(min_int128) == str_min_int128);
    assert(g_show(max_int128) == str_max_int128);
    assert(g_show(max_uint128) == str_max_uint128);
  }

  cerr << "ok\n";
  return 0;
}
