#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;

// @@ !! LIM(debug random trie)

int dummy;

template<int bt_size, int set_mode, typename S, bool b_compact, bool b_has_offset>
struct MyTest {
  void run(int run_id) {
    dummy ^= run_id; // to suppress the unused-parameter warning
    // DLOGKL("** running **", run_id);

    constexpr char from = 'a';
    Random rand;

#if DEBUG
    ll rep1 = 25, rep2 = 25;
    ll maxlen = 4;
#else
    ll rep1 = 30, rep2 = 100;
    ll maxlen = 4;
#endif

#if 0
    // This is for thorough testing
    rep1 = 100;
    rep2 = 500;
    maxlen = 6;
#endif

    for (ll _r1 = 0; _r1 < rep1; _r1++) {
      auto mk_seq = [&](ll len) -> S {
        S s;
        for (ll i = 0; i < len; i++) s.push_back(from + rand.range(0, bt_size));
        return s;
      };
      auto root = new Trie<bt_size, from, set_mode, monostate, S, b_compact, b_has_offset>();
      using myset = conditional_t<set_mode == TRIE_SET_MULTI, multiset<S>, set<S>>;
      myset naive;
      for (ll _r2 = 0; _r2 < rep2; _r2++) {
        ll len = rand.range(0, maxlen + 1);
        S s1 = mk_seq(len);
        auto p1 = root->insert(s1);
        naive.insert(s1);
        assert(p1->reside);
        assert(root->size_st == ssize(naive));
        ll x = p1->get_offset();
        if (s1.empty()) assert(x == -1);
        else assert(x == s1.back() - from);

        S s2 = mk_seq(len);
        auto p2 = root->search(s2);
        assert(not p2 == not naive.contains(s2));
        if (naive.contains(s2)) {
          assert(p2->repr() == s2);
          if (rand.range(0, 2) == 0) p2->erase();
          else                       root->erase(s2);  // should have the same effect as above
          if (auto it = naive.find(s2); it != naive.end()) naive.erase(it);
          assert(not root->search(s2) == not naive.contains(s2));
          assert(root->size_st == ssize(naive));
        }
            
        S s3 = mk_seq(len);
        if (rand.range(0, 10) < 5) {
          auto p3 = root->search(s3);
          auto p3a = root->get_node(s3);
          auto p3b = root->get_or_create_node(s3);
          if (p3a) assert(p3a == p3b);
          else assert(not p3 and p3b->repr() == s3);
        }else {
          auto p3 = root->get_node(s3);
          if (p3) {
            int d = rand.range(0, bt_size);
            char c = from + d;
            auto p3a = p3->get_child_val(c);
            auto p3c = p3->get_child_offset(d);
            auto p3b = p3->get_or_create_child_val(c);
            auto p3d = p3->get_or_create_child_offset(d);
            assert(p3a == p3c);
            if (p3a) assert(p3a == p3b and p3c == p3d);
            else {
              S ss = s3;
              ss.push_back(c);
              assert(p3b == p3d and p3b->repr() == ss);
            }
          }
        }

        auto vec = root->elem_list();
        if (not (ssize(vec) == ssize(naive) and root->size_st == ssize(naive))) {
          DLOGK(naive);
          DLOGK(vec);
          DLOGK(ssize(vec), ssize(naive), root->size_st);
          assert(0);
        }
        {
          ll i = 0;
          for (S s : naive) assert(s == vec[i++]);
        }
      }
    }
  }
};

void test_set_none() { // test for SET_MODE_NONE
  constexpr int bt_size = 2;
  constexpr char from = '0';
  using pll = pair<ll, ll>;
  using MyTrie = Trie<bt_size, from, TRIE_SET_NONE, pll, string, false, true>;
  MyTrie* root;
  multimap<string, ll> naive;
  auto add = [&](string s, ll val) -> void {
    MyTrie* p = root->get_or_create_node(s);
    p->user.first += val;
    for (; p; p = p->parent) p->user.second += val;
    naive.emplace(s, val);
  };
  auto check = [&](string s) -> void {
    MyTrie* p = root->get_node(s);
    ll val_trie = p ? p->user.second : 0;
    ll val_naive = 0;
    for (auto it = naive.lower_bound(s); it != naive.end() and it->first.substr(0, ssize(s)) == s; it++) {
      val_naive += it->second;
    }
    assert(val_trie == val_naive);
  };
  
  Random rand;
  ll rep1 = 25;
  ll rep2 = 100;
  ll maxlen = 4;
  for (int r = 0; r < rep1; r++) {
    root = new MyTrie;
    naive = multimap<string, ll>();
    for (int r2 = 0; r2 < rep2; r2++) {
      string s;
      ll len = rand.range(0, maxlen + 1);
      for (int i = 0; i < len; i++) s += from + rand.range(0, bt_size);
      ll val = rand.range(0, 100);
      if (rand.range(0, 2) == 0) add(s, val);
      else                       check(s);
    }
  }
}

void test_show() {
  {
    using MyTrieA = Trie<2, '0', TRIE_SET_SINGLE>;
    auto rootA = new MyTrieA;
    rootA->insert("001");
    rootA->insert("01");
    rootA->insert("001");
    rootA->insert("10");
    rootA->erase("01");
    string s = rootA->show();
    string exp = "[ (001, 1, 1),\n  (10, 1, 1)]";
    assert(rootA->show() == exp);
    assert(g_show(*rootA) == exp);
  }
  {
    using MyTrie = Trie<4, 'a', TRIE_SET_MULTI>;
    auto root = new MyTrie;
    root->insert("ab");
    root->insert("dd");
    root->insert("ab");
    string exp1 = "[ (ab, 2, 2),\n  (dd, 1, 1)]";
    string exp2 = "[ (, 0, 3),\n  (a, 0, 2),\n  (ab, 2, 2),\n  (d, 0, 1),\n  (dd, 1, 1)]";
    string s1 = root->show();
    string s2 = root->show(false);
    assert(s1 == exp1);
    assert(s2 == exp2);
  }
  {
    using MyTrie = Trie<4, 'a', TRIE_SET_NONE, pair<int, int>>;
    auto root = new MyTrie;
    auto p1 = root->insert("ab");
    p1->user = {3, 5};
    auto p2 = root->insert("cc");
    p2->user = {-5, 0};
    string exp = "[ (, (0, 0)),\n  (a, (0, 0)),\n  (ab, (3, 5)),\n  (c, (0, 0)),\n  (cc, (-5, 0))]";
    string s = root->show();
    assert(s == exp);
  }
}


int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << setprecision(20);

  {
    auto tr1 = new Trie<26, 'a', TRIE_SET_SINGLE, monostate, string, false, false>();

    tr1->insert("az");
    tr1->insert("abcppp");
    tr1->insert("abcaaa");
    tr1->insert("abczzz");
    assert(not tr1->search("abc"));
    auto p2 = tr1->get_node("abc");
    assert(not p2->reside);
    assert(p2->size_st == 3);
    auto p3 = p2->get_child_val('p');
    assert(p3->size_st == 1 and not p3->reside);
    auto p4 = p2->get_child_offset(0);
    assert(p4->size_st == 1 and not p4->reside);
    assert(p4->repr() == "abca");

    auto p5 = tr1->insert("");
    assert(p5 == tr1);
    assert(p5->reside);
    assert(p5->size_st == 5);

    auto p6 = tr1->search("abcppp");
    assert(p6->get_offset() == 'p' - 'a');
    p6->erase();
    assert(not p6->reside and p6->size_st == 0 and tr1->size_st == 4);
    bool b7 = tr1->elem_list() == vector<string>{"", "abcaaa", "abczzz", "az"};
    assert(b7);

    auto p11 = tr1->search("az");
    assert(p11->repr() == "az");
    auto p11a = tr1->search(string("az"));
    assert(p11a->repr() == "az");

    auto tr2 = new Trie<2, '0', TRIE_SET_SINGLE, monostate, string, true, false>();
    tr2->insert("010101");
    assert(tr2->search("010101"));
    tr2->erase("010101");
    assert(not tr2->search("010101"));

    vector<int> vec3{0, 0, 1, 1, 0, 1, 0};
    auto tr3 = new Trie<2, 0, TRIE_SET_SINGLE, monostate, vector<int>, false, true>();
    tr3->insert(vec3);
    assert(tr3->search(vec3));
    tr3->erase(vec3);
    assert(not tr3->search(vec3));

    auto tr4 = new Trie<26, 'A', TRIE_SET_SINGLE, monostate, string, true, true>();
    tr4->insert("AZBCD");
    assert(tr4->search("AZBCD"));
    tr4->erase("AZBCD");
    assert(not tr4->search("AZBCD"));
  }
  {
    
  }


  (new MyTest<2, TRIE_SET_SINGLE, string,        true,  true >()) -> run(1);
  (new MyTest<2, TRIE_SET_MULTI,  string,        true,  true >()) -> run(2);
  (new MyTest<2, TRIE_SET_SINGLE, string,        true,  false>()) -> run(3);
  (new MyTest<2, TRIE_SET_MULTI,  string,        false, true >()) -> run(4);
  (new MyTest<2, TRIE_SET_SINGLE, vector<char>,  false, true >()) -> run(5);
  (new MyTest<4, TRIE_SET_MULTI,  vector<char>,  true,  true >()) -> run(6);
  (new MyTest<5, TRIE_SET_SINGLE, vector<int>,   true,  false>()) -> run(7);
  (new MyTest<6, TRIE_SET_MULTI,  deque<char>,   false, true >()) -> run(8);
  (new MyTest<7, TRIE_SET_SINGLE, string,        false, true >()) -> run(9);

  {
    auto root = new Trie<26, 'a', TRIE_SET_SINGLE, ll>;
    vector<string> data{"abcde", "abe", "abcde", "a", "ab", "x", "az", "a"};
    map<string, int> mp;
    for (string s : data) {
      auto p = root->insert(s);
      p->user ++;
      mp[s]++;
    }
    for (string s : root->elem_list()) assert(root->search(s)->user == mp[s]); 
    for (auto [s, n] : mp) assert(root->search(s)->user == n);
  }

  test_set_none();  // set_mode == TEST_SET_NONE
  test_show();  // show()

  cout << "ok\n";
  return 0;
}
