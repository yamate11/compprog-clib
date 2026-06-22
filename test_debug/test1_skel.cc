#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;

#define CMPR_BEGIN() cerr << "{{{{ " << __FILE__ << ":" << __LINE__ << "\n"
#define CMPR_SEP() cerr << "~~~~\n"
#define CMPR_END() cerr << "}}}}\n\n"
#define CMPR(EXPECTED, TEST) CMPR_BEGIN(); (TEST); CMPR_SEP(); cerr << (EXPECTED) << "\n"; CMPR_END()

// @@ !! LIM(debug)

// @DefStruct(sta, (a, (b, ll), (c, vector<ll>), (d, string))) [CR2AMSQY]
struct sta {
  ll a;
  ll b;
  vector<ll> c;
  string d;
  string show() const {
    return "(" + to_string(a) + ", " + to_string(b) + ", " + g_show(c) + ", " + d + ")";
  }
  bool operator==(const sta&) const = default;
};
// @End [CR2AMSQY]

struct S_hand_0 { // Defines operator<<() only
  int a;
  friend ostream& operator<<(ostream& ostr, const S_hand_0& x) { return ostr << "{{" << x.a << "}}"; }
};

struct S_hand_1 { // Defines both show() and operator<<()
  int a;
  string show() const { return "<<" + to_string(a) + ">>"; }
  friend ostream& operator<<(ostream& ostr, const S_hand_1& x) { return ostr << "[[" << x.a << "]]"; }
};

int main() {
  {
    ll a = 10;
    CMPR("10", DLOG(a));
    int b = -10;
    CMPR("label b=-10", DLOGKL("label", b));
    string c = "hello";
    string c1 = g_show(c);
    CMPR_BEGIN();
    DLOGK(c);
    DLOGK(c1);
    CMPR_SEP();
    cerr << "c=hello\nc1=hello\n";
    CMPR_END();
    double d = 1.0 / 3.0;
    CMPR("0.333333", DLOG(d));
    CMPR("a=10 b=-10 c=hello d=0.333333", DLOGK(a, b, c, d));
    bool e = false;
    CMPR("false", DLOG(e));
    char f = 'A';
    CMPR("A", DLOG(f));
  }
  {
    ll a = 10;
    ll b = 20;
    CMPR("10", DLOG(a));
    CMPR("10 20", DLOG(a, b));
    CMPR_BEGIN();
    DLOGNNL(a);
    DLOG(b);
    CMPR_SEP();
    cerr << "1020\n";
    CMPR_END();
    ll c = 30;
    auto f = [&](ll& x) -> void { x += 1; };
    DCALL(f, c);
    assert(c == 31);
    CMPR("a=10", DLOGK(a));
    CMPR("a=10 b=20", DLOGK(a, b));
    CMPR("label a=10 b=20", DLOGKL("label", a, b));
  }
  {
    vector<ll> v1{0, 10, 20};
    CMPR("[0, 10, 20]", DLOG(v1));
    pair<int, string> p(5, "abc");
    CMPR("(5, abc)", DLOG(p));
    tuple<int, int, int, string> t(1, 2, 3, "four");
    CMPR("(1, 2, 3, four)", DLOG(t));
    set<ll> s1{-10, 0, 10};
    CMPR("[-10, 0, 10]", DLOG(s1));
    vector<vector<ll>> v2{{1}, {2, 3, 4}, {}, {5, 6}};
    CMPR("[[1], [2, 3, 4], [], [5, 6]]", DLOG(v2));
    unordered_map<ll, map<string, bool>> mp1;
    mp1[3]["aaa"] = false;
    mp1[3]["bbb"] = true;
    mp1[5]["ccc"] = true;
    mp1[7] = {};
    CMPR("[(7: []), (5: [(ccc: true)]), (3: [(aaa: false), (bbb: true)])]", DLOG(mp1));
    // The correctness of the line above of course depends on the hash function.  Edit it accordingly.
    using pll = pair<ll, ll>;
    priority_queue<pll, vector<pll>, greater<pll>> pque;
    pque.emplace(0, 0);
    pque.emplace(30, 1);
    pque.emplace(4, 2);
    CMPR("pque=[(0, 0), (4, 2), (30, 1)]", DLOGK(pque));
    pque.emplace(7, 3);
    CMPR("pque=[(0, 0), (4, 2), (7, 3), (30, 1)]", DLOGK(pque));
    pque.pop();
    CMPR("pque=[(4, 2), (7, 3), (30, 1)]", DLOGK(pque));
  }
  {
    sta x(0, 10, vector<ll>{20, 30, 40}, "bye");
    CMPR("(0, 10, [20, 30, 40], bye)", DLOG(x));
    S_hand_0 z{17};
    CMPR("{{17}}", DLOG(z));
    CMPR("{{17}}", cerr << z << endl);
    S_hand_1 y{93};
    CMPR("<<93>>", DLOG(y));
    CMPR("[[93]]", cerr << y << endl);
  }

}
