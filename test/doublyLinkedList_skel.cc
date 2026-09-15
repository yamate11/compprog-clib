#include <bits/stdc++.h>
#include <cassert>
using namespace std;
using ll = long long int;
using pll = pair<ll, ll>;
using ull = unsigned long long;

// @@ !! LIM(debug random doublyLinkedList)

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << setprecision(20);
  Random rand;

  {
#if DEBUG
    ll rep1 = 100, rep2 = 100;
#else
    ll rep1 = 500, rep2 = 500;
#endif
    for (ll i1 = 0; i1 < rep1; i1++) {
      using myDLL = DLList<pll>;
      myDLL* root = new myDLL();
      vector<pll> vec;
      auto get_pos = [&](ll pos) -> myDLL* {
        myDLL* ret = root->nxt;
        for (int i = 0; i < pos; i++) ret = ret->nxt;
        return ret;
      };
      auto vec_view = [&]() -> vector<pll> {
        vector<pll> ret;
        for (myDLL* p = root->nxt; p; p = p->nxt) ret.push_back(p->val);
        return ret;
      };
      for (ll i2 = 0; i2 < rep2; i2++) {
        ll sz = ssize(vec);
        ll z = sz == 0 ? 0 : rand.range(0, 3);
        if (z == 0) {
          pll pair(rand.range(0, 100), rand.range(0, 100));
          if (sz == 0) {
            vec.push_back(pair);
            root->insert_after(move(pair));
          }else {
            ll pos = rand.range(0, sz + 1);
            vec.insert(vec.begin() + pos, pair);
            bool bf = pos == 0 or (pos != sz and rand.range(0, 2) == 0);
            if (bf) get_pos(pos)->insert_before(move(pair));
            else    get_pos(pos - 1)->insert_after(move(pair));
          }
        }else if (z == 1) {
          pll pair(rand.range(0, 100), rand.range(0, 100));
          ll pos = rand.range(0, sz);
          vec[pos] = pair;
          get_pos(pos)->val = move(pair);
        }else if (z == 2) {
          ll pos = rand.range(0, sz);
          vec.erase(vec.begin() + pos);
          if (rand.range(0, 5) == 0) {
            myDLL* p = get_pos(pos);
            auto [a, b] = p->val;
            p->remove();
            assert(p->val == pll(a, b));
            delete p;
          }else {
            myDLL::erase(get_pos(pos));
          }
        }else assert(0);
        // DLOGK(z, vec);
        // DLOGK(vec_view());
        assert(vec == vec_view());
      }
    }
  }

  {
    using myDLL = DLList<ll>;
    myDLL* root = new myDLL(-1000);
    myDLL* p = root;
    for (ll i = 0; i < 5; i++) p = p->insert_after(i);
    assert(g_show(*root) == "[-1000, 0, 1, 2, 3, 4]");
  }


  cerr << "ok\n";

  return 0;
}
