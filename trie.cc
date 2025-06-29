#include <bits/stdc++.h>
#include <cassert>
using namespace std;
using ll =  long long int;
using u64 = unsigned long long;

/*
  https://yamate11.github.io/blog/posts/2025/06-28-trie-with-pointers/
 */

//////////////////////////////////////////////////////////////////////
// See help of libins command for dependency spec syntax.
// @@ !! BEGIN() ---- trie.cc

template <int bt_size, char from, bool compact = 2 < bt_size, bool has_offset = false>
struct Trie {

  Trie* parent = nullptr;

  using c_pat_t = conditional_t<compact, unsigned long long, monostate>;
  [[no_unique_address]] c_pat_t c_pat{};

  using cpt_children_t = conditional_t<compact, vector<Trie*>, monostate>;
  [[no_unique_address]] cpt_children_t cpt_children{};

  using children_t = conditional_t<compact, monostate, array<Trie*, bt_size>>;
  [[no_unique_address]] children_t children{};

  using offset_t = conditional_t<has_offset, int, monostate>;
  static consteval offset_t make_default_offset() {
    if constexpr (is_same_v<offset_t, int>) return -1;
    else return {};
  }
  [[no_unique_address]] offset_t offset = make_default_offset();

  bool reside = false;

  int size_st = 0;

  Trie() = default;
  Trie(Trie* p) : parent(p) {}

  Trie* get_child(int c, bool create) {
    ll d = c - from;
    if constexpr(compact) {
      ll idx = popcount(c_pat & ((1ULL << d) - 1));
      if (c_pat >> d & 1) return cpt_children[idx];
      if (not create) return nullptr;
      Trie* p = new Trie(this);
      if constexpr (has_offset) p->offset = d;
      cpt_children.insert(cpt_children.begin(), p);
      c_pat |= 1ULL << d;
      return p;
    }else {
      Trie* p = children[d];
      if (p) return p;
      if (not create) return nullptr;
      p = children[d] = new Trie(this);
      return p;
    }
  }

  Trie* _downward(const auto& s, bool create, bool res) {
    Trie* tr = this;
    for (auto c : s) {
      tr->size_st += res;
      auto cld = tr->get_child(c, create);
      if (not cld) return nullptr;
      tr = cld;
    }
    return tr;
  }

  Trie* search(const auto& s, bool create = false) {
    return _downward(s, create, false);
  }

  Trie* insert(const auto& s) {
    Trie* tr = _downward(s, true, true);
    tr->reside = true;
    return tr;
  }

  void erase() {
    reside = false;
    for (Trie* tr = this; tr; tr = tr->parent) tr->size_st--;
  }

  int get_offset() {
    if constexpr (has_offset) return offset;
    Trie* p = parent;
    if (not p) return -1;
    for (int d = 0; d < bt_size; d++) {
      if (p->c_pat >> d & 1 and p->get_child(from + d, false) == this) return d;
    }
    assert(0);
  }

  template<typename S>
  S repr() {
    S ret;
    for (Trie* tr = this; true; tr = tr->parent) {
      ll d = tr->get_offset();
      if (d < 0) break;
      ret.push_back(from + tr->get_offset());
    }
    reverse(ALL(ret));
    return ret;
  }

};


/*
template <typename T = ll>
struct TrieNode {
  // このノードに対応する文字列が Trie に格納されている数 
  int _num;
  // このノード以下に存在する要素の数 
  int _size;
  // ユーザデータ 
  T user;
  
  // コンストラクタ 
  TrieNode(int n = 0, int s = 0, const T& u = T())
    : _num(n), _size(s), user(u) {};
};

template <typename T = ll>
struct Trie {
  // 最初の文字 
  char from;
  // 文字の種類数 
  int br_size;
  // ノード．nodes[0] は，空文字列に対応する．
  vector<TrieNode<T>> nodes;
  // 添字 
      若干奇妙ではあるが，第 i 番目のノードに対応する文字列を s とするとき，
      その親ノードの添字を index[i * (br_size + 1)] に格納し，
      j 番目の子供，つまり，s + (char)(from + j) に対応するノードの添字を index[i * (br_size + 1) + (1 + j)]
      に格納している．
      つまり，index は (1 + br_size) ずつのグループになっており，
      各グループには先頭に親の添字を，そのあとは from, (char)(from + 1), ... の添字を格納している．

  vector<int> index;

  // コンストラクタ 
  Trie(char from_, int br_size_) : from(from_), br_size(br_size_), nodes(1), index(br_size + 1, -1) {}

  int& _parent_index(int idx) { return index[idx * (br_size + 1)]; }
  int& _child_index(int idx, char ch) { return index[idx * (br_size + 1) + 1 + (ch - from)]; }

  // 親ノードの添字 
  int parent_index(int idx) { return _parent_index(idx); }

  // 子ノードの添字．create = true の時には，ノードが存在しなければ作成する 
  int child_index(int idx, char ch, bool create = false) {
    int i = _child_index(idx, ch);
    if (i >= 0) return i;
    if (not create) return -1;
    int new_idx = nodes.size();
    nodes.resize(new_idx + 1);
    index.resize((new_idx + 1) * (br_size + 1), -1);
    _child_index(idx, ch) = new_idx;
    _parent_index(new_idx) = idx;
    return new_idx;
  }

  // 文字列ノードの添字 (途中から) 
  int prefixed_string_index(int idx, const string& s, bool create = false) {
    int cur = idx;
    for (size_t i = 0; i < s.size(); i++) {
      cur = child_index(cur, s[i], create);
      if (cur < 0) return cur;
    }
    return cur;
  }

  // 文字列ノードの添字 
  int string_index(const string& s, bool create = false) { return prefixed_string_index(0, s, create); }

  // 指定された添字を持つノードの文字列の存在個数 
  int num(int idx) { return idx < 0 ? 0 : nodes[idx]._num; }

  // 指定された添字を持つノード以下にある文字列の存在個数 
  int size(int idx) { return idx < 0 ? 0 : nodes[idx]._size; }

  // 文字列の存在個数を返す 
  int num_string(const string& s) { return num(string_index(s)); }

  // 指定された prefix を持つ文字列の存在個数を返す 
  int num_prefix(const string& prefix) { return size(string_index(prefix)); }

  // 文字列を1個追加する．対応するノードの添字を返す 
  int insert(const string& s) {
    int idx = string_index(s, true);
    nodes[idx]._num++;
    for (int i = idx; i >= 0; i = _parent_index(i)) nodes[i]._size++;
    return idx;
  }
  
  // 文字列を1個削除する 
  void erase(const string& s) {
    int idx = string_index(s);
    if (idx < 0) throw runtime_error("Trie: tried to erase non-existing node.");
    if (nodes[idx]._num == 0) throw runtime_error("Trie: tried to remove a number from zero.");
    nodes[idx]._num--;
    for (int i = idx; i >= 0; i = _parent_index(i)) nodes[i]._size--;
  }

  // 指定された添字のノードの文字列を返す．低効率デバッグ用 
  string node_to_str(int idx) {
    string ret;
    while (idx > 0) {
      int p = _parent_index(idx);
      char ch = from;
      for (; child_index(p, ch) != idx; ch++);
      ret.push_back(ch);
      idx = p;
    }
    reverse(ret.begin(), ret.end());
    return ret;
  }

  vector<string> _sub_string_set(int idx, const string& prefix) {
    vector<string> ret;
    for (int k = 0; k < num(idx); k++) ret.push_back(prefix);
    for (int i = 0; i < br_size; i++) {
      int j = _child_index(idx, from + i);
      if (j < 0) continue;
      auto vv = _sub_string_set(j, prefix + (char)(from + i));
      copy(vv.begin(), vv.end(), back_inserter(ret));
    }
    return ret;
  }

  // 格納されている文字列のリスト (vector<string>) を返す．デバッグ用 
  vector<string> string_set() {
    return _sub_string_set(0, "");
  }

};

template <typename T = ll>
Trie<T> create_trie(char from_, char to_) {
  Trie t(from_, to_ - from_ + 1);
  return t;
}

template <typename T = ll>
ostream& operator<<(ostream& ostr, Trie<T> trie) {
  ostr << trie.string_set();
  return ostr;
}

// Poorman's Binary Trie

string ull2binstr(unsigned long long x, int len = 64) {
  string ret(len, ' ');
  for (int i = 0; i < len; i++) ret[i] = (x >> (len - 1 - i) & 1) ? '1' : '0';
  return ret;
}

unsigned long long binstr2ull(string s) {
  unsigned long long ret = 0;
  for (char c : s) ret = 2 * ret + (c - '0');
  return ret;
}
*/


// @@ !! END ---- trie.cc
