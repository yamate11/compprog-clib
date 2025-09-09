#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;

/*
  Cartesian Product

  Usage:
    vector<ll> vec{80, 90, 20, 50, 50, 40}
    CartesianProduct cp(vec);
    // CartesianProduct cp(vec, less<ll>()); ... same
    // CartesianProduct cp(vec, [](ll a, ll b) { return a < b; }); ... same
    // CartesianProduct<ll> cp; cp.build(vec);  ... same
    assert(cp.root == 2);     // vec[2] is the least element
    assert(cp.left[2] == 0);  // least in vec[0:2] is vec[0]
    assert(cp.left[0] == -1); // "no such element" is expressed by -1
    assert(cp.right[2] == 5);
    assert(cp.left[5] == 3 or cp.left[5] == 4);  // either of tied element can be chosen

 */

//////////////////////////////////////////////////////////////////////
// See help of libins command for dependency spec syntax.
// @@ !! BEGIN() ---- cartesianProd.cc

template<typename T>
struct CartesianProduct {
  int root;
  vector<int> left;
  vector<int> right;

  CartesianProduct() : root(-1), left(), right() {}
  template<typename Comp = less<typename T::value_type>>
  CartesianProduct(const T& vec, Comp comp = Comp()) { build(vec, comp); }

  template<typename Comp = less<typename T::value_type>>
  void build(const T& vec, Comp comp = Comp()) {
    int n = ssize(vec);
    root = -1;
    left = vector<int>(n, -1);
    right = vector<int>(n, -1);
    vector<int> stack{-1};
    for (int i = 0; i < n; i++) {
      int left_cand = -1;
      while (true) {
        if (int j = stack.back(); j == -1 or comp(vec[j], vec[i])) {
          left[i] = left_cand;
          if (j == -1) root = i;
          else         right[j] = i;
          stack.push_back(i);
          break;
        }else {
          left_cand = j;
          stack.pop_back();
        }
      }
    }
  }
};

// @@ !! END ---- cartesianProd.cc

