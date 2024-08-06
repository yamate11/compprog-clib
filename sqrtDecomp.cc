#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;

/*
  Square Root Decomposition

  see https://yamate11.github.io/blog/posts/2024/????

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
    auto [hi_p, hi_r] = idx2pos(hi - 1);
    if (lo_p == hi_p) return {lo_p, lo_r, hi_r + 1, hi_p, 0, 0};
    else              return {lo_p, lo_r, bsize,    hi_p, 0, hi_r + 1};
  }
  B& data(int i) { return _data[i]; }
  int block_size(int i) {
    if (i < numb - 1) return bsize;
    else return tot_size - (numb - 1) * bsize;
  }
  void exec_general(int lo, int hi, auto f_edge, auto f_body) {
    auto [b_f, lo_f, hi_f, b_l, lo_l, hi_l] = range_pos(lo, hi);
    f_edge(b_f, lo_f, hi_f);
    if (b_f + 1 < b_l) f_body(b_f + 1, b_l);
    if (b_f < b_l) f_edge(b_l, lo_l, hi_l);
  }
  void exec(int lo, int hi, auto inloop_edge, auto inloop_body) {
    auto f_edge = [&](int b, int lo_b, int hi_b) {
      for (int i = lo_b; i < hi_b; i++) inloop_edge(b, data(b), i, pos2idx(b, i));
    };
    auto f_body = [&](int b0, int b1) {
      for (int b = b0; b < b1; b++) inloop_body(b, data(b));
    };
    exec_general(lo, hi, f_edge, f_body);
  }
};


// @@ !! END ---- sqrtDecomp.cc


