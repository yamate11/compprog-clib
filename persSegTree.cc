/*
  Persistent Segment Tree (ordinary / lazy)

  * make_pers_seg_tree[_lazy] has similar interface to make_seg_tree[_lazy].  Differences are:
    * init_vec is mandotary
    * it returns a pair of a segment tree object and a pointer to the initial root
  * query(), set(), update() require the pointer to a root as the first argument.


  Typical Usage:

  // lazy version
  using DAT = ll;
  using OP = optional<ll>;
  const ll big = 1LL << 60;
  const DAT unit_dat = big;
  const OP unit_op = OP{};
  auto add = [](DAT x, DAT y) -> DAT { return min(x, y); };
  auto comp = [](OP f, OP g) -> OP { return f ? f : g; };
  auto appl = [](OP f, DAT x) -> DAT { return f ? *f : x; };
  vector<DAT> init_vec = ....;
  auto [stL, rootL] = make_pers_seg_tree_lazy(unit_dat, unit_op, add, comp, appl, init_vec);
  auto p1 = stL.update(rootL, lo1, hi1, op);
  auto p2 = stL.set(p1, i, newVal);
  ll ans1 = stL.query(p2, lo2, hi2);

  // ordinary version
  auto [stO, rootO] = make_per_seg_tree(0LL, plus<ll>(), vector<ll>(N));
  auto p3 = stO.set(rootO, i, newVal);
  ll ans2 = stO.query(p3, lo3, hi3);
*/

//////////////////////////////////////////////////////////////////////
// @@ !! BEGIN() ---- persSegTree.cc
// https://github.com/yamate11/compprog-clib/blob/master/persSegTree.cc

template <typename DAT, typename OP,
	  typename ADD_t, typename COMP_t, typename APPL_t, bool lazy>
struct GenPersSegTree {
  
  struct NodeOrd {
    NodeOrd* left;
    NodeOrd* right;
    DAT val;
  };
  
  struct NodeLazy {
    NodeLazy* left;
    NodeLazy* right;
    DAT val;
    OP susp;
  };

  using Node = conditional_t<lazy, NodeLazy, NodeOrd>;

  DAT unit_dat;
  OP unit_op;
  ADD_t add;
  COMP_t comp;
  APPL_t appl;
  int size = 0;

  Node* newNode() {
    if constexpr (lazy) return new Node(nullptr, nullptr, unit_dat, unit_op);
    else                return new Node(nullptr, nullptr, unit_dat);
  };

  Node* newNodeVal(DAT val) {
    Node* nd = newNode();
    nd->val = move(val);
    return nd;
  }

  Node* newNodeTwo(Node* nd1, Node* nd2) {
    Node* nd = newNode();
    nd->left = nd1;
    nd->right = nd2;
    nd->val = add(nd1->val, nd2->val);
    return nd;
  }

  Node* fromVector(const vector<DAT>& vec) {
    auto sub = [&](auto rF, ll lo, ll hi) -> Node* {
      if (lo + 1 == hi) {
        return newNodeVal(vec[lo]);
      }else {
        ll mid = (lo + hi) / 2;
        return newNodeTwo(rF(rF, lo, mid), rF(rF, mid, hi));
      }
    };
    size = ssize(vec);
    return sub(sub, 0, size);
  }

  DAT query(Node* root, ll rngLo, ll rngHi) const {
    if (rngHi <= rngLo) return unit_dat;
    auto sub = [&](auto rF, Node* nd, ll lo, ll hi) -> DAT {
      if (rngHi <= lo or  hi <= rngLo) return unit_dat;
      if (rngLo <= lo and hi <= rngHi) return nd->val;
      ll mid = (lo + hi) / 2;
      DAT ret;
      if constexpr (lazy) ret = appl(nd->susp, add(rF(rF, nd->left, lo, mid), rF(rF, nd->right, mid, hi)));
      else                ret =                add(rF(rF, nd->left, lo, mid), rF(rF, nd->right, mid, hi));
      return ret;
    };
    return sub(sub, root, 0, size);
  }

  Node* _push(Node* nd, const OP& op) {
    if constexpr (lazy) {
      if (op == unit_op) return nd;
      return new Node(nd->left, nd->right, appl(op, nd->val), comp(op, nd->susp));
    }else {
      return nd;
    }
  }

  Node* _func_update_set(Node* root, ll rngLo, ll rngHi, auto fin) {
    auto sub = [this, &rngLo, &rngHi, &fin](auto rF, Node* nd, ll lo, ll hi, const OP& op) -> Node* {
      if (rngHi <= lo or  hi <= rngLo) return _push(nd, op);
      if (rngLo <= lo and hi <= rngHi) return fin(nd, op);
      ll mid = (lo + hi) / 2;
      OP comped;
      if constexpr (lazy) comped = comp(op, nd->susp);
      else                comped = OP{};
      Node* ndL = rF(rF, nd->left,  lo,  mid,      comped );
      Node* ndR = rF(rF, nd->right, mid, hi,  move(comped));
      // Make sure that "move(comped)" is called after "comped".
      return newNodeTwo(ndL, ndR);
    };
    return sub(sub, root, 0, size, unit_op);
  }

  Node* update(Node* root, ll rngLo, ll rngHi, const OP& op_asked) {
    static_assert(lazy, "update() only supports for lazy trees.");
    return _func_update_set(root, rngLo, rngHi,
                            [this, &op_asked](Node* nd, const OP& op) { return _push(nd, comp(op_asked, op)); });
  }

  Node* set(Node* root, ll i, DAT t) {
    return _func_update_set(root, i, i + 1, [this, &t](Node*, const OP&) { return newNodeVal(move(t)); });
  }
};

template<typename DAT>
auto make_pers_seg_tree(DAT unit_dat, auto add, const vector<DAT>& init_dat) {
  auto st = GenPersSegTree<DAT, monostate, decltype(add), monostate, monostate, false
                           >(unit_dat, {}, add, {}, {}, 0);
  auto root = st.fromVector(init_dat);
  return make_pair(st, root);
}

template<typename DAT, typename OP>
auto make_pers_seg_tree_lazy(DAT unit_dat, OP unit_op, auto add, auto comp, auto appl, const vector<DAT>& init_dat) {
  auto st = GenPersSegTree<DAT, OP, decltype(add), decltype(comp), decltype(appl), true
                           >(unit_dat, unit_op, add, comp, appl, 0);
  auto root = st.fromVector(init_dat);
  return make_pair(st, root);
}

// @@ !! END() ---- persSegTree.cc
