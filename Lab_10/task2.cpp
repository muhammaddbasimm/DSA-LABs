// Task 2: IDE Undo/Redo Macro Synthesizer (Stack + Backtracking)
#include <iostream>
#include <string>
using namespace std;

// ---------- Operation and a hand-made array Stack ----------
struct Operation {
  char type;  // '+', '-', '*', '/'
  int value;
};

const int MAXK = 20;
class OpStack {
  Operation arr[MAXK];
  int topIdx;
public:
  OpStack() : topIdx(-1) {}
  bool isEmpty() { return topIdx == -1; }
  int size() { return topIdx + 1; }
  void push(Operation op) { arr[++topIdx] = op; }
  Operation pop() { return arr[topIdx--]; }
  Operation at(int i) { return arr[i]; }  // read bottom to top for printing
};

// Apply op to s. Returns false if the op is not allowed (division with remainder).
bool apply(int s, Operation op, int& result) {
  switch (op.type) {
    case '+': result = s + op.value; return true;
    case '-': result = s - op.value; return true;
    case '*': result = s * op.value; return true;
    case '/':
      if (s % op.value != 0) return false;  // only exact division
      result = s / op.value; return true;
  }
  return false;
}

Operation ops[] = {{'+', 3}, {'*', 2}, {'-', 5}, {'/', 2}};
int nOps = 4;
int S_start, S_target, K, M_max;
int solutions = 0;
OpStack st;

void printSequence() {
  solutions++;
  int s = S_start;
  cout << "  #" << solutions << " (" << st.size() << " ops): " << s;
  for (int i = 0; i < st.size(); i++) {
    Operation op = st.at(i);
    apply(s, op, s);
    cout << " " << op.type << op.value << "=" << s;
  }
  cout << "\n";
}

void synthesize(int state) {
  if (state == S_target && !st.isEmpty()) {  // reached target: record macro
    printSequence();
  }
  if (st.size() == K) return;               // budget used up
  for (int i = 0; i < nOps; i++) {
    int next;
    if (!apply(state, ops[i], next)) continue;
    if (next < 0 || next > M_max) continue;  // prune: invalid intermediate state
    st.push(ops[i]);                          // do
    synthesize(next);
    st.pop();                                 // undo (backtrack)
  }
}

void runCase(int start, int target, int k, int mmax) {
  S_start = start; S_target = target; K = k; M_max = mmax;
  solutions = 0;
  cout << "==================================================\n";
  cout << "S_start = " << S_start << ", S_target = " << S_target
       << ", K = " << K << ", M_max = " << M_max << "\n";
  cout << "Allowed ops: +3, *2, -5, /2 (exact only)\n";
  cout << "==================================================\n";
  synthesize(S_start);
  if (solutions == 0) cout << "  No valid macro sequence exists.\n";
  cout << "Total sequences found: " << solutions << "\n";
  cout << "Stack empty after search: " << (st.isEmpty() ? "yes" : "no") << "\n\n";
}

int main() {
  runCase(5, 16, 4, 50);
  runCase(4, 1, 3, 20);
  runCase(7, 100, 3, 60);
  return 0;
}
