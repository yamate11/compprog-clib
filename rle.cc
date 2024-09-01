#include <bits/stdc++.h>
#include <cassert>
using namespace std;
using ll = long long int;

/*
  RLE -- run length encoding

  (1) rle
      This receives string, vector<T>, deque<T> etc and returns vector<pair<T, int>>.  
      (In case of string, it returns vector<pair<char, int>>)
  (2) rle_iter
      Probably you do not need this.  See Usage.

  Usage:
    // (1)
    auto enc1 = rle(string("aabbbcdd")); // enc1 == vector<pair<char, int>>{{'a', 2}, {'b', 3}, {'c', 1}, {'d', 2}}
         // Note that rle("aabbbcdd") does NOT work.  Argument must be a string instead of a const char*.
    auto enc2 = rle(vector<ll>{5, 0, 0, 0, -2, -2});  // enc2 == vector<pair<ll, int>>{{5, 1}, {0, 3}, {-2, 2}}
    // (2)
    vector<ll> vec{3, 1, 1, 1, 1, 9, 9};
    vector<pair<ll, int>> result;
    rle_iter(vec.begin(), vec.end(), back_inserter(result));  // result == {{3, 1}, {1, 4}, {9, 2}}

 */

//////////////////////////////////////////////////////////////////////
// See help of libins command for dependency spec syntax.
// @@ !! BEGIN()    ---- rle.cc

template<class InputIt, class OutputIt>
OutputIt rle_iter(InputIt first, InputIt last, OutputIt d_first) {
  InputIt itA = first;
  OutputIt oit = d_first;
  while (itA != last) {
    InputIt itB = next(itA);
    for (; itB != last and *itA == *itB; itB++);
    *oit = make_pair(*itA, itB - itA);
    itA = itB;
    oit++;
  }
  return oit;
}

template<class V>
auto rle(V vec) {
  vector<pair<typename V::value_type, int>> ret;
  rle_iter(vec.begin(), vec.end(), back_inserter(ret));
  return ret;
}

// @@ !! END ---- rle.cc
