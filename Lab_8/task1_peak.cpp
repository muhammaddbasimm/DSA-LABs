#include <iostream>
#include <vector>
using namespace std;

// Finds a 2D peak using binary search on columns: O(n log n) for an n x n grid
pair<int, int> findPeak(const vector<vector<int>>& a) {
  int rows = a.size(), cols = a[0].size();
  int lo = 0, hi = cols - 1;
  while (lo <= hi) {
    int mid = (lo + hi) / 2;
    int maxRow = 0;
    for (int i = 1; i < rows; i++)
      if (a[i][mid] > a[maxRow][mid]) maxRow = i;

    int left = mid > 0 ? a[maxRow][mid - 1] : -1e9;
    int right = mid < cols - 1 ? a[maxRow][mid + 1] : -1e9;

    if (a[maxRow][mid] >= left && a[maxRow][mid] >= right) return {maxRow, mid};
    if (left > a[maxRow][mid]) hi = mid - 1;
    else lo = mid + 1;
  }
  return {-1, -1};
}

int main() {
  int n, m;
  cout << "Enter rows and columns: ";
  cin >> n >> m;
  vector<vector<int>> a(n, vector<int>(m));
  cout << "Enter the matrix:\n";
  for (auto& row : a)
    for (int& x : row) cin >> x;

  pair<int, int> p = findPeak(a);
  cout << "Peak element: " << a[p.first][p.second]
       << " at (" << p.first << ", " << p.second << ")\n";
  return 0;
}

/*
  Complexity: binary search runs log(m) times on columns, each step scans n rows,
  so time is O(n log m), which is O(n log n) for an n x n grid. Space: O(1) extra.
*/
