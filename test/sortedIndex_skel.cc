#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;

// @@ !! LIM(sortedIndex)

using pll = pair<ll, ll>;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << setprecision(20);

  {
    vector<ll> vec1{10, 5, 7, 1, 20, 3};
    auto ord2idx_a = sortedIndex(vec1);
    vector<ll> exp_a{3, 5, 1, 2, 0, 4};
    assert(ord2idx_a == exp_a);
    auto ord2idx_b = sortedIndex(vec1, greater<ll>());
    vector<ll> exp_b{4, 0, 2, 1, 5, 3};
    assert(ord2idx_b == exp_b);
    auto [ord2idx_c, idx2ord_c] = sortedIndex2(vec1);
    assert(ord2idx_c == exp_a);
    vector<ll> exp_cio{4, 2, 3, 0, 5, 1};
    assert(idx2ord_c == exp_cio);
    auto [ord2idx_d, idx2ord_d] = sortedIndex2(vec1, greater<ll>());
    assert(ord2idx_d == exp_b);
    vector<ll> exp_dio{1, 3, 2, 5, 0, 4};
    assert(idx2ord_d == exp_dio);
  }
  {
    deque<ll> vec1{10, 5, 7, 1, 20, 3};
    auto ord2idx_a = sortedIndex(vec1);
    vector<ll> exp_a{3, 5, 1, 2, 0, 4};
    assert(ord2idx_a == exp_a);
    auto ord2idx_b = sortedIndex(vec1, greater<ll>());
    vector<ll> exp_b{4, 0, 2, 1, 5, 3};
    assert(ord2idx_b == exp_b);
    auto [ord2idx_c, idx2ord_c] = sortedIndex2(vec1);
    assert(ord2idx_c == exp_a);
    vector<ll> exp_cio{4, 2, 3, 0, 5, 1};
    assert(idx2ord_c == exp_cio);
    auto [ord2idx_d, idx2ord_d] = sortedIndex2(vec1, greater<ll>());
    assert(ord2idx_d == exp_b);
    vector<ll> exp_dio{1, 3, 2, 5, 0, 4};
    assert(idx2ord_d == exp_dio);
  }

  {
    vector<ll> vec1{10, 5, 7, 1, 20, 3};
    auto ord2idx_a = sortedIndex<int>(vec1);
    vector<int> exp_a{3, 5, 1, 2, 0, 4};
    assert(ord2idx_a == exp_a);
  }

  {
    vector<pll> vec1({{7, 3}, {7, 2}, {3, 0}, {4, 4}, {7, 10}, {1, 5}, {3, 2}});
    auto compare = [&](pll x, pll y) -> bool {
      auto [x1, x2] = x;
      auto [y1, y2] = y;
      if (x1 != y1) return x1 < y1;
      else          return x2 > y2;
    };
    auto p = sortedIndex(vec1, compare);
    for (size_t i = 0; i < vec1.size() - 1; i++) {
      assert(not compare(vec1[p[i + 1]], vec1[p[i]]));
    }
  }
           

}
