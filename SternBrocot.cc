#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;

/*
  Stern Brocot Tree
  https://yamate11.github.io/blog/posts/2025/06-08-stern-brocot/
 */

//////////////////////////////////////////////////////////////////////
// See help of libins command for dependency spec syntax.
// @@ !! BEGIN()    ---- SternBrocot.cc

struct SternBrocotNode {
  vector<ll> coeff;
  string show() const { // for DEBUG
    string ret;
    ret = "[";
    for (ll i = 0; i < ssize(coeff) - 1; i++) ret += to_string(coeff[i]) + ", ";
    if (ssize(coeff) > 0) ret += to_string(coeff.back());
    ret += "]  ";
    auto [numer, denom] = numden();
    ret += "(" + to_string(numer) + ", " + to_string(denom) + ")";
    return ret;
  }
  SternBrocotNode() : coeff{{1}} {}
  SternBrocotNode(const vector<ll> coeff_) { set(move(coeff_)); }
  SternBrocotNode(ll a, ll b) { set(a, b); }
  SternBrocotNode& set(const vector<ll> coeff_) {
    coeff = move(coeff_);
    return *this;
  }
  SternBrocotNode& set(ll a, ll b) {
    coeff = vector<ll>();
    if (a == b) {
      coeff = vector<ll>{{1}};
      return *this;
    }
    ll r, s;
    if (a > b) {
      r = b;
      s = a;
    }else {
      r = a;
      s = b;
      coeff.push_back(0);
    }
    while (true) {
      ll p = s / r;
      ll q = s % r;
      coeff.push_back(p);
      if (q == 0) return *this;
      s = r;
      r = q;
    }
  }
  bool operator==(const SternBrocotNode& o) const { return coeff == o.coeff; }
  strong_ordering operator<=>(const SternBrocotNode& o) const {
    // DLOGKL("operator", this->show(), o.show());
    const ll sz = ssize(coeff);
    const ll szo = ssize(o.coeff);
    for (ll i = 0; i < min(sz, szo); i++) {
      if ((coeff[i] != o.coeff[i])) {
        if ((coeff[i] < o.coeff[i]) == (i % 2 == 0)) {
          // DLOG("operator return less A");
          return strong_ordering::less;
        }
        else {
          // DLOG("operator return greater A");
          return strong_ordering::greater;
        }
      }
    }
    if (sz == szo) {
      // DLOG("operator return equal B");
      return strong_ordering::equal;
    }
    else if (((sz < szo) and (sz % 2 != 0)) or ((sz > szo) and (szo % 2 == 0))) {
      // DLOG("operator return less B");
      return strong_ordering::less;
    }
    else {
      // DLOG("operator return greater B");
      return strong_ordering::greater;
    }
  }
  pair<ll, ll> numden() const {
    ll numer = coeff.back();
    ll denom = 1;
    for (int i = ssize(coeff) - 2; i >= 0; i--) {
      swap(numer, denom);
      numer += coeff[i] * denom;
    }
    return {numer, denom};
  }
  bool is_root() const { return ssize(coeff) == 1 and coeff[0] == 1; }
  ll depth() const { return accumulate(coeff.begin(), coeff.end(), 0LL) - 1; }
  SternBrocotNode lca(const SternBrocotNode& o) const {
    int i = 0;
    for (; i < ssize(coeff) and i < ssize(o.coeff) and coeff[i] == o.coeff[i]; i++);
    if (i == ssize(coeff)) return *this;
    if (i == ssize(o.coeff)) return o;
    const auto& p = coeff[i] < o.coeff[i] ? *this : o;
    if (ssize(p.coeff) == i + 1) return p;
    vector<ll> vec = p.coeff;
    vec.resize(i + 1);
    vec[i] += 1;
    return SternBrocotNode(move(vec));
  }
  static SternBrocotNode lca(const SternBrocotNode& n1, const SternBrocotNode& n2) { return n1.lca(n2); }
  SternBrocotNode& _change_to_parent(bool big_step = false) {
    // DLOGKL("_change_to_parent", big_step, show());
    if (ssize(coeff) == 1) {
      if (coeff[0] == 1) throw runtime_error("_change_to_parent(): root");
      coeff[0] = big_step ? 1 : coeff[0] - 1;
    }else if (big_step or coeff.back() == 2) {
      coeff.pop_back();
      coeff.back()++;
    }else {
      coeff.back()--;
    }
    // DLOGKL("_change_to_parent return", show());
    return *this;
  }
  SternBrocotNode parent() const { return SternBrocotNode(*this)._change_to_parent(); }
  SternBrocotNode big_step_parent() const { return SternBrocotNode(*this)._change_to_parent(true); }
  SternBrocotNode& _change_to_child(bool is_hi) {
    if ((ssize(coeff) % 2 == 0) == is_hi) {
      coeff.back()--;
      coeff.push_back(2);
    }else {
      coeff.back()++;
    }
    return *this;
  }
  SternBrocotNode child_lo() const { return SternBrocotNode(*this)._change_to_child(false); }
  SternBrocotNode child_hi() const { return SternBrocotNode(*this)._change_to_child(true); }
  SternBrocotNode ancestor(ll dep) const {
    vector<ll> vec;
    ll s = 0;
    for (ll i = 0; i < ssize(coeff); i++) {
      ll rem = (dep + 1) - s;
      if (coeff[i] >= rem) {
        if (rem >= 2)         vec.push_back(rem);
        else if (vec.empty()) vec.push_back(1);
        else                  vec.back()++;
        return SternBrocotNode(move(vec));
      }
      vec.push_back(coeff[i]);
      s += coeff[i];
    }
    throw runtime_error("ancestor(): too deep");
  }
  using osbn = optional<SternBrocotNode>;
  // pair<optional<SternBrocotNode>, optional<SternBrocotNode>> range() {
  pair<osbn, osbn> range() const {
    if (is_root()) return {nullopt, nullopt};
    auto p = parent();
    auto q = big_step_parent();
    osbn q1 = q.is_root() ? osbn() : osbn(move(q._change_to_parent()));
    if (ssize(coeff) % 2 == 0) return {move(q1), move(p)};
    else                       return {move(p), move(q1)};
  }
};


// @@ !! END ---- SternBrocot.cc

