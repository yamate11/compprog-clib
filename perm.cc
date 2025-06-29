#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;

/*
  Permutations

  Structs:
    IntPerm, IntComb, IntDupPerm, IntDupComb, IntDirProd, IntPartition

  Usage:

    IntPerm ip(3, 2);      // Sequences of length 2 from [0, 3), no duplication
    IntComb ic(3, 2);      // Sorted sequences of length 2 from [0, 3), no duplication
    IntDupPerm dip(3, 2);  // Sequences of length 2 from [0, 3)
    IntDupComb dic(3, 2);  // Sorted sequences of length 2 from [0, 3)
    IntDirProd did{vector{{1, 2, 3}}};  // Sequences (a, b, c) s.t. a < 1, b < 2 and c < 3
    IntPartition part(5);  // Partitions of 5

    while (ip.get()) {  
      for (int i = 0; i < 2; i++) cout << ip[i] << " ";    // ip[i] or ip.at(i).  same results
      // or, equivalently,
      const vector<int>& v = ip.vec_view();
      for (int i = 0; i < 2; i++) cout << v[i] << " ";
    }

    // Results:
    //    IntPerm    ...       [0,1],[0,2],[1,0],      [1,2],[2,0],[2,1]
    //    IntComb    ...       [0,1],[0,2],            [1,2]
    //    IntDupPerm ... [0,0],[0,1],[0,2],[1,0],[1,1],[1,2],[2,0],[2,1],[2,2]
    //    IntDupComb ... [0,0],[0,1],[0,2],      [1,1],[1,2],            [2,2]
    //
    //    IntDirProd ... [0,0,0],[0,0,1],[0,0,2],[0,1,0],[0,1,1],[0,1,2]
    //    IntPartition ... [[1,1,1,1,1],[1,1,1,2],[1,1,3],[1,2,2],[1,4],[2,3],[5]]

    // Note:
    //    Unless 0 <= n and 0 <= r, they returns {}.
    //    IntPerm and IntComb returns {} unless r <= n.
    //    When n = 0 and r = 0, IntDupPerm and IntDupComb returns { [] } instead of {},
    //       which may not be appropriate (since they mean 0^0 = 1 and H(n, r) != C(n+r-1, r).)
    //    When n = 0 and r != 0, all returns {}.

    // When ip.get() returns false, it's internal has gone back to the
    // init state, so you can continue the next round.

   Results other than [0, 1, .., n -1]:

     IntComb ip(4, 2, [100, 50, 90, 10]);  // [[100, 50], [100, 90], [100, 10], [50, 90], [50, 10], [90, 10]]

     IntPerm<char> ip(3, 2, vector<char>{'a', 'b', 'c'});
     // or, equivalently
     IntPerm<char> ip(3, 2);
     ip.set_mapping([](int i) -> "abc"[i]);

   For Debugging:
      const vector<int>& vec_view() const;

   About partition:
      table of partition numbers up to 10000: https://oeis.org/A000041/b000041.txt
      // 0  1  2  3  4  5   6   7   8   9  10  11  12   13   14   15   16   17   18   19     //  i
      [  1, 1, 2, 3, 5, 7, 11, 15, 22, 30, 42, 56, 77, 101, 135, 176, 231, 297, 385, 490]    // p(i)
      p(20) = 627
      p(25) = 1958
      p(30) = 5604
      p(40) = 37338 (3.7E4)
      p(50) = 204226 (2.0E5)
      p(61) = 1121505 (1.1E6)
      p(77) = 10619863 (1.1E7)

*/

// @@ !! LIM()

//////////////////////////////////////////////////////////////////////
// See help of libins command for dependency spec syntax.
// @@ !! BEGIN() ---- perm.cc

template <bool dup, typename T>
struct IntPermBase {
  int n;
  int r;
  vector<int> vec;
  bool started;
  vector<T> mapping;

  bool start_check() {
    if constexpr (dup) { if (not ((1 <= n and 0 <= r) or (n == 0 and r == 0))) return false; }
    else { if (not (0 <= n and 0 <= r and r <= n)) return false; }
    started = true;
    vec.resize(r, 0);
    return true;
  }

  bool finish() {
    vec.resize(0);
    started = false;
    return false;
  }

  IntPermBase(int n_, int r_) : n(n_), r(r_), started(false) {
    if (n >= 0) {
      mapping = vector<T>(n);
      for (int i = 0; i < n; i++) {
        if constexpr (is_integral<T>::value) mapping[i] = (T)i;
        else mapping[i] = T();
      }
    }
  }
  IntPermBase(int n_, int r_, vector<T> mp) : n(n_), r(r_), started(false), mapping(move(mp)) {
    if (ssize(mapping) != n) throw runtime_error("IntPermBase: incorrect mapping length");
  }

  T at(int i) const { return mapping[vec[i]]; }

  T operator[](int i) const { return at(i); }

  void set_mapping(auto f) {
    for (int i = 0; i < n; i++) mapping[i] = f(i);
  }

  vector<T> vec_view() const {
    vector<T> res;
    for (int i = 0; i < r; i++) res.push_back(mapping[vec[i]]);
    return res;
  }
};

template<typename T = int>
struct IntPerm : IntPermBase<false, T> {
  using Base = IntPermBase<false, T>;
  using Base::vec, Base::r, Base::n, Base::started;

  vector<vector<int>> cands;
  vector<int> cidx;

  bool start_check() {
    if (not IntPermBase<false, T>::start_check()) return false;
    iota(vec.begin(), vec.end(), 0);
    cands.resize(r);
    cidx.resize(r);
    for (int i = 0; i < r; i++) {
      for (int j = n - 1; j >= i; j--) cands[i].push_back(j);
      cidx[i] = n - i - 1;
    }
    return true;
  }

  bool finish() {
    cands.resize(0);
    cidx.resize(0);
    return IntPermBase<false, T>::finish();
  }

  IntPerm(int n_, int r_) : IntPermBase<false, T>(n_, r_) {}
  IntPerm(int n_, int r_, vector<T> mp) : IntPermBase<false, T>(n_, r_, mp) {}

  bool get() {
    if (not started) return start_check();
    int i = r - 1;
    for (; i >= 0 and cidx[i] == 0; i--);
    if (i < 0) return finish();
    vec[i] = cands[i][--cidx[i]];
    for (int j = i + 1; j < r; j++) {
      if (j == i + 1) {
        cands[j].resize(0);
        for (int k = 0; k < (int)cands[i].size(); k++) {
          if (k == cidx[i]) continue;
          cands[j].push_back(cands[i][k]);
        }
      }else {
        cands[j] = cands[j - 1];
        cands[j].pop_back();
      }
      cidx[j] = n - j - 1;
      vec[j] = cands[j][cidx[j]];
    }
    return true;
  }
};

template<typename T = int>
struct IntComb : IntPermBase<false, T> {
  using Base = IntPermBase<false, T>;
  using Base::vec, Base::r, Base::n, Base::started, Base::finish;

  bool start_check() {
    if (not IntPermBase<false, T>::start_check()) return false;
    iota(vec.begin(), vec.end(), 0);
    return true;
  }

  IntComb(int n_, int r_) : IntPermBase<false, T>(n_, r_) {}
  IntComb(int n_, int r_, vector<T> mp) : IntPermBase<false, T>(n_, r_, mp) {}

  bool get() {
    if (not started) return start_check();
    int i = r - 1;
    for (; i >= 0 and vec[i] == n - r + i; i--);
    if (i < 0) return finish();
    vec[i]++;
    for (int j = i + 1; j < r; j++) vec[j] = vec[j - 1] + 1;
    return true;
  }
};

template<typename T = int>
struct IntDupPerm : IntPermBase<true, T> {
  using Base = IntPermBase<true, T>;
  using Base::vec, Base::r, Base::n, Base::started, Base::finish, Base::start_check;

  IntDupPerm(int n_, int r_) : IntPermBase<true, T>(n_, r_) {}
  IntDupPerm(int n_, int r_, vector<T> mp) : IntPermBase<true, T>(n_, r_, mp) {}

  bool get() {
    if (not started) return start_check();
    for (int i = r - 1; i >= 0; vec[i--] = 0) if (++vec[i] < n) return true;
    return finish();
  }
};

template<typename T = int>
struct IntDupComb : IntPermBase<true, T> {
  using Base = IntPermBase<true, T>;
  using Base::vec, Base::r, Base::n, Base::started, Base::finish, Base::start_check;

  IntDupComb(int n_, int r_) : IntPermBase<true, T>(n_, r_) {}
  IntDupComb(int n_, int r_, vector<T> mp) : IntPermBase<true, T>(n_, r_, mp) {}

  bool get() {
    if (not started) return start_check();
    int i = r - 1;
    for (; i >= 0 and vec[i] == n - 1; i--);
    if (i < 0) return finish();
    vec[i]++;
    for (int j = i + 1; j < r; j++) vec[j] = vec[i];
    return true;
  }
};

template<typename INT>
struct IntDirProd {
  vector<INT> lim;
  int r;
  vector<INT> vec;
  bool started;

  IntDirProd(const vector<INT>& lim_) : lim(lim_), r(lim.size()), started(false) {}

  int at(int i) const { return vec[i]; }

  const vector<INT>& vec_view() const { return vec; }

  bool start_check() {
    for (int i = 0; i < r; i++) if (lim[i] == 0) return false;
    started = true;
    vec.resize(r, 0);
    return true;
  }

  bool finish() {
    vec.resize(0);
    started = false;
    return false;
  }

  bool get() {
    if (not started) return start_check();
    for (int i = r - 1; i >= 0; vec[i--] = 0) if (++vec[i] < lim[i]) return true;
    return finish();
  }
};

template<typename INT>
struct IntPartition {
  INT n;
  vector<INT> vec;
  bool started = false;
  IntPartition(INT n_) : n(n_) {}
  
  bool get() {
    if (not started) {
      started = true;
      vec = vector<INT>(n, 1);
      return true;
    }else if (ssize(vec) == 1) {
      started = false;
      return false;
    }else {
      ll b = vec.back(); vec.pop_back();
      ll a = vec.back(); vec.pop_back();
      ll c = a + b;
      ll a1 = a + 1;
      while (c - a1 >= a1) {
        vec.push_back(a1);
        c -= a1;
      }
      vec.push_back(c);
      return true;
    }
  }

  INT at(int i) const { return vec[i]; }
  const vector<INT>& vec_view() const { return vec; }
};

// @@ !! END() ---- perm.cc
