#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;

// @@ !! LIM(f:<< trie)

int main(int argc, char *argv[]) {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << setprecision(20);

  {
    auto tr1 = new Trie<26, 'a', false, false>();
    auto tr2 = new Trie<2, '0', true, false>();
    auto tr3 = new Trie<2, 0, false, true>();
    auto tr4 = new Trie<26, 'A', true, true>();
  }


  cout << "ok\n";
  return 0;
}
