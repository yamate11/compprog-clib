/*
  Disjoint Sparse Table
     cf. https://noshi91.hatenablog.com/entry/2018/05/08/183946

  construction: O(N logN)
  query:        O(1)

  Note that:
  * construction is slower than segment trees
  * modification is not allowed

  Usage:
    DisjSparseTbl dst(unit, add_func, init_dat);   // add_func is not necessarily be commutative.
    dst.query(l, r);   // half-open interval [l, r)
      // From the point of implementation, closed interval [l, r] is more natural,
      // but we prefer similarity to segment trees.
 */

//////////////////////////////////////////////////////////////////////
// @@ !! BEGIN() ---- disjSparseTbl.cc

template <typename DAT, typename ADD_tp>
struct DisjSparseTbl {
  int size;
  int height;
  vector<vector<DAT>> tbl;
  DAT unit_dat;
  ADD_tp add;      // not necessarily be commutative

  DisjSparseTbl() {}
  DisjSparseTbl(DAT unit_dat_, ADD_tp add_, vector<DAT> values)
    : unit_dat(unit_dat_), add(add_) { set_vals(move(values)); }
  
  void set_vals(vector<DAT> values) {
    size = ssize(values);
    height = max(1, (int)countr_zero(bit_ceil((unsigned)size)));
    tbl.resize(height);
    tbl[0] = move(values);
    for (int h = 1; h < height; h++) {
      int len = 1 << h;
      int blen = 2 * len;
      int numB = (size + blen - 1) / blen;
      tbl[h].resize(size);
      for (int i = 0; i < numB; i++) {
        int c = i * blen + len;
        if (c >= size) continue; // This block will not be used by query().
        tbl[h][c - 1] = tbl[0][c - 1];
        for (int j = c - 2; j >= c - len; j--) tbl[h][j] = add(tbl[0][j], tbl[h][j + 1]);
        tbl[h][c] = tbl[0][c];
        for (int j = c + 1; j < min(size, c + len); j++) tbl[h][j] = add(tbl[h][j - 1], tbl[0][j]);
      }
    }
  }

  DAT query(int l, int r) {  // half-open interval [l, r)
#if DEBUG
    if (not (0 <= l and l <= r and r <= size)) {
      cerr << "DisjSparseTbl: Warning: border values inconsistent: l = " << l << ", r = " << r << endl;
      l = max(0, l);
      r = max(0, r);
      l = min(size, l);
      r = min(size, r);
      r = max(l, r);
    }
#endif
    r--;  // Now, the interval is closed: [l, r]
    if (l > r) return unit_dat;
    if (l == r) return tbl[0][l];
    int h = bit_width((unsigned)(l ^ r)) - 1;
    return add(tbl[h][l], tbl[h][r]);
  }
};

// @@ !! END() ---- disjSparseTbl.cc
