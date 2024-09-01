#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;
#define REP(i, a, b) for (ll i = (a); i < (b); i++)
#define REPrev(i, a, b) for (ll i = (a); i >= (b); i--)
#define ALL(coll) (coll).begin(), (coll).end()
#define SIZE(v) ((ll)((v).size()))
#define REPOUT(i, a, b, exp, sep) REP(i, (a), (b)) cout << (exp) << (i + 1 == (b) ? "" : (sep)); cout << "\n"

// @@ !! LIM(rle)

int main() {

  {
    /*
      rle() should return vector<pair<T, ll>> rather than vector<pair<T, int>> although
      the return value is always expected to be in the range of int.
      Typical code such as the following would fail if rle() returns vector<pair<T, int>>.
     */
    string s("aaaaaaaabcdddddddd");
    ll cnt = 0;
    for (auto [a, n] : rle(s)) {
      int x = 1 << 30;
      ll lim = 1LL << 32;
      if (x * n >= lim) cnt++;   // overflows if n is of type int.
    }
    assert(cnt == 2);
  }

  {
    vector<ll> vec1{5, 10, 7, 4, 4, 4, 4, 2, 2};
    vector<pair<ll, ll>> exp{{5, 1}, {10, 1}, {7, 1}, {4, 4}, {2, 2}};
    vector<pair<ll, ll>> res;
    rle_iter(ALL(vec1), back_inserter(res));
    assert(res == exp);
  }

  {
    auto res1 = rle(string("aaabccccbbdd"));
    vector<pair<char, ll>> exp1{{'a', 3}, {'b', 1}, {'c', 4}, {'b', 2}, {'d', 2}};
    assert(res1 == exp1);
    auto res2 = rle(string());
    vector<pair<char, ll>> exp2{};
    assert(res2 == exp2);
  }

  {
    deque<ll> deq1{2, 2, 2, 7, 4, 8, 8, 2, 7, 7, 7};
    auto res1 = rle(deq1);
    vector<pair<ll, ll>> exp1{{2, 3}, {7, 1}, {4, 1}, {8, 2}, {2, 1}, {7, 3}};
    assert(res1 == exp1);
  }

  cerr << "ok" << endl;
}
