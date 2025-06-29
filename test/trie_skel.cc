#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;

// @@ !! LIM(trie random debug)

template<int bt_size, typename S, bool b_compact, bool b_has_offset>
struct MyTest {
  void run() {
    // DLOGK(bt_size, b_compact, b_has_offset);

    constexpr char from = 'a';
    Random rand;

#if DEBUG
    ll rep1 = 25, rep2 = 25;
    ll maxlen = 4;
#else
    ll rep1 = 30, rep2 = 100;
    ll maxlen = 4;
#endif
    /*  This is for through testing
    ll rep1 = 100, rep2 = 300;
    ll maxlen = 6;
    */

    for (ll _r1 = 0; _r1 < rep1; _r1++) {
      auto mk_seq = [&](ll len) -> S {
        S s;
        for (ll i = 0; i < len; i++) s.push_back(from + rand.range(0, bt_size));
        return s;
      };
      auto root = new Trie<bt_size, from, monostate, S, b_compact, b_has_offset>();
      set<S> naive;
      for (ll _r2 = 0; _r2 < rep2; _r2++) {
        ll len = rand.range(0, maxlen + 1);
        S s1 = mk_seq(len);
        auto p1 = root->insert(s1);
        naive.insert(s1);
        // DLOGKL("insert", s1, naive, root->show(), root->size_st);
        assert(p1->reside);
        assert(root->size_st == ssize(naive));
        ll x = p1->get_offset();
        if (s1.empty()) assert(x == -1);
        else {
          // DLOGKL("  get_offset", s1, x, s1.back());
          assert(x == s1.back() - from);
        }

        S s2 = mk_seq(len);
        auto p2 = root->search(s2);
        assert(not p2 == not naive.contains(s2));
        auto p2a = root->get_node(s2);
        if (p2a) {
          assert(p2a->repr() == s2);
          p2a->erase();
          naive.erase(s2);
          assert(not root->search(s2));
          assert(root->size_st == ssize(naive));
        }
            
        S s3 = mk_seq(len);
        if (rand.range(0, 10) < 5) {
          auto p3 = root->search(s3);
          auto p3a = root->get_node(s3);
          auto p3b = root->get_node(s3, true);
          if (p3a) assert(p3a == p3b);
          else assert(not p3 and p3b->repr() == s3);
        }else {
          auto p3 = root->get_node(s3);
          if (p3) {
            int d = rand.range(0, bt_size);
            char c = from + d;
            auto p3a = p3->get_child_val(c);
            auto p3c = p3->get_child_offset(d);
            auto p3b = p3->get_child_val(c, true);
            auto p3d = p3->get_child_offset(d, true);
            assert(p3a == p3c);
            if (p3a) assert(p3a == p3b and p3c == p3d);
            else {
              S ss = s3;
              ss.push_back(c);
              assert(p3b == p3d and p3b->repr() == ss);
            }
            
            vector<char> rec1;
            for (auto [p, cc] : p3->children_w_val()) {
              assert(p->get_offset() == cc - from);
              rec1.emplace_back(cc);
            }
            vector<char> rec2;
            for (ll dd = 0; dd < bt_size; dd++) {
              auto p = p3->get_child_offset(dd);
              if (p) rec2.emplace_back(from + p->get_offset());
            }
            assert(rec1 == rec2);

          }
        }

        auto vec = root->show();
        assert(ssize(vec) == ssize(naive) and root->size_st == ssize(naive));
        {
          ll i = 0;
          for (S s : naive) assert(s == vec[i++]);
        }
      }
    }
  }
};


int main(int argc, char *argv[]) {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << setprecision(20);

  {
    auto tr1 = new Trie<26, 'a', monostate, string, false, false>();

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
    bool b7 = tr1->show() == vector<string>{"", "abcaaa", "abczzz", "az"};
    assert(b7);

    auto p11 = tr1->search("az");
    assert(p11->repr() == "az");

    auto tr2 = new Trie<2, '0', monostate, string, true, false>();
    tr2->insert("010101");
    vector<int> vec3{0, 0, 1, 1, 0, 1, 0};

    auto tr3 = new Trie<2, 0, monostate, vector<int>, false, true>();
    tr3->insert(vec3);

    auto tr4 = new Trie<26, 'A', monostate, string, true, true>();
    tr4->insert("AZBCD");

  }

  (new MyTest<2, string, true,  true >()) -> run();
  (new MyTest<2, string, true,  false>()) -> run();
  (new MyTest<2, string, false, true >()) -> run();
  (new MyTest<2, vector<char>, false, true >()) -> run();
  (new MyTest<4, vector<char>, true,  true >()) -> run();
  (new MyTest<5, vector<int>, true,  false>()) -> run();
  (new MyTest<6, deque<char>, false, true >()) -> run();
  (new MyTest<7, string, false, true >()) -> run();

  {
    auto root = new Trie<26, 'a', ll>;
    vector<string> data{"abcde", "abe", "abcde", "a", "ab", "x", "az", "a"};
    map<string, int> mp;
    for (string s : data) {
      auto p = root->insert(s);
      p->user ++;
      mp[s]++;
    }
    for (string s : root->show()) assert(root->search(s)->user == mp[s]); 
    for (auto [s, n] : mp) assert(root->search(s)->user == n);
  }


  cout << "ok\n";
  return 0;
}
