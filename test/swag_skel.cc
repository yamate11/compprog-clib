#include <bits/stdc++.h>
#include <cassert>
using namespace std;
using ll = long long int;
using pll = pair<ll, ll>;
using ull = unsigned long long;

// @@ !! LIM(debug random matrix swag)

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << setprecision(20);
  Random rand;

  {
    auto sw = make_swag(plus<string>(), string{});
    sw.push("ab");
    sw.push("cde");
    sw.push("f");
    assert(sw.front() == "ab" and sw.back() == "f");
    assert(sw.get_val() == "abcdef");
    sw.pop();
    assert(sw.front() == "cde" and sw.back() == "f");
    assert(sw.get_val() == "cdef");
    sw.pop();
    assert(sw.front() == "f" and sw.back() == "f");
    assert(sw.get_val() == "f");
    sw.push("x");
    sw.push("y");
    sw.push("z");
    assert(sw.front() == "f" and sw.back() == "z");
    assert(sw.get_val() == "fxyz");
    sw.pop();
    sw.pop();
    assert(sw.front() == "y" and sw.back() == "z");
    assert(sw.get_val() == "yz");
    sw.push("AB");
    sw.push("CD");
    assert((sw.vec_view() == vector<string>{"y", "z", "AB", "CD"}));
  }

  {
    using MyMat = Matrix<ll>;
    MyMat unit{{1, 0}, {0, 1}};

#if DEBUG
    ll rep1 = 100, rep2 = 100;
#else
    ll rep1 = 500, rep2 = 500;
#endif
    for (ll i1 = 0; i1 < rep1; i1++) {
      auto sw = make_swag(multiplies<MyMat>(), unit);
      deque<MyMat> deq;
      auto getMyMat = [&]() -> MyMat { 
        return MyMat{{rand.range(-4, 5), rand.range(-4, 5)}, {rand.range(-4, 5), rand.range(-4, 5)}};
      };
      auto insert = [&]() {
        auto m = getMyMat();
        deq.push_back(m);
        sw.push(m);
      };
      auto remove = [&]() {
        deq.pop_front();
        sw.pop();
      };
      auto check = [&]() {
        auto v = unit;
        for (const auto& m : deq) v *= m;
        assert(v == sw.get_val());
        if (ssize(deq) >= 1) {
          assert(deq.front() == sw.front());
          assert(deq.back() == sw.back());
        }
      };
      for (ll i2 = 0; i2 < rep2; i2++) {
        if (ssize(deq) == 0) insert();
        else {
          if (rand.range(0, 2) == 0) insert();
          else remove();
          check();
        }
      }
    }
  }

  cerr << "ok\n";

  return 0;
}
