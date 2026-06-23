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

template<typename Ctn, typename Comp = less<typename Ctn::value_type>, bool comp_arg_index = false>
struct CartesianTree {
  int root;
  vector<int> left;
  vector<int> right;
  vector<int> parent;

  CartesianTree() : root(-1), left(), right(), parent() {}
  CartesianTree(const Ctn& vec, Comp comp = Comp()) { build(vec, comp); }

  void build(const Ctn& vec, Comp comp = Comp()) {
    auto comp_idx = [&](int a, int b) -> bool {
      if constexpr (comp_arg_index) return comp(a, b);
      else return comp(vec[a], vec[b]);
    };

    int n = ssize(vec);
    root = -1;
    left = vector<int>(n, -1);
    right = vector<int>(n, -1);
    parent = vector<int>(n, -1);
    vector<int> stack{-1};
    for (int i = 0; i < n; i++) {
      int left_cand = -1;
      while (true) {
        if (int j = stack.back(); j == -1 or comp_idx(j, i)) {
          left[i] = left_cand;
          if (left_cand >= 0) parent[left_cand] = i;

          if (j == -1) root = i;
          else {
            right[j] = i;
            parent[i] = j;
          }

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

template<typename Ctn, typename Comp = less<typename Ctn::value_type>>
auto make_cartesian_tree(const Ctn& vec, Comp comp = Comp()) {
  return CartesianTree<Ctn, Comp, false>(vec, comp);
}
template<typename Ctn, typename Comp>
auto make_cartesian_tree_comp_index(const Ctn& vec, Comp comp) {
  return CartesianTree<Ctn, Comp, true>(vec, comp);
}

// @@ !! END ---- cartesianTree.cc

