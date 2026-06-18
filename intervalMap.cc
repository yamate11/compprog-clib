#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;

/*
  Interval Map

  The library manages functions f : [lo, hi) -> T; assuming that a same value tends to
  continue.  The implementation uses a map<ll, T>.  
  Map ((x0, t0), (x1, t1), ..., (xn, tn)), where x0 == lo and xn == hi, means that for x \in [x_i, x_{i+1})
  the value f(x) is t_i.
  It is guaranteed that t_i != t_{i + 1}  (for i + 1 < n)

  For managing an interval set, use its characteristic function.

  API:

  template<typename T>
  itv_map(ll lo, ll hi, T t = T())  ... constant function f(x) = t for x \in [lo, hi)
  
  void put(ll x, T t) ... f(x) := t
  void put(ll l, ll r, T t) ...  f(x) := t for x \in [l, r)
  T get_val(ll x) ... returns f(x)
  itv_map::Tuple3 get(ll x) ... returns (l, r, t) where f(x) = t and l <= x < r and l and r are defined points.
                                The return type Tuple3 is a struct { ll left; ll right; T val; }.
  T sum(ll l, ll r) ... returns \sum_{i = l}^{r - 1} f(i)．  Note that T must have "+" and "*", and "ll" should be 
                        convertible into T.
  auto itv_apply(auto f, const auto& x, const auto& y)
       // When x is itv_map<x_t>, y is itv_map<y_t> and f : x_t -> y_t -> res_t, 
       //     itv_apply returns an itv_map<res_t> r such that r(i) = f(x(i), y(i))

  Loop:
    itv_map is(...);
    for (const auto& [l, r, t] : is) { ... }

  Debugging:
    Use the impl member of type map<ll, T>.

 */

//////////////////////////////////////////////////////////////////////
// See help of libins command for dependency spec syntax.
// @@ !! BEGIN() ---- intervalSet.cc

template<typename T>
struct itv_map {
  using value_type = T;
  
  struct Tuple3 {
    ll left;
    ll right;
    T val;
    bool operator==(const Tuple3&) const = default;
  };

  struct Itr {
    using iterator_category = bidirectional_iterator_tag;
    using difference_type = ptrdiff_t;
    using value_type = Tuple3;
    using reference = value_type;
    using pointer = void;

    using impl_iterator = typename map<ll, T>::const_iterator;
    impl_iterator it_impl;
    Itr(impl_iterator it_impl_) : it_impl(it_impl_) {}

    bool operator ==(const Itr& o) const { return it_impl == o.it_impl; }
    bool operator !=(const Itr& o) const { return it_impl != o.it_impl; }
    value_type operator *() const { return value_type(it_impl->first, std::next(it_impl)->first, it_impl->second); }
    Itr& operator ++() { 
      ++it_impl;
      return *this;
    }
    Itr operator ++(int) { return Itr(it_impl++); }
    Itr& operator --() {
      --it_impl;
      return *this;
    }
    Itr operator --(int) { return Itr(it_impl--); }
    ll left() const { return it_impl->first; }
    ll right() const { return std::next(it_impl)->first; }
    const T& val() const { return it_impl->second; }
    Itr prev() const { return Itr(std::prev(it_impl)); }
    Itr next() const { return Itr(std::next(it_impl)); }
  };
  using iterator = Itr;
  using const_iterator = Itr;
  Itr begin() const { return Itr(impl.begin()); }
  Itr end() const { return Itr(prev(impl.end())); }

  map<ll, T> impl;  
  ll lo;
  ll hi;

  itv_map(ll lo_ = -(1LL << 60), ll hi_ = (1LL << 60), const T& t = T()) : lo(lo_), hi(hi_) {
    impl[lo] = t;
    impl[hi] = t;  // the value is just a dummy.
  }

  bool operator==(const itv_map& o) const { return lo == o.lo and hi == o.hi and impl == o.impl; }
  bool operator!=(const itv_map& o) const { return not (*this == o); }

  auto get_iter(ll x) const { return Itr(std::prev(impl.upper_bound(x))); }

  auto divide(ll x) {
    auto it_nxt = impl.upper_bound(x);
    auto it = std::prev(it_nxt);
    if (it->first == x) return it;
    return impl.emplace_hint(it_nxt, x, it->second);
  }
  
  void range_check(ll l, ll r) const {
    if (l < lo or r > hi) throw runtime_error("intervalSet: out of range: " + to_string(l) + ", " + to_string(r));
  }
  void range_check(ll x) const {
    if (x < lo or x >= hi) throw runtime_error("intervalSet: out of range: " + to_string(x));
  }

  void put(ll l, ll r, const T& t) {
    range_check(l, r);
    if (l >= r) return;
    auto it0 = divide(l);
    auto it1 = divide(r);
    it0->second = t;
    for (auto it = std::next(it0); it != it1; it = impl.erase(it));
    if (std::next(it1) != impl.end() and it0->second == it1->second) impl.erase(it1);
    if (it0 != impl.begin() and std::prev(it0)->second == it0->second) impl.erase(it0);
  }

  void put(ll x, const T& t) {
    range_check(x);
    put(x, x + 1, t);
  }

  T get_val(ll x) const {
    range_check(x);
    return get_iter(x).val();
  }

  Tuple3 get(ll x) {
    range_check(x);
    return *get_iter(x);
  }

  T sum(ll l0, ll r0) {
    range_check(l0, r0);
    T ret = T();
    ll i = l0;
    while (true) {
      auto [l, r, t] = get(i);
      ret += (min(r, r0) - i) * t;
      if (r0 <= r) return ret;
      i = r;
    }
  }

};

auto itv_apply(auto f, const auto& x, const auto& y) {
  using x_t = typename remove_reference_t<decltype(x)>::value_type;
  using y_t = typename remove_reference_t<decltype(x)>::value_type;
  using res_t = decltype(f(declval<x_t>(), declval<y_t>()));

  if (x.lo != y.lo or x.hi != y.hi) throw runtime_error("intervalSet: range mismatch");
  auto itx = x.impl.begin();
  auto ity = y.impl.begin();
  itv_map<res_t> ret(x.lo, x.hi, f(itx->second, ity->second));
  auto itcc = ret.impl.begin();
  auto itce = std::next(itcc);
  while (true) {
    ll t;
    tie(t, itx, ity) = [&]() -> tuple<ll, decltype(itx), decltype(ity)> {
      auto nitx = std::next(itx);
      auto nity = std::next(ity);
      if      (nitx->first <  nity->first) return {nitx->first, nitx,  ity};
      else if (nitx->first >  nity->first) return {nity->first,  itx, nity};
      else if (nitx->first < x.hi)         return {nitx->first, nitx, nity};
      else                                 return {-1,          nitx, nity};
    }();
    if (t == -1) break;
    res_t ncur = f(itx->second, ity->second);
    if (ncur != itcc->second) itcc = ret.impl.emplace_hint(itce, t, move(ncur));
  }
  return ret;
}

// @@ !! END ---- intervalSet.cc

