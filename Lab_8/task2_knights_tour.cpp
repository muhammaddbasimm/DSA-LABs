#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
using namespace std;

int N;
vector<vector<int>> board;
int dx[8] = {2, 1, -1, -2, -2, -1, 1, 2};
int dy[8] = {1, 2, 2, 1, -1, -2, -2, -1};

bool valid(int x, int y) {
  return x >= 0 && y >= 0 && x < N && y < N && board[x][y] == -1;
}

int degree(int x, int y) {
  int c = 0;
  for (int k = 0; k < 8; k++)
    if (valid(x + dx[k], y + dy[k])) c++;
  return c;
}

// Backtracking with Warnsdorff ordering: try the move with fewest onward moves first
bool solve(int x, int y, int step) {
  if (step == N * N) return true;
  vector<pair<int, int>> moves;
  for (int k = 0; k < 8; k++) {
    int nx = x + dx[k], ny = y + dy[k];
    if (valid(nx, ny)) moves.push_back({degree(nx, ny), k});
  }
  sort(moves.begin(), moves.end());

  for (auto& mv : moves) {
    int nx = x + dx[mv.second], ny = y + dy[mv.second];
    board[nx][ny] = step;
    if (solve(nx, ny, step + 1)) return true;
    board[nx][ny] = -1;
  }
  return false;
}

int main() {
  int x, y;
  cout << "Enter board size N: ";
  cin >> N;
  cout << "Enter starting position (x y): ";
  cin >> x >> y;

  board.assign(N, vector<int>(N, -1));
  board[x][y] = 0;
  if (!solve(x, y, 1)) {
    cout << "No knight's tour exists from this position.\n";
    return 0;
  }
  cout << "\nKnight's tour (move numbers):\n";
  for (auto& row : board) {
    for (int v : row) cout << setw(4) << v;
    cout << "\n";
  }
  return 0;
}

/*
  Complexity:
  Plain backtracking worst case: O(8^(N^2)), since each square can branch into 8 moves.
  With Warnsdorff ordering the first path almost never needs to backtrack, so each of the
  N^2 squares is visited once with constant (8) work: best/typical case O(N^2).
  Worst case stays exponential in theory, but it is not reached in practice. Space: O(N^2).
*/
