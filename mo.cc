/*
  Mo's algorithm
    Answers queries in the form of [l_i, r_i), where 0 <= l_i < r_i <= N, for i \in [0, Q).
    Complexity is O(N sqrt(Q)).

  Typical Usage:
    Mo mo(N);  // N is as above
    ...
    REP(i, 0, Q) mo.add_query(L[i], R[i]);   // half-open interval [L[i], R[i]) 

    // You need to define five (sometimes three) functions: in_left, in_right, out_left, out_right and calc.
    // Then, you execute mo.run(in_left, in_right, out_left, out_right, calc).
    // Sometimes, you can just use the same function for in_left and in_right, and for out_left and out_right.
    // In such cases, three argument version mo.run(in, out, calc) can be used.
    // Typically, you maintain some data in the first four functions, and set ans[q] in the function calc.

    vector<ll> ans(Q);
    auto in_left = [&](ll x) -> void { ...; };  
      // Write what you need to do when x steps into the range as the left boundary.
      // If you need the right boundary, use mo.cr.  (x == mo.cl)
      // I.e., the range changes from [x + 1, mo.cr) to [x, mo.cr)
    auto in_right = [&](ll x) -> void {...; };
      // "x" steps in from the right boundary.
      // The range changes from [mo.cl, x) to [mo.cl, x + 1)
    auto out_left = [&](ll x) -> void { ...; };
      // "x" steps out from the left boundary.
      // The range changes from [x, mo.cr) to [x + 1, mo.cr)
    auto out_right = [&](ll x) -> void { ...; };
      // "x" steps out from the right boundary.
      // The range changes from [mo.cl, x + 1) to [mo.cl, x)
    auto calc = [&](ll q) -> void { ans[q] = ....; };
      // Substitute the answer into ans[q].
    mo.run(in_left, in_right, out_left, out_right, calc);
    // or    mo.run(in_, out_, calc);   in case in_left == in_right and out_left == out_right
    REPOUT(q, 0, Q, ans[q], "\n");
 */

//////////////////////////////////////////////////////////////////////
// See help of libins command for dependency spec syntax.
// @@ !! BEGIN()    ---- mo.cc

struct Mo {
  using vq_t = tuple<int, int, int>;
  int size;
  int vq_idx;
  vector<vq_t> vq;
  int cl;
  int cr;

  Mo(int size_) : size(size_), vq_idx(0), vq(), cl(0), cr(0) {}
  void add_query(int l, int r) { vq.emplace_back(l, r, vq_idx++); }

  void run(auto in_left, auto in_right, auto out_left, auto out_right, auto calc) {
    int qsz = vq.size();
    int bsize = max(1, int((double)size / sqrt(qsz)));
    cl = 0;
    cr = 0;
    sort(vq.begin(), vq.end(),
         [&](const vq_t& p1, const vq_t& p2) -> bool {
           const auto& [a1, b1, i1] = p1;
           const auto& [a2, b2, i2] = p2;
           int z1 = a1 / bsize;
           int z2 = a2 / bsize;
           if (z1 != z2) return z1 < z2;
           else if (z1 % 2 == 0) return b1 < b2;
           else return b1 > b2;
         });
    for (const auto& [l, r, i] : vq) {
      // in_* must be called before out_*
      while (l < cl)   in_left(--cl);
      while (cr < r)  in_right(cr++);
      while (cl < l)  out_left(cl++);
      while (r < cr) out_right(--cr);
      calc(i);
    }
  }

  void run(auto f_in, auto f_out, auto calc) { return run(f_in, f_in, f_out, f_out, calc); }

};

// @@ !! END ---- mo.cc
