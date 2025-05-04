#include <bits/stdc++.h>
#include <cassert>
using namespace std;
using ll = long long int;
using pll = pair<ll, ll>;

// @@ !! LIM(bellmanford random forall f:updMaxMin debug)

Random myrand;

void bf_test1() {
  BellmanFord bf(3);
  bf.add_edge(0, 1,  5);
  bf.add_edge(0, 2, -1);
  bf.add_edge(1, 2,  1);
  bf.add_edge(2, 1,  2);
  bool b = bf.run(0);
  assert(b);
  assert(bf.dist[1] ==  1);
  assert(bf.dist[2] == -1);
}

void bf_test2() {
  BellmanFord bf(3);
  bf.add_edge(0, 1,  5);
  bf.add_edge(0, 2, -1);
  bf.add_edge(1, 2,  1);
  bf.add_edge(2, 1,  2);
  bf.add_edge(1, 0, -2);
  bool b = bf.run(0);
  assert(not b);
}

void bf_test3()  {
  ll big = 2e18;
  ll repc = 10000;
  ll ncf = 0;
  for (int _rep = 0; _rep < repc; _rep++) {
    ll n = myrand.range(1, 6);
    ll m;
    if (n == 1) m = 0;
    else if (n <= 3) m = myrand.range(1, n * (n - 1) + 1);
    else m = myrand.range(2, n * (n - 1) / 2);
    set<pll> ps;
    for (ll i = 0; i < m; ) {
      ll a = myrand.range(0, n);
      ll b = myrand.range(0, n);
      if (a == b) continue;
      if (ps.contains(pll(a, b))) continue;
      ps.insert(pll(a, b));
      i++;
    }
    vector<tuple<ll, ll, ll>> vec;
    for (auto [a, b] : ps) {
      ll c = myrand.range(-8, 11);
      vec.emplace_back(a, b, c);
    }
    auto f_bf = [&]() {
      BellmanFord bf(n);
      for (auto [a, b, c] : vec) bf.add_edge(a, b, c);
      return make_pair(bf.run(0), bf.dist);
    };
    auto f_wf = [&]() {
      vector dist(n, vector(n, big));
      for (ll i = 0; i < n; i++) dist[i][i] = 0;
      for (auto [a, b, c] : vec) dist[a][b] = c;
      for (ll k = 0; k < n; k++) for (ll i = 0; i < n; i++) for (ll j = 0; j < n; j++) {
            updMin(dist[i][j], dist[i][k] + dist[k][j]);
          }
      return make_pair(FORALL(i, 0, n, dist[i][i] == 0), dist[0]);
    };
    auto asame = [&big](ll x, ll y) -> bool { return (x >= big / 2 and y >= big / 2) or x == y; };
    auto [b1, vbf] = f_bf();
    auto [b2, vwf] = f_wf();
    if (b1 != b2) {
      DLOGK(vec);
      DLOGK(b1, b2);
      DLOGK(vbf);
      DLOGK(vwf);
      assert(false);
    }
    if (b1) {
      ncf++;
      bool b = FORALL(i, 0, n, asame(vbf[i], vwf[i]));
      if (not b) {
        DLOGK(vec);
        DLOGK(vbf);
        DLOGK(vwf);
        assert(false);
      }
    }
  }
  // cerr << std::format("Negative cycle free cases are {} among {} cases.\n", ncf, repc);
}

void ug_test1() {
  UshiGame ug1(3);
  ug1.add_constrGE(0, 1, 10);
  ug1.add_constrGE(1, 2, 20);
  ug1.add_constrLE(0, 1, 100);
  ug1.add_constrLE(1, 2, 200);
  assert(*ug1.getmax(0, 2) == 300);
  assert(*ug1.getmin(0, 2) == 30);
  assert(*ug1.getmax(2, 0) == -30);
  ug1.add_constrLE(0, 1, 9);
  assert(not ug1.getmax(0, 1));
  assert(not ug1.getmin(1, 2));
};

void ug_test2() {
  ll big = 2e18;

  string path = "/home/y-tanabe/proj/compprog-clib/test/data/bellmanford1.txt";
  std::ifstream file(path);
  if (!file) {
    cerr << "Failed to open file " << path << endl;
    return;
  }
  ll t; file >> t;
  for (ll _i = 0; _i < t; _i++) {
    ll n, i_from, i_to, num_cl, num_cr; file >> n >> i_from >> i_to >> num_cl >> num_cr;
    bool isMax; file >> isMax;
  
    UshiGame ug(n);
    for (ll i = 0; i < num_cl; i++) {
      ll a, b, c; file >> a >> b >> c;
      // DLOGKL("code1", a, b, c);
      ug.add_constrLE(a, b, c);
    }
    for (ll i = 0; i < num_cr; i++) {
      ll a, b, c; file >> a >> b >> c;
      // DLOGKL("code2", a, b, c);
      ug.add_constrGE(a, b, c);
    }
    ll exp_rc, exp_ans; file >> exp_rc >> exp_ans;
    auto call = [&]() {
      if (isMax) {
        // DLOGKL("call getmax", i_from, i_to);
        return ug.getmax(i_from, i_to);
      } else {
        // DLOGKL("call getmin", i_from, i_to);
        return ug.getmin(i_from, i_to);
      }
    };
    auto r = call();
    if (r) {
      // DLOGKL("return from call (ok) ", *r);
      if (*r >= big / 2 or *r <= - big / 2) {
        assert(exp_rc == -2);
      }else {
        assert(exp_rc == 0 and exp_ans == *r);
      }
    }else {
      assert(exp_rc == -1);
    }
  }
}

int main() {

  bf_test1();
  bf_test2();
  bf_test3();

  ug_test1();
  ug_test2();

  cerr << "ok" << endl;
  return 0;
}


