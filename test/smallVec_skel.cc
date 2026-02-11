#include <bits/stdc++.h>
#include <cassert>
using namespace std;
using ll = long long int;
using pll = pair<ll, ll>;
using u64 = unsigned long long;

// @@ !! LIM(smallVec debug)

int main() {

  {
    using myvec = small_vector_u64<4>;
    myvec vec1;
    for (ll i = 0; i < 16; i++) vec1[i] = i;
    for (ll i = 0; i < 16; i++) {
      assert(vec1[i] == i);
    }
    stringstream ss1;
    ss1 << vec1;
    vector<ll> vll1(16);
    for (ll i = 0; i < 16; i++) vll1[i] = i;
    stringstream ss2;
    ss2 << vll1;
    assert(ss1.str() == ss2.str());

    myvec vec2, vec3, vec4;
    vec2[1] = 13;
    vec3[1] = 15;
    vec4[1] = 13;
    safe_umap<myvec, ll> mp;
    mp[vec1] = 100;
    mp[vec2] = 200;
    mp[vec3] = 300;
    auto it4 = mp.find(vec4);
    assert(it4 != mp.end() and it4->second == 200);
    vec4[1] = 7;
    it4 = mp.find(vec4);
    assert(it4 == mp.end());
    vec2[1] = vec4[1];
    assert(vec2[1] == 7);
    vec2[1] = vec2[2] = vec2[3] = 2;
    assert(vec2[1] == vec2[2] and vec2[2] == vec2[3] and vec2[3] == 2);
    
    myvec vec5{3, 6, 9};
    assert(vec5[0] == 3 and vec5[1] == 6 and vec5[2] == 9);
    myvec vec5a{3LL, 6LL, 9LL};
    assert(vec5a[0] == 3 and vec5a[1] == 6 and vec5a[2] == 9);
    myvec vec6(vector{2, 5, 8, 11}, 3);
    assert(vec6[0] == 2 and vec6[1] == 5 and vec6[2] == 8);
    myvec vec6a(vector<ll>{2, 5, 8, 11}, 3);
    assert(vec6a[0] == 2 and vec6a[1] == 5 and vec6a[2] == 8);

    myvec vec7;
    vec7 = vec5;
    myvec vec8(vec5);
    assert(vec7 == vec5 and vec8 == vec5);

  }

  {
    using myvec = small_vector_u64<5>;
    myvec vec1;
    for (ll i = 0; i< 12; i++) vec1[i] = i;
    stringstream ss1;
    ss1 << vec1;
    vector<ll> vll1(12);
    for (ll i = 0; i < 12; i++) vll1[i] = i;
    stringstream ss2;
    ss2 << vll1;
    assert(ss1.str() == ss2.str());
  }

  {
    small_vector_string vec1(5);
    vec1[1] = 15;
    vec1[3] = 20;
    vec1[2] = vec1[1] + vec1[3];
    stringstream ss1;
    ss1 << vec1;
    assert(ss1.str() == "[0, 15, 35, 20, 0]");
    stringstream ss2;
    ss2 << vec1[3];
    assert(ss2.str() == "20");

    small_vector_string vec2(2);
    vec2[1] = 13;

    small_vector_string vec3(3);
    vec3[1] = 13;

    small_vector_string vec4(2);
    vec4[1] = 5 + 8;

    safe_umap<small_vector_string, ll> mp;
    mp[vec1] = 100;
    mp[vec2] = 200;
    mp[vec3] = 300;
    auto it = mp.find(vec4);
    assert(it != mp.end() and it-> second == 200);
    vec4[0] = 5;
    it = mp.find(vec4);
    assert(it == mp.end());
    vec4[0] = vec2[1];
    assert(vec4[0] == 13);
    vec3[0] = vec3[1] = vec3[2] = 2;
    assert(vec3[0] == 2 and vec3[1] == 2 and vec3[2] == 2);

    small_vector_string vec5{3, 6, 9};
    assert(vec5[0] == 3 and vec5[1] == 6 and vec5[2] == 9);
    small_vector_string vec5a{3LL, 6LL, 9LL};
    assert(vec5a[0] == 3 and vec5a[1] == 6 and vec5a[2] == 9);
    small_vector_string vec6(vector{2, 5, 8, 11}, 3);
    assert(vec6[0] == 2 and vec6[1] == 5 and vec6[2] == 8);
    small_vector_string vec6a(vector<ll>{2, 5, 8, 11}, 3);
    assert(vec6a[0] == 2 and vec6a[1] == 5 and vec6a[2] == 8);

    small_vector_string vec7;
    vec7 = vec5;
    small_vector_string vec8(vec5);
    assert(vec7 == vec5 and vec8 == vec5);

  }


  cerr << "ok." << endl;
}

