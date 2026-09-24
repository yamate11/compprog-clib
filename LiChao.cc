/*
  Li Chao Tree
 */

//////////////////////////////////////////////////////////////////////
// See help of libins command for dependency spec syntax.
// @@ !! BEGIN() ---- LiChao.cc

// T must be integral
template<typename T = long long, typename comp_tp = less<T>>
/*
// requires (std::integral<T> || std::floating_point<T>)
... seems difficult to support segments for floating-point
*/
requires (std::integral<T>)
struct LiChaoDyn {
  struct Line {
    T a{};
    T b{};
    T val_at(T t) const { return a * t + b; }
  };

  struct Node {
    Line line;
    Node* cldL{};
    Node* cldH{};
    Node(Line line_ = Line{}) : line(line_) {}
    T val_at(T t) const { return line.val_at(t); }
  };

  T range_min;
  T range_max;
  comp_tp comp;
  Node* root{};
  T _unit = T{};
  bool _has_unit_set{false};
  
  LiChaoDyn(type_identity_t<T> rmin,      // so that T cannot be deduced from rmin
            type_identity_t<T> rmax,      // so that T cannot be deduced from rmax
            comp_tp comp_ = comp_tp()) : range_min(rmin), range_max(rmax), comp(comp_) {}

  T _better(T t1, T t2) const { return comp(t1, t2) ? t1 : t2; };

  T _get_midL(T lo, T hi) const { return lo + (hi - lo) / 2; } // beware lo + hi can be negative
  T _get_midH(T lo, T hi) const { return _get_midL(lo, hi) + 1; }

  void _sub_add_line(Node*& p, T lo, T hi, Line y) {
    // DLOGKL("_sub_add_line", lo, hi, y.a, y.b);
    if (not p) p = new Node(y);
    else {
      bool r_lo  = comp(p->val_at(lo), y.val_at(lo));
      bool r_hi  = comp(p->val_at(hi), y.val_at(hi));
      if (r_lo and r_hi) ; // nothing to do
      else if (not r_lo and not r_hi) p->line = y;
      else {
        T midL = _get_midL(lo, hi);
        T midH = _get_midH(lo, hi);
        bool r_mid = comp(p->val_at(midL), y.val_at(midL));
        if (not r_mid) {
          swap(p->line, y);
          r_lo = not r_lo;
          r_hi = not r_hi;
        }
        if (not r_lo)      _sub_add_line(p->cldL, lo, midL, y);
        else if (not r_hi) _sub_add_line(p->cldH, midH, hi, y);
        else assert(0);
      }
    }
  }
  void add_line(T a, T b) { _sub_add_line(root, range_min, range_max, Line{a, b}); }

  T _sub_query(Node* p, T lo, T hi, T t) const {
    if (not p) { throw runtime_error("LiChaoDyn.query: null pointer"); }
    T thisval = p->val_at(t);
    if (lo == hi) return thisval;
    T midL = _get_midL(lo, hi);
    if (t <= midL) return p->cldL ? _better(thisval, _sub_query(p->cldL, lo, _get_midL(lo, hi), t)) : thisval;
    else           return p->cldH ? _better(thisval, _sub_query(p->cldH, _get_midH(lo, hi), hi, t)) : thisval;
  }
  T query(T t) const { return _sub_query(root, range_min, range_max, t); }

  void set_unit(T _unit_) {
    // DLOGKL("set_unit is called", _unit_);
    _unit = _unit_;
    _has_unit_set = true;
    // DLOGKL("in set_unit", _unit);
  }

  void _sub_add_segment(Node*& p, T lo, T hi, T lo_segm, T hi_segm, Line y) {
    // DLOGKL("_sub_add_segment", lo, hi, lo_segm, hi_segm, y.a, y.b);
    if (hi < lo_segm or hi_segm < lo) ; // nothing to do 
    else if (lo_segm <= lo and hi <= hi_segm) _sub_add_line(p, lo, hi, y);
    else {
      if (not p) {
        // DLOGK(_unit);
        p = new Node(Line{T{}, _unit});
      }
      ll midL = _get_midL(lo, hi);
      ll midH = _get_midH(lo, hi);
      _sub_add_segment(p->cldL, lo, midL, lo_segm, hi_segm, y);
      _sub_add_segment(p->cldH, midH, hi, lo_segm, hi_segm, y);
    }
  }
  void add_segment(T a, T b, T lo_segm, T hi_segm) {
    if (not _has_unit_set) throw runtime_error("LChaoDyn.add_segment: set_unit() has not been called.");
    _sub_add_segment(root, range_min, range_max, lo_segm, hi_segm, Line{a, b}); 
  }

#if LIB_DEBUG
  string innerstr() const {
    stringstream ss;
    ll seq = 0;
    map<Node*, ll> mp;
    auto f = [&](auto rF, Node* p, ll lo, ll hi) -> void {
      ss << "{" << mp[p] << "}  y = " << p->line.a << " x + " << p->line.b << "  ";
      ss << "[" << lo << ", " << hi << "]  ";
      auto g = [&](Node* q) -> void {
        if (q) {
          ss << "(" << seq << ")  ";
          mp[q] = seq;
          seq++;
        }else {
          ss << "(null)  ";
        }
      };
      g(p->cldL);
      g(p->cldH);
      ss << "\n";
      if (p->cldL) rF(rF, p->cldL, lo, _get_midL(lo, hi));
      if (p->cldH) rF(rF, p->cldH, _get_midH(lo, hi), hi);
    };
    mp[root] = seq++;
    f(f, root, range_min, range_max);
    return ss.str();
  }
#endif // LIB_DEBUG

};

// @@ !! END ---- LiChao.cc
