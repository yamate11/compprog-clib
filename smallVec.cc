#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;

/*
  Vector of small integers
  https://yamate11.github.io/blog/posts/2025/08-17-smallvec/
*/

//////////////////////////////////////////////////////////////////////
// See help of libins command for dependency spec syntax.
// @@ !! BEGIN(unordered_map) ---- smallVec.cc

template<int bits>
struct small_vector_u64 {
  u64 impl;

  small_vector_u64(u64 impl_ = 0) : impl(impl_) {}
  small_vector_u64(const small_vector_u64& o) : impl(o.impl) {}
  small_vector_u64(initializer_list<int> init) : impl(0) {
    int i = 0;
    for (int x : init) set(i++, x);
  }
  small_vector_u64(const auto& v, int k) : impl(0) { for (int i = 0; i < k; i++) set(i, v[i]); }

  constexpr u64 mask() const { return (1ULL << bits) - 1; }

  struct Subst {
    small_vector_u64& x;
    int i;
    Subst(small_vector_u64& x_, int i_) : x(x_), i(i_) {}
    Subst& operator=(ll t) { x.set(i, t); return *this; }
    Subst& operator=(const Subst& o) { return (*this) = ll(o); }
    operator ll() const { return x.at(i); }
  };

  ll set(ll i, ll t) {
    impl &= ~(mask() << (i * bits));
    impl |= t << (i * bits);
    return t;
  }

  ll at(ll i) const { return (impl >> (i * bits)) & mask(); }
  Subst rs(ll i) { return Subst(*this, i); }
  ll operator[](ll i) const { return at(i); }
  Subst operator[](ll i) { return rs(i); }

  bool operator==(const small_vector_u64& o) const { return impl == o.impl; }
  operator u64() const { return impl; }

  string show(int len=-1) const {
    string ret = "[";
    if (len < 0) len = 64 / bits;
    for (ll i = 0; i < len; i++) {
      if (i > 0) ret += ", ";
      ret += to_string(at(i));
    }
    ret += ']';
    return ret;
  }

  friend ostream& operator<<(ostream& os, const small_vector_u64& vec) { return os << vec.show(); }
};

template <int bits>
struct safe_custom_hash<small_vector_u64<bits>, void> {
  size_t operator()(const small_vector_u64<bits>& x) const {
    return safe_custom_hash<u64>{}(u64(x));
  }
};

struct small_vector_string {
  string impl;
  small_vector_string() : impl() {}
  small_vector_string(ll sz, ll init = 0) : impl(sz, (char)init) {}
  small_vector_string(const string& impl_) : impl(impl_) {}
  small_vector_string(string&& impl_) : impl(move(impl_)) {}
  small_vector_string(initializer_list<int> init) : impl() { for (int x : init) impl += (char)x; }
  small_vector_string(const auto& v, int k) : impl() { for (int i = 0; i < k; i++) impl += (char)(v[i]); }

  // We employ Subst struct so that "cerr << vec[0];" produces "0" rather than "^@".
  struct Subst {
    small_vector_string& x;
    int i;
    Subst(small_vector_string& x_, int i_) : x(x_), i(i_) {}
    Subst& operator=(ll t) { x.impl[i] = t; return *this; }
    Subst& operator=(const Subst& o) { return (*this) = ll(o); }
    operator ll() const { return ll(x.impl[i]); }
  };

  ll operator[](ll i) const { return impl[i]; }
  Subst operator[](ll i) { return Subst(*this, i); }

  bool operator==(const small_vector_string& o) const { return impl == o.impl; }
  operator string() const { return impl; }

  string show() const {
    string ret = "[";
    for (ll i = 0; i < ssize(impl); i++) {
      if (i > 0) ret += ", ";
      ret += to_string(impl[i]);
    }
    ret += ']';
    return ret;
  }

  friend ostream& operator<<(ostream& os, const small_vector_string& vec) { return os << vec.show(); }
};

template <>
struct safe_custom_hash<small_vector_string, void> {
  size_t operator()(const small_vector_string& x) const {
    return safe_custom_hash<string>{}(string(x));
  }
};



// @@ !! END ---- smallVec.cc
