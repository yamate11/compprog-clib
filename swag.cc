/*
  SWAG

  Usage:
    auto sw = make_swag(multiplies<MyMatrix>(), MyMatrix{{1,0},{0,1}});
              // operator and its unit element.
              // operator should be associative.
    sw.push(obj1);   // push obj1 into the queue
    sw.pop();        // pop the front element
    T v = sw.get_val(); // calculate the aggregated value e1 `op` e2 `op` ... `op` en
                        //     where e1 is the front and en is the back
    T a = sw.front();   // the front element
    T b = sw.back();    // the back element
    auto vec = sw.vec_view();  // list the elements for debugging

 */

//////////////////////////////////////////////////////////////////////
// See help of libins command for dependency spec syntax.
// @@ !! BEGIN() ---- swag.cc

template<typename T, typename prod_tp>
struct swag {
  prod_tp prod;
  T unit;
  vector<T> elem_front{};
  vector<T> val_front{};
  vector<T> elem_back{};
  vector<T> val_back{};
  
  void push(T t) {
    const T& e = val_back.empty() ? unit : val_back.back();
    val_back.push_back(prod(e, t));  // Order matters!
    elem_back.push_back(move(t));
  }
  void pop() {
    if (val_front.empty()) {
      if (val_back.empty()) throw runtime_error("swag empty");
      while (not val_back.empty()) {
        val_back.pop_back();
        T t = move(elem_back.back());
        elem_back.pop_back();
        const T& e = val_front.empty() ? unit : val_front.back();
        val_front.push_back(prod(t, e)); // Order matters!
        elem_front.push_back(move(t));
      }
    }
    val_front.pop_back();
    elem_front.pop_back();
  }
  T get_val() const {
    T vf = val_front.empty() ? unit : val_front.back();
    T vb = val_back.empty()  ? unit : val_back.back();
    return prod(vf, vb);
  }
  const T& back() const {
    if (not elem_back.empty()) return elem_back.back(); 
    else if (not elem_front.empty()) return elem_front.front();
    else throw runtime_error("swag empty");
  }
  const T& front() const {
    if (not elem_front.empty()) return elem_front.back();
    else if (not elem_back.empty()) return elem_back.front();
    else throw runtime_error("swag empty");
  }
  vector<T> vec_view() const {
    vector<T> ret;
    for (int i = ssize(elem_front) - 1; i >= 0; i--) ret.push_back(elem_front[i]);
    for (int i = 0; i < ssize(elem_back); i++) ret.push_back(elem_back[i]);
    return ret;
  }
};

template<typename T, typename prod_tp>
swag<T, prod_tp> make_swag(prod_tp prod, T unit) { return swag<T, decltype(prod)>(prod, unit); }

// @@ !! END ---- swag.cc
