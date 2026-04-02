#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;

/*
  sortedIndex

  * sortedIndex2(vec, comp) returns a pair (ord2idx, idx2ord) of vector<INT>:
    - vec[ord2idx[0]], vec[ord2idx[1]], ..., vec[ord2idx[N-1]] is sorted.
    - idx2ord is the reverse of ord2idx
      Thus, vec[i] is the idx2ord[i]-th least element (zero-indexed).
    - comp can be omitted.  The default is less<T>(), where T is the value type of vec.
    - The default of INT is "long long".

  * sortedIndex(vec, comp) returns ord2idx: it is equivallent to sortedIndex2(vec, comp).first.

  Typical Usage:

    vector<string> vec = {"bbb", "ccc", "aaa"};

    auto [ord2idx, idx2ord] = sortedIndex2(vec);
      // ord2idx == vector<INT>{2, 0, 1},   idx2ord == vector<INT>{1, 2, 0}
    REP(ii, 0, N) {
      ll i = ord2idx[ii];
      // Now, vec[i] is the ii-th least element of vec.
    }

    auto ord2idx = sortedIndex(vec);   // Same as above if you do not need idx2ord.

    auto ord2idx = sortedIndex(vec, greater<string>());   // reverse order

*/

//////////////////////////////////////////////////////////////////////
// See help of libins command for dependency spec syntax.
// @@ !! BEGIN() ---- sortedIndex.cc

template<typename INT = long long int>
vector<INT> sortedIndex(const auto& vec, auto comp) {
  vector<INT> ret(vec.size());
  iota(ret.begin(), ret.end(), (INT)0);
  sort(ret.begin(), ret.end(), [&vec, &comp](INT i, INT j) { return comp(vec[i], vec[j]); });
  return ret;
}

template<typename INT = long long int>
pair<vector<INT>, vector<INT>> sortedIndex2(const auto& vec, auto comp) {
  auto ord2idx = sortedIndex(vec, comp);
  vector<INT> idx2ord(vec.size());
  for (size_t i = 0; i < vec.size(); i++) idx2ord[ord2idx[i]] = i;
  return {move(ord2idx), move(idx2ord)};
}

template<typename INT = long long int, typename Collection>
vector<INT> sortedIndex(const Collection& vec) {
  return sortedIndex<INT>(vec, less<typename Collection::value_type>());
}

template<typename INT = long long int, typename Collection>
pair<vector<INT>, vector<INT>> sortedIndex2(const Collection& vec) {
  return sortedIndex2<INT>(vec, less<typename Collection::value_type>());
}


// @@ !! END ---- sortedIndex.cc
