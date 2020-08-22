#include <bits/stdc++.h>

// dp[i] := 足場iにたどり着くまでに支払うコストの最小値

// 足場Nにたどり着くまでに支払うコストの最小値
// dp[N] = min dp[N-1] + abs(h[N] - h[N-1]),
//             dp[N-2] + abs(h[N] - h[N-2])

using namespace std;
// 足場iにたどり着くまでに支払うコストの最小値
#define MAX_N 100001
int memo[MAX_N];
int h[MAX_N];
int solve(int i) {
  int ret = -1;
  if (memo[i] != -1) return memo[i];
  if (i <= 1) return 0; // 足場1にたどり着く最小値
  if (i >= 3) ret = min(solve(i-1) + abs(h[i] - h[i-1]), solve(i-2) + abs(h[i] - h[i-2]));
  else ret = solve(i-1) + abs(h[i] - h[i-1]);
  return memo[i] = ret;
}
int main() {
  int n;
  cin >> n;
  memset(memo, -1, sizeof(memo));
  for (int i = 1; i <= n; i++) {
    cin >> h[i];
  }
  cout << solve(n) << endl;
  return 0;
}
