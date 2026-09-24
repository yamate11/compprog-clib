/*
  Li Chao Tree
 */

//////////////////////////////////////////////////////////////////////
// See help of libins command for dependency spec syntax.
// @@ !! BEGIN() ---- LiChao.cc

// T must be integral or floating
template<typename T = long long, typename comp_tp = less<T>>
requires (std::integral<T> || std::floating_point<T>)
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
  
  LiChaoDyn(T rmin, T rmax, comp_tp comp_ = comp_tp()) : range_min(rmin), range_max(rmax), comp(comp_) {}

  T _better(T t1, T t2) const { return comp(t1, t2) ? t1 : t2; };

  void _sub_add_line(Node*& p, T lo, T hi, Line y) {
    if (not p) p = new Node(y);
    else {
      bool r_lo  = comp(p->val_at(lo), y.val_at(lo));
      bool r_hi  = comp(p->val_at(hi), y.val_at(hi));
      if (r_lo and r_hi) ; // nothing to do
      else if (not r_lo and not r_hi) p->line = y;
      else {
        T mid = (lo + hi) / 2;
        bool r_mid = comp(p->val_at(mid), y.val_at(mid));
        if (not r_mid) {
          swap(p->line, y);
          r_lo = not r_lo;
          r_hi = not r_hi;
        }
        if (not r_lo)      _sub_add_line(p->cldL, lo, mid, y);
        else if (not r_hi) _sub_add_line(p->cldH, mid, hi, y);
        else assert(0);
      }
    }
  }
  void add_line(T a, T b) { _sub_add_line(root, range_min, range_max, Line{a, b}); }

  T _sub_query(Node* p, T lo, T hi, T t) const {
    if (not p) { throw runtime_error("LiChaoDyn.query: null pointer"); }
    T thisval = p->val_at(t);
    T mid = (lo + hi) / 2;
    if (t < mid) return p->cldL ? _better(thisval, _sub_query(p->cldL, lo, mid, t)) : thisval;
    else         return p->cldH ? _better(thisval, _sub_query(p->cldH, mid, hi, t)) : thisval;
  }
  T query(T t) const { return _sub_query(root, range_min, range_max, t); }

#if LIB_DEBUG
  string innerstr() const {
    stringstream ss;
    ll seq = 0;
    map<Node*, ll> mp;
    auto f = [&](auto rF, Node* p) -> void {
      ss << "[" << mp[p] << "]  y = " << p->line.a << " x + " << p->line.b << "  ";
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
      if (p->cldL) rF(rF, p->cldL);
      if (p->cldH) rF(rF, p->cldH);
    };
    mp[root] = seq++;
    f(f, root);
    return ss.str();
  }
#endif // LIB_DEBUG

};

// @@ !! END ---- LiChao.cc
