#include <bits/stdc++.h>
#include <cassert>
using namespace std;
using ll = long long int;
using pll = pair<ll, ll>;
using ull = unsigned long long;

// @@ !! LIM(digit)

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << setprecision(20);

  {
    digit_util du;
    assert(du.pow_size() == 19);
    assert(du.pow(0) == 1);
    assert(du.pow(1) == 10);
    assert(du.pow(6) == 1000000);
    assert(du.pow(18) == 1000000000000000000);
    assert(du.width(0, true) == 0);
    assert(du.width(1) == 1);
    assert(du.width(2) == 1);
    assert(du.width(9) == 1);
    assert(du.width(10) == 2);
    assert(du.width(99) == 2);
    assert(du.width(100) == 3);
    assert(du.nd_min(1) == 1);
    assert(du.nd_min(2) == 10);
    assert(du.nd_min(3) == 100);
    assert(du.nd_max(1) == 9);
    assert(du.nd_max(2) == 99);
    assert(du.nd_max(3) == 999);
    assert(du.width(123456789012345678) == 18);
    assert(du.width(1000000000000000000) == 19);
    assert(du.floor(0, true) == 0);
    assert(du.floor(123) == 100);
    assert(du.floor(1000) == 1000);
    assert(du.ceil(0, true) == 0);
    assert(du.ceil(123) == 1000);
    assert(du.ceil(1000) == 1000);
    assert(du.d_at(1234, 0) == 4);
    assert(du.d_at(1234, 1) == 3);
    assert(du.d_at(1234, 2) == 2);
    assert(du.d_at(1234, 3) == 1);

    assert(du.d_at( 94283,  1)       ==    8);
    assert(du.d_at( 94283, -2)       ==    4);
    assert(du.d_sub(94283,  1,  3)   ==  428);
    assert(du.d_sub(94283,  1, -2)   ==   83);
    assert(du.d_sub(94283, -2,  2)   ==   94);
    assert(du.d_sub(94283, -2, -4)   == 4283);
    assert(du.d_sub(94283,  1,  100) == 9428);
    assert(du.d_sub(94283,  1, -100) ==   83);
    
    assert(du.to_string(0) == "0");
    assert(du.to_string(1) == "1");
    assert(du.to_string(123) == "123");
    assert(du.to_string_padding(1234, 8) == "00001234");
    assert(du.to_string_padding(1234, 2) == "1234");
    assert(du.to_string_padding(1234, 8, '~') == "~~~~1234");
    assert(du.from_string("0") == 0);
    assert(du.from_string("230") == 230);
    assert(du.from_string("00230") == 230);
    assert(du.to_vector(0) == vector<ll>{0});
    assert(du.to_vector(1) == vector<ll>{1});
    assert(du.to_vector(123) == (vector<ll>{3, 2, 1}));
    assert(du.to_vector_big_endian(123) == (vector<ll>{1, 2, 3}));
    assert(du.from_vector(vector<ll>{0}) == 0);
    assert(du.from_vector((vector<ll>{3, 0, 2})) == 203);
    assert(du.from_vector((vector<ll>{0, 0, 2, 3, 0})) == 3200);
    assert(du.from_vector((vector<ll>{0, 0, 2, 3, 0})) == 3200);
    assert(du.from_vector_big_endian((vector<ll>{0, 0, 2, 3, 0})) == 230);
    
    assert(du.from_vector((vector<int>{3, 0, 2})) == 203);
    assert(du.from_vector_big_endian((deque<int>{3, 0, 2})) == 302);

    assert(du.pow(18) == du.pow(10) * du.pow(8));
    try { du.pow(19); assert(0); } catch(digit_error& e) {}
    assert(du.nd_min(19) == du.pow(10) * du.pow(8));
    try { du.nd_max(19); assert(0); } catch(digit_error& e) {}

  }

  {
    digit_util du(2);
    assert(du.pow(0) == 1);
    assert(du.pow(4) == 16);
    assert(du.width(0) == 0);
    assert(du.width(1) == 1);
    assert(du.width(2) == 2);
    assert(du.width(3) == 2);
    assert(du.width(4) == 3);
    assert(du.floor(0, true) == 0);
    assert(du.floor(1) == 1);
    assert(du.floor(3) == 2);
    assert(du.ceil(0, true) == 0);
    assert(du.ceil(1) == 1);
    assert(du.ceil(3) == 4);
    assert(du.to_string(10) == "1010");
    assert(du.to_string_padding(10, 8) == "00001010");
    assert(du.to_string_padding(10, 2) == "1010");
    assert(du.from_string("10101") == 21);
  }


  {
    digit_util du(3);
    assert(du.pow(0) == 1);
    assert(du.pow(4) == 81);
    assert(du.width(80) == 4);
    assert(du.floor(80) == 27);
    assert(du.ceil(80) == 81);
    assert(du.d_at(79, 0) == 1);
    assert(du.d_at(79, 1) == 2);
    assert(du.d_at(79, 3) == 2);
    assert(du.to_string(83) == "10002");
    assert(du.from_string("10002") == 83);
  }

  {
    digit_util du(16);
    assert(du.pow(0) == 1);
    assert(du.pow(4) == 65536);
    assert(du.width(100) == 2);
    assert(du.floor(100) == 16);
    assert(du.ceil(100) == 256);
    assert(du.to_string(254) == "fe");
    assert(du.to_string(254, true) == "FE");
    assert(du.to_string_padding(254, 4) == "00fe");
    assert(du.to_string_padding(254, 4, '!') == "!!fe");
    assert(du.to_string_padding(254, 4, '0', true) == "00FE");
    assert(du.to_string_padding(254, 1, '0', true) == "FE");
    assert(du.from_string("1af") == 256 + 10 * 16 + 15);
    assert(du.from_string("1AF") == 256 + 10 * 16 + 15);
  }

  cerr << "ok\n";

  return 0;
}
