#include <iostream>
#include <stack>
#include <cmath>
using namespace std;

stack<int> peg[3];
char name[3] = {'A', 'B', 'C'};

// Makes the only legal move between two pegs
void moveBetween(int a, int b) {
  if (peg[a].empty() || (!peg[b].empty() && peg[b].top() < peg[a].top())) swap(a, b);
  int disk = peg[a].top();
  peg[a].pop();
  peg[b].push(disk);
  cout << "Move disk " << disk << " from " << name[a] << " to " << name[b] << "\n";
}

int main() {
  int n;
  cout << "Enter number of disks: ";
  cin >> n;
  for (int d = n; d >= 1; d--) peg[0].push(d);

  int src = 0, aux = 1, dst = 2;
  if (n % 2 == 0) swap(aux, dst);

  long long total = (1LL << n) - 1;
  for (long long i = 1; i <= total; i++) {
    if (i % 3 == 1) moveBetween(src, dst);
    else if (i % 3 == 2) moveBetween(src, aux);
    else moveBetween(aux, dst);
  }
  cout << "Total moves: " << total << "\n";
  return 0;
}

/*
  Complexity:
  The loop makes exactly 2^n - 1 moves and each move is O(1), so time is O(2^n).
  Best and worst case are the same, since 2^n - 1 is the minimum number of moves.
  Space: O(n) for the three stacks holding n disks.
*/
