#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;

/*
  Cartesian Tree
  https://yamate11.github.io/blog/posts/2025/09-09-cartesian-tree/
 */

//////////////////////////////////////////////////////////////////////
// See help of libins command for dependency spec syntax.
// @@ !! BEGIN() ---- cartesianTree.cc

template<typename T>
struct CartesianTree {
  int root;
  vector<int> left;
  vector<int> right;

  CartesianTree() : root(-1), left(), right() {}
  template<typename Comp = less<typename T::value_type>>
  CartesianTree(const T& vec, Comp comp = Comp()) { build(vec, comp); }

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

// @@ !! END ---- cartesianTree.cc

