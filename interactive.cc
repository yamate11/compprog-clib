#include <bits/stdc++.h>
#include <cassert>
typedef long long int ll;
using namespace std;

/*
  NOTE: This file is OBSOLETE.
  Consult https://yamate11.github.io/blog/posts/2023/04-21-interactive-debug/

  OBSOLETE OBSOLETE
  interactive.cc -- skelton for interactive question

  (0) Remove the main function from the ordinary template.
  (1) Copy the part just before the LIM end mark of this file into the
      effective part.
  (2) Edit the following
    (2-1) Choose either 0 or 1 for preprocessor macro ASK_FIRST.
          If your program asks first to the judge and receives the answer,
          then ASK_FIRST = 1.  In this case, you implement Judge.ask().
          Otherwise, i.e., if the judge asks your 
          program something first and your program then responds, then
          ASK_FIRST = 0.  In this case, you implement Judge.ask_q() and
          Judge.ask_a().
    (2-2) three "using" definitions in abstract class judge
          They are used in the signatures of functions listed in (2-2).
          These have nothing to do with real interfaces to the external
          judge program, but just for internal interface within this program,
          so most probably you can leave them as-is.
    (2-3) For each of JudgeLoc and JudgeRem,
       (2-3-a) constructor
       (2-3-b) either ask() or the pair of ask_q() and ask_a()
       (2-3-c) answer()
          Note that JudgeRem is a must.  This is for the true program.
          JudgeLoc is for local testing.  Thus, if you do not use them,
          it can be left (almost) as-is.
    (2-4) Function game()
          This is where you need to write the algorithm, using
          Judge.ask*() and Judge.answer().
  (3) Testing
    (3-1) ./cans
       This is the true program.  You can manually input data into its
       stdin.
    (3-2) ./cans judge < din.txt
       One-shot testing.  The format of din.txt is not defined by the
       writer of the question.  You can freely set the spec for the input
       file so that JudgeLoc() can read the input.
    (3-3) cmpNaive -i -e -p './cans judge'
       cmpNaive testing.  Runs multiple times against test files
       generated by `gen' script.  A test run is regarded as a success
       if its exit code is zero.

 */

// @@ !! LIM()

//////////////////////////////////////////////////////////////////////
// See help of libins command for dependency spec syntax.
// @@ !! BEGIN() ---- interactive.cc

const string end_mark("^__=end=__^");

struct SolveExc : exception {};

void do_game(bool is_judge);

int main(int argc, char *argv[]) {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << setprecision(20);

  bool is_judge = false;
  if (argc == 2) {
    if (strcmp(argv[1], "judge") == 0) {
      is_judge = true;
    }else if (strcmp(argv[1], "skip") == 0) {
      return 0;
    }else {
      cerr << "Unknown argument\n";
      exit(1);
    }
  }
    
  try {
    do_game(is_judge);
  }catch (const SolveExc& e) {
  }
  return 0;
}

/* 
   Copy and edit the following into the part below END
 */
/*
#define ASK_FIRST 1

ll sz;

struct Judge {
  using ask_q_t = ll;
  using ask_a_t = ll;
  using answer_t = ask_q_t;

#if ASK_FIRST
  virtual ask_a_t ask(ask_q_t x) = 0;
#else
  virtual void ask_q(ask_q_t x) = 0;
  virtual ask_a_t ask_a() = 0;
#endif
  virtual void answer(answer_t x) = 0;

  // Call ac() in JudgeLoc when the answer is correct.
  void ac() {
    cout << "AC\n";
    throw SolveExc();
  }

  // Call wa() in JudgeLoc when the answer is not correct.
  void wa() {
    cout << "WA\n";
    exit(1);
  }
};

// This is a class for local testing.
struct JudgeLoc : Judge {
  ll expected;    // Typically, expected answer is read in the beginning
                  // from the question-specifying file.

  JudgeLoc() {
    // Typically, read the question for testing from stdin
    cin >> sz >> expected;
  }

#if ASK_FIRST
  // Returns the answer for a query x.
  ask_a_t ask(ask_q_t x) {
    // An example for Guessing Number Game.
    if (x < expected)      return 1;
    else if (x > expected) return -1;
    else                   return 0;
  }
#else
  // Send a request for the user program to process data
  ask_a_t ask_a() {
    // An example for Stone Taking Game (1-3)
    ll e = rand() % max(sz, 3) + 1;
    sz -= e;
    return e;
  }
  // Receives a reply from the user program 
  void ask_q(ask_q_t x) {
    // An example for Stone Taking Game (1-3)
    if (x <= 0 || 3 <= x || sz < x) wa();
    sz -= x;
  }
#endif

  // Always call ac() or wa().
  void answer(answer_t x) {
    if (x == expected) ac();
    else wa();
  }
};

// The class that implements interfaces to the external judge program.
struct JudgeRem : Judge {
  // Read initial information, if any.
  JudgeRem() {
    cin >> sz;
  }
#if ASK_FIRST
  // Asks for and receives information
  // Typically prepended by "? " and followed by endl
  ask_a_t ask(ask_q_t x) {
    cout << "? " << x << endl;
    ask_a_t ret; cin >> ret;
    return ret;
  }
#else
  // Reads a request from the external judge program.
  ask_a_t ask_a() {
    ask_a_t rep; cin >> rep;
    return rep;
  }
  // Writes a reply to the external judge program.
  void ask_q(ask_q_t x) {
    cout << "? " << x << endl;
  }
#endif

  // Final output.  Typically prepended by "! " and followed by endl
  void answer(answer_t x) {
    cout << "! " << x << endl;
    // You need to throw SolveExc() at the end of the function.
    throw SolveExc();
  }

};

// You write your algorithm using judge.ask()
// (or judge.ask_a() and judge.ask_q()) and judge.answer().
void game(Judge&& judge) {
  // example for Guessing Number Game
  ll left = 1;
  ll right = sz;
  while (true) {
    ll mid = (left + right) / 2;
    ll reply = judge.ask(mid);
    if (reply == 0) judge.answer(mid);
    if (reply == -1) right = mid - 1;
    if (reply == 1)  left  = mid + 1;
  }
}

// You should not edit this function.
void do_game(bool is_judge) {
  if (is_judge) game(JudgeLoc());
  else          game(JudgeRem());
}
*/

// @@ !! END ---- interactive.cc

#define ASK_FIRST 1

ll sz;

struct Judge {
  using ask_q_t = ll;
  using ask_a_t = ll;
  using answer_t = ask_q_t;

#if ASK_FIRST
  virtual ask_a_t ask(ask_q_t x) = 0;
#else
  virtual void ask_q(ask_q_t x) = 0;
  virtual ask_a_t ask_a() = 0;
#endif
  virtual void answer(answer_t x) = 0;

  // Call ac() in JudgeLoc when the answer is correct.
  void ac() {
    cout << "AC\n";
    throw SolveExc();
  }

  // Call wa() in JudgeLoc when the answer is not correct.
  void wa() {
    cout << "WA\n";
    exit(1);
  }
};

// This is a class for local testing.
struct JudgeLoc : Judge {
  ll expected;    // Typically, expected answer is read in the beginning
                  // from the question-specifying file.

  JudgeLoc() {
    // Typically, read the question for testing from stdin
    cin >> sz >> expected;
  }

#if ASK_FIRST
  // Returns the answer for a query x.
  ask_a_t ask(ask_q_t x) {
    // An example for Guessing Number Game.
    if (x < expected)      return 1;
    else if (x > expected) return -1;
    else                   return 0;
  }
#else
  // Send a request for the user program to process data
  ask_a_t ask_a() {
    // An example for Stone Taking Game (1-3)
    ll e = rand() % max(sz, 3) + 1;
    sz -= e;
    return e;
  }
  // Receives a reply from the user program 
  void ask_q(ask_q_t x) {
    // An example for Stone Taking Game (1-3)
    if (x <= 0 || 3 <= x || sz < x) wa();
    sz -= x;
  }
#endif

  // Always call ac() or wa().
  void answer(answer_t x) {
    if (x == expected) ac();
    else wa();
  }
};

// The class that implements interfaces to the external judge program.
struct JudgeRem : Judge {
  // Read initial information, if any.
  JudgeRem() {
    cin >> sz;
  }
#if ASK_FIRST
  // Asks for and receives information
  // Typically prepended by "? " and followed by endl
  ask_a_t ask(ask_q_t x) {
    cout << "? " << x << endl;
    ask_a_t ret; cin >> ret;
    return ret;
  }
#else
  // Reads a request from the external judge program.
  ask_a_t ask_a() {
    ask_a_t rep; cin >> rep;
    return rep;
  }
  // Writes a reply to the external judge program.
  void ask_q(ask_q_t x) {
    cout << "? " << x << endl;
  }
#endif

  // Final output.  Typically prepended by "! " and followed by endl
  void answer(answer_t x) {
    cout << "! " << x << endl;
    // You need to throw SolveExc() at the end of the function.
    throw SolveExc();
  }

};

// You write your algorithm using judge.ask()
// (or judge.ask_a() and judge.ask_q()) and judge.answer().
void game(Judge&& judge) {
  // example for Guessing Number Game
  ll left = 1;
  ll right = sz;
  while (true) {
    ll mid = (left + right) / 2;
    ll reply = judge.ask(mid);
    if (reply == 0) judge.answer(mid);
    if (reply == -1) right = mid - 1;
    if (reply == 1)  left  = mid + 1;
  }
}

// You should not edit this function.
void do_game(bool is_judge) {
  if (is_judge) game(JudgeLoc());
  else          game(JudgeRem());
}
