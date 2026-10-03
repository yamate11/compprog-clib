#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;

/*
  *NOTE* This implementation is slow O(N(logN)^2).
         Use ac-library O(N) instead.

  Suffix Array

    SuffixArray sa("abcabc");         // let N = "abcabc".size()
    const auto& arr = sa.arr;         // vector<int>, size N+1
    const auto& rank = sa.rank;       // vector<int>, size N+1
    const auto& lcp = sa.getLcp();    // vector<int>, size N

  string:
    0123456
    abcabc    

  sorted suffixes

    i  arr[i]  str[arr[i]:]  lcp[i]
    0  6       (empty)       0
    1  3       abc           3
    2  0       abcabc        0
    3  4       bc            2
    4  1       bcabc         0
    5  5       c             1
    6  2       cabc          ---

    arr  ==> [6, 3, 0, 4, 1, 5, 2]
    rank ==> [2, 4, 6, 1, 3, 5, 0]
         arr[rank[i]] = i,  rank[arr[x]] = x
         str[i:] < str[j:] <=> rank[i] < rank[j]

    lcp[i] is the length of common prefix of str[arr[i]:] and str[arr[i+1]:]

    Note: for i < j, the length of common prefix of str[arr[i]:] and
          str[arr[j]:] is min { lcp[i], ..., lcp[j-1] }
 */


// @@ !! LIM()


//////////////////////////////////////////////////////////////////////
// See help of libins command for dependency spec syntax.
// @@ !! BEGIN() ---- suffixArray.cc

struct SuffixArray {
private:
  string str;

  /* arr and rank is automatically built, but lcp is not.
     Therefore, we make lcp private so that the user does not accidentally
     access unbuilt empty vector.
   */
  vector<ll> lcp;  // lcp.size() == str.size()
  bool lcp_built;

public:
  vector<ll> arr;  // arr.size() == str.size() + 1
  vector<ll> rank; // rank.size() == str.size() + 1

  void init(string s) {
    str = s;
    lcp_built = false;
    ll N = str.size();
    arr.resize(N + 1);
    rank.resize(N + 1);
    if (N == 0) return;
    if (N == 1) {
      arr.at(0) = rank.at(0) = 1;
      return;
    }
    copy(str.begin(), str.end(), rank.begin());
    rank.at(N) = 0;
    for (ll k = 1; k < N; k *= 2) {
      iota(arr.begin(), arr.end(), 0);
      auto compare = [&](ll p, ll q) -> bool {
	if (p == q) return false;
	if (rank.at(p) != rank.at(q)) return rank.at(p) < rank.at(q);
	if (p + k > N) return true;
	if (q + k > N) return false;
	return rank.at(p + k) < rank.at(q + k);
      };
      sort(arr.begin(), arr.end(), compare);
      vector<ll> tmp(N + 1);
      tmp.at(arr.at(0)) = 0;
      for (ll i = 1; i <= N; i++) {
	ll prev = arr.at(i-1);
	ll cur = arr.at(i);
	tmp.at(cur) = tmp.at(prev) + (compare(prev, cur) ? 1 : 0);
      }
      rank = move(tmp);
    }
  }

  SuffixArray(string s) { init(s); }

  ll at(ll i) { return arr.at(i); }

  const vector<ll>& getLcp() {
    if (lcp_built) return lcp;
    lcp_built = true;
    ll N = str.size();
    lcp.resize(N);
    if (N <= 1)  return lcp;
    ll h = 0;
    for (ll i = 0; i < N; i++) {
      ll ri = rank.at(i);
      assert(ri - 1 >= 0);
      ll j = arr.at(ri - 1);
      if (h > 0) h--;
      while (j + h < N && i + h < N && str.at(j + h) == str.at(i + h)) h++;
      lcp.at(ri - 1) = h;
    }
    return lcp;
  }

};

// @@ !! END ---- suffixArray.cc

