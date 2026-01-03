#include <bits/stdc++.h>
#include <cassert>
using namespace std;
using ll = long long int;
using pll = pair<ll, ll>;
// #include <atcoder/all>
// using namespace atcoder;

// @@ !! LIM(UnionFind FuncGraph)

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << setprecision(20);

  random_device rd;
  mt19937 rng(rd());
  auto randrange = [&rng](ll i, ll j) -> ll {
    uniform_int_distribution<ll> dist(i, j - 1);
    return dist(rng);
  };

  {
    FuncGraph fg(4);
    fg.add_edge(0, 1);
    fg.add_edge(1, 3);
    fg.add_edge(2, 2);
    fg.add_edge(3, 1);
    assert(fg.numcc() == 2);
    assert(fg.cc(0) == fg.cc(1) and fg.cc(1) == fg.cc(3));
    assert(fg.cc(0) != fg.cc(2));
    assert(fg.cc_size(fg.cc(0)) == 3);
    assert(fg.cc_size(fg.cc(1)) == 3);
    assert(fg.cc_size(fg.cc(2)) == 1);
    assert(fg.cc_size(fg.cc(3)) == 3);
    assert(fg.cycle_size(fg.cc(0)) == 2);
    assert(fg.cycle_size(fg.cc(1)) == 2);
    assert(fg.cycle_size(fg.cc(2)) == 1);
    assert(fg.cycle_size(fg.cc(3)) == 2);
    assert(fg.node_on_cycle(0) == 1);
    assert(fg.node_on_cycle(1) == 1);
    assert(fg.node_on_cycle(2) == 2);
    assert(fg.node_on_cycle(3) == 3);
    assert(not fg.is_on_cycle(0));
    assert(fg.is_on_cycle(1));
    assert(fg.is_on_cycle(2));
    assert(fg.is_on_cycle(3));
    assert(fg.dist_to_cycle(0) == 1);
    assert(fg.dist_to_cycle(1) == 0);
    assert(fg.dist_to_cycle(2) == 0);
    assert(fg.dist_to_cycle(3) == 0);
    for (int c = 0; c < fg.numcc(); c++) assert(fg.cc(fg.repr(c)) == c);
    {                 assert(fg.bwd(0).empty()); }
    { vector v{3, 0}; assert(fg.bwd(1) == v); }  // the order matters.
    { vector v{2};    assert(fg.bwd(2) == v); }
    { vector v{1};    assert(fg.bwd(3) == v); }
  }
  
  {
    FuncGraph fg(3);
    fg.add_edge(0, 1);
    fg.add_edge(1, 0);
    fg.add_edge(2, 1);
    assert(fg.numcc() == 1);
    assert(fg.cc_size(fg.cc(0)) == 3);
    assert(fg.cc_size(fg.cc(1)) == 3);
    assert(fg.cc_size(fg.cc(2)) == 3);
    assert(fg.cycle_size(fg.cc(0)) == 2);
    assert(fg.cycle_size(fg.cc(1)) == 2);
    assert(fg.cycle_size(fg.cc(2)) == 2);
    assert(fg.node_on_cycle(0) == 0);
    assert(fg.node_on_cycle(1) == 1);
    assert(fg.node_on_cycle(2) == 1);
    assert(fg.is_on_cycle(0));
    assert(fg.is_on_cycle(1));
    assert(not fg.is_on_cycle(2));
    assert(fg.dist_to_cycle(0) == 0);
    assert(fg.dist_to_cycle(1) == 0);
    assert(fg.dist_to_cycle(2) == 1);
    for (int c = 0; c < fg.numcc(); c++) assert(fg.cc(fg.repr(c)) == c);
    { vector v{1};    assert(fg.bwd(0) == v); }
    { vector v{0, 2}; assert(fg.bwd(1) == v); }  // the order matters.
    {                 assert(fg.bwd(2).empty()); }
  }
  
  {
    FuncGraph fg(3);
    fg.add_edge(0, 1);
    fg.add_edge(1, 2);
    fg.add_edge(2, 0);
    assert(fg.dist_to_cycle(0) == 0);
  }

  {
    int numRep = 10000;
    int sizeLim = 16;
    for (int rep = 0; rep < numRep; rep++) {
      int size = randrange(1, sizeLim);
      FuncGraph fg(size);
      UnionFind uf(size);
      for (int i = 0; i < size; i++) {
        int j = randrange(0, size);
        fg.add_edge(i, j);
        uf.merge(i, j);
      }
      ll nuf = 0; for (int i = 0; i < size; i++) if (uf.leader(i) == i) nuf++;
      assert(nuf == fg.numcc());
      for (int i = 0; i < size; i++) for (int j = i + 1; j < size; j++) {
          assert((uf.leader(i) == uf.leader(j)) == (fg.cc(i) == fg.cc(j)));
        }
      for (int i = 0; i < size; i++) assert(fg.cc_size(fg.cc(i)) == uf.group_size(i));
      for (int i = 0; i < size; i++) {
        ll p = fg.dist_to_cycle(i);
        ll c = fg.cycle_size(fg.cc(i));
        vector<int> traj(p + c);
        for (int j = 0, m = i; j < p + c; j++, m = fg.fwd[m]) traj[j] = m;
        for (int j = 0; j < p + c; j++) for (int k = j + 1; k < p + c; k++) assert(traj[j] != traj[k]);
        assert(fg.fwd[traj.back()] == traj[p]);
        assert(fg.node_on_cycle(i) == traj[p]);
        assert(fg.is_on_cycle(i) == (p == 0));
      }
      for (int c = 0; c < fg.numcc(); c++) {
        int i = fg.repr(c);
        assert(fg.cc(i) == c);
        assert(fg.is_on_cycle(i));
      }
      {
        int s = 0;
        for (int i = 0; i < size; i++) s += ssize(fg.bwd(i));
        assert(s == size);
        for (int i = 0; i < size; i++) {
          for (ll j : fg.bwd(i)) assert(fg.fwd[j] == i);
          if (fg.is_on_cycle(i)) assert(not fg.bwd(i).empty() and fg.is_on_cycle(fg.bwd(i)[0]));
        }
      }

    }
  }

  cout << "ok" << endl;
  return 0;
}

