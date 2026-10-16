#include <bits/stdc++.h>
#include <cassert>
using namespace std;
using ll = long long int;
using u64 = unsigned long long;
using pll = pair<ll, ll>;
#define REP(i, a, b) for (ll i = (a); i < (b); i++)
#define REPrev(i, a, b) for (ll i = (a); i >= (b); i--)
#define ALL(coll) (coll).begin(), (coll).end()

// @@ !! LIM(streamIn)

int main() {

  {
    stringstream ss1("5 2 4 -1");
    pair<int, int> p1, p2, q1({5, 2}), q2({4, -1});
    ss1 >> p1 >> p2;
    assert(p1 == q1 && p2 == q2);

    stringstream ss2("4 0 5 2 abc 1 -9");
    tuple<int, int, int> t31, ta31({4, 0, 5});
    tuple<int, string, bool, int> t41, ta41({2, "abc", true, -9});
    ss2 >> t31 >> t41;
    assert(t31 == ta31 && t41 == ta41);

    stringstream ss3("5 7 1 2 4 9");
    vector<pair<int, int>> v1(3), va1({{5, 7}, {1, 2}, {4, 9}});
    ss3 >> v1;
    assert(v1 == va1);
    stringstream ss4("5 7 1 2 4 9");
    auto v2 = read_vector_n<pair<int, int>>(ss4, 3);
    assert(v2 == va1);
    stringstream ss5("3 5 7 1 2 4 9");
    auto v3 = read_vector<pair<int, int>>(ss5);
    assert(v3 == va1);
    
  }

  cerr << "ok\n";
  return 0;
}
