#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;

/*
  Square Root Decomposition

  see https://yamate11.github.io/blog/posts/2024/08-06-sqrt-decomp/

*/

//////////////////////////////////////////////////////////////////////
// See help of libins command for dependency spec syntax.
// @@ !! BEGIN()    ---- sqrtDecomp.cc

template<typename B>
struct SRD { // square root decomposition

  int tot_size;  // total number of elements
  int bsize;  // block size
  int numb;  // number of blocks
  vector<B> _data;

  SRD() : tot_size(0), bsize(0), numb(0), _data(0) {}
  SRD(int tot_size_, int bsize_ = 0) : tot_size(tot_size_) {
    if (bsize_ > 0) bsize = bsize_;
    else            bsize = lround(sqrt(tot_size));
    numb = (tot_size + bsize - 1) / bsize;
    _data = vector<B>(numb);
  }

  pair<int, int> idx2pos(int x) { return {x / bsize, x % bsize}; }
  int pos2idx(int p, int r) { return p * bsize + r; }

  tuple<int, int, int, int, int, int> range_pos(int lo, int hi) {
    auto [lo_p, lo_r] = idx2pos(lo);
    auto [hi_p, hi_r] = idx2pos(hi);
    ll block_start = lo_r == 0 ? lo_p : lo_p + 1;
    ll block_end = hi_p;
    ll pre_start = -1, pre_end = -1, post_start = -1, post_end = -1;
    if (lo_r > 0 and hi_r > 0 and lo_p == hi_p) {
      block_start = block_end;
      post_start = lo;
      post_end = hi;
    }else {
      if (lo_r > 0) {
        pre_start = lo;
        pre_end = pos2idx(lo_p + 1, 0);
      }
      if (hi_r > 0) {
        post_start = pos2idx(hi_p, 0);
        post_end = hi;
      }
    }
    return {block_start, block_end, pre_start, pre_end, post_start, post_end};
  }

  B& data(int i) { return _data[i]; }

  int block_size(int i) {
    if (i < numb - 1) return bsize;
    else return tot_size - (numb - 1) * bsize;
  }

  void exec(int lo, int hi, auto f_edge, auto f_core) {
    auto [block_start, block_end, pre_start, pre_end, post_start, post_end] = range_pos(lo, hi);
    if (pre_start >= 0) for (int i = pre_start; i < pre_end; i++) f_edge(i, block_start - 1);
    for (int b = block_start; b < block_end; b++) f_core(b);
    if (post_start >= 0) for (int i = post_start; i < post_end; i++) f_edge(i, block_end);
  }

};


// @@ !! END ---- sqrtDecomp.cc


