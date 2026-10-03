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

  ll tot_size;  // total number of elements
  ll bsize;  // block size
  ll numb;  // number of blocks
  vector<B> _data;

  SRD() : tot_size(0), bsize(0), numb(0), _data(0) {}
  SRD(ll tot_size_, ll bsize_ = 0) : tot_size(tot_size_) {
    if (bsize_ > 0) bsize = bsize_;
    else            bsize = lround(sqrt(tot_size));
    numb = (tot_size + bsize - 1) / bsize;
    _data = vector<B>(numb);
  }

  pair<ll, ll> idx2pos(ll x) { return {x / bsize, x % bsize}; }
  ll pos2idx(ll p, ll r) { return p * bsize + r; }

  tuple<ll, ll, ll, ll, ll, ll> range_pos(ll lo, ll hi) {
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

  B& data(ll i) { return _data[i]; }

  ll block_size(ll i) {
    if (i < numb - 1) return bsize;
    else return tot_size - (numb - 1) * bsize;
  }

  void exec(ll lo, ll hi, auto f_edge, auto f_core) {
    auto [block_start, block_end, pre_start, pre_end, post_start, post_end] = range_pos(lo, hi);
    if (pre_start >= 0) for (ll i = pre_start; i < pre_end; i++) f_edge(i, block_start - 1);
    for (ll b = block_start; b < block_end; b++) f_core(b);
    if (post_start >= 0) for (ll i = post_start; i < post_end; i++) f_edge(i, block_end);
  }

};


// @@ !! END ---- sqrtDecomp.cc


