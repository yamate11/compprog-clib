#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;

#define CMPR_BEGIN() cerr << "{{{{ " << __FILE__ << ":" << __LINE__ << "\n"
#define CMPR_SEP() cerr << "~~~~\n"
#define CMPR_END() cerr << "}}}}\n\n"
#define CMPR(EXPECTED, TEST) CMPR_BEGIN(); (TEST); CMPR_SEP(); cerr << (EXPECTED) << "\n"; CMPR_END()

// @@ !! LIM(debug)


// check if it is ok to define operator<<() for vector
template<typename T>
ostream& operator<<(ostream& ostr, const vector<T>& v) {
  ostr << "{{{";
  for (const T& t : v) ostr << g_show(t) << ", ";
  ostr << "}}}";
  return ostr;
}

int main() {
  vector<ll> x{1, 2, 3};
  vector<string> y{"a", "bc", "def"};
  CMPR("{{{1, 2, 3, }}}", cerr << x << endl);
  CMPR("{{{a, bc, def, }}}", cerr << y << endl);
  CMPR("x=[1, 2, 3]", DLOGK(x));
  CMPR("y=[a, bc, def]", DLOGK(y));

  return 0;
}
