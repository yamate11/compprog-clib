/*
  Doubly Linked List

  Synopsis:
    template<typename T>
    struct DLList {
      DLList* prv;   // pointer to the previous node
      DLList* nxt;   // pointer to the next node
      T val;         // data 
      DLList() = default;
      DLList(DLList* prv_, DLList* nxt_, T t) // natural constructor.  t will be moved.
      DLList(T t)    // when omitted, prv and nxt will be nullptr.
      DLList* insert_after(T t);  // inserted after this.  Again, t will be moved.
      DLList* insert_before(T t); // inserted before this.  Again, t will be moved.
      void remove();  // remove this from the list
      static void erase(DLList* p);   // p is remove()d and deleted.
      string show() const;        // for debugging
    };

  Usage:
    using myDLL = DLList<pll>;
    myDLL* root = new DLList();  // You should first create `root` that represents the empty list,
                                 // or you will be in trouble when you erase the last element of the list.
                                 // NOTE THAT the first element of the list IS NOT root->val BUT root->nxt->val.
    myDLL* p = root;
    REP(i, 0, N) {
        ll a, ll b; cin >> a >> b;
        p = p->insert_after(pll(a, b));
    }

    myDLL* p1 = p0->insert_before(t);  // t is inserted before p0 and the pointer to the new node is returned.
    assert(p1->val == t);              // "val" is the data member

    myDLL* p2 = b ? p1->prv : p1->nxt;  // "prv" and "nxt" are the pointers to the neighbours
   
    myDLL::erase(p);    // p is removed from the list and deleted

    DLOGK(*root);      // list contents from start will be printed.  NOTE THAT THE FIRST PRINTED IS A DUMMY.
 */

//////////////////////////////////////////////////////////////////////
// See help of libins command for dependency spec syntax.
// @@ !! BEGIN() ---- doublyLinkedList.cc

template<typename T>
struct DLList {
  DLList* prv{};
  DLList* nxt{};
  T val{};

  DLList() = default;
  DLList(T val_) : val(move(val_)) {}
  DLList(DLList* prv_, DLList* nxt_, T val_) : prv(prv_), nxt(nxt_), val(move(val_)) {}

  DLList* insert_before(T val_) {
    DLList* p = new DLList(prv, this, move(val_));
    if (prv) prv->nxt = p;
    prv = p;
    return p;
  }

  DLList* insert_after(T val_) {
    DLList* p = new DLList(this, nxt, move(val_));
    if (nxt) nxt->prv = p;
    nxt = p;
    return p;
  }

  void remove() {
    if (prv) prv->nxt = nxt;
    if (nxt) nxt->prv = prv;
  }

  static void erase(DLList* p) {
    p->remove();
    delete p;
  }

  string show() const {
    string s = "[";
    bool first = true;
    for (const DLList* p = this; p; p = p->nxt) {
      if (first) first = false;
      else s += ", ";
      s += g_show(p->val);
    }
    s += "]";
    return s;
  }

};

// @@ !! END ---- doublyLinkedList.cc
