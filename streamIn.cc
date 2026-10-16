#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;

/* 
   streamIn.cc

   operator>>(istream&, *) are defined.

*/

//////////////////////////////////////////////////////////////////////
// See help of libins command for dependency spec syntax.
// @@ !! BEGIN() ---- streamIn.cc

template <typename T1, typename T2>
istream& operator>> (istream& is, pair<T1,T2>& p) {
  is >> p.first >> p.second;
  return is;
}

template <typename T1, typename T2, typename T3>
istream& operator>> (istream& is, tuple<T1,T2,T3>& t) {
  is >> get<0>(t) >> get<1>(t) >> get<2>(t);
  return is;
}

template <typename T1, typename T2, typename T3, typename T4>
istream& operator>> (istream& is, tuple<T1,T2,T3,T4>& t) {
  is >> get<0>(t) >> get<1>(t) >> get<2>(t) >> get<3>(t);
  return is;
}

template <typename T>
istream& operator>> (istream& is, vector<T>& v) {
  for (size_t i = 0; i < v.size(); i++) { is >> v[i]; }
  return is;
}

template <typename T>
vector<T> read_vector_n(istream& is, size_t n) {
  vector<T> ret(n);
  for (size_t i = 0; i < n; i++) {
    is >> ret[i];
  }
  return ret;
}

template <typename T>
vector<T> read_vector(istream& is) {
  size_t n; is >> n;
  return read_vector_n<T>(is, n);
}

// signed and unsigned __int128.  This assumes correct input is given.

unsigned __int128 _uint128_from_string(const string& s, int start) {
  unsigned __int128 y = 0;
  for (int i = start; i < ssize(s); i++) y = y * 10 + (s[i] - '0');
  return y;
}

istream& operator>>(istream& is, unsigned __int128& x) {
  string s; is >> s;
  if (!is) return is;
  x = _uint128_from_string(s, s[0] == '+' ? 1 : 0);
  return is;
}

istream& operator>>(istream& is, __int128& x) {
  string s; is >> s;
  if (!is) return is;
  bool negative; int start;
  if      (s[0] == '-') { negative = true;  start = 1; }
  else if (s[0] == '+') { negative = false; start = 1; }
  else                  { negative = false; start = 0; }
  unsigned __int128 v = _uint128_from_string(s, start);
  x = negative ? -(__int128)(v - 1) - 1 : (__int128)v;
  return is;
}

// @@ !! END ---- streamIn.cc

