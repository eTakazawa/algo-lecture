#include <iostream>
#include <vector>
#include <cstring>
#include <cassert>

int N;
int a[100002], b[100002], c[100002];

// ボトムアップに解く
int solveBottomUp() {
  // DP
  int dp[100002][3]; // dp[i][j] := i日目にjを選んだ時の幸福度最大値
  memset(dp, 0, sizeof(dp)); // 0で初期化しておく
  
  // 1日目は先に埋めておく
  dp[1][0] = a[1];
  dp[1][1] = b[1];
  dp[1][2] = c[1];

  // DP更新
  for (int i = 2; i <= N; i++) {
    // i日目に0(A)を選んだ時の幸福度最大値
    //  = 前日にB,Cを選んだ時の最大値のmax + i日目にAを選ぶ最大値
    dp[i][0] = std::max(dp[i - 1][1], dp[i - 1][2]) + a[i];

    // i日目に1(B)を選んだ時の幸福度最大値
    dp[i][1] = std::max(dp[i - 1][0], dp[i - 1][2]) + b[i];

    // i日目に2(C)を選んだ時の幸福度最大値
    dp[i][2] = std::max(dp[i - 1][0], dp[i - 1][1]) + c[i];
  }

  // N日目にA,B,Cを選んだ時の最大値の最大値を取る
  int result = std::max(dp[N][0], std::max(dp[N][1], dp[N][2]));

  return result;
}


// トップダウンに解く
int memo[100002][3]; // memo[i][j] := i日目にjを選んだ時の幸福度最大値
int solveRecursive(int n, int action) {
  int &cache = memo[n][action];
  if (cache != -1) return cache;
  if (n == 0) return 0;
  if (action == 0) {
    return cache = std::max(solveRecursive(n-1, 1), solveRecursive(n-1, 2)) + a[n];
  }
  if (action == 1) {
    return cache = std::max(solveRecursive(n-1, 0), solveRecursive(n-1, 2)) + b[n];
  }
  if (action == 2) {
    return cache = std::max(solveRecursive(n-1, 0), solveRecursive(n-1, 1)) + c[n];
  }
  assert(0);
  return -1;
}
int solveTopDown() {
  memset(memo, -1, sizeof(memo)); // -1で初期化しておく
  int result_a = solveRecursive(N, 0);
  int result_b = solveRecursive(N, 1);
  int result_c = solveRecursive(N, 2);
  return std::max(result_a, std::max(result_b, result_c));
}

int main(void) {
  std::cin >> N;
  // 入力の受け取り
  for (int i = 1; i <= N; i++) { // 1-indexedで入れる
    std::cin >> a[i] >> b[i] >> c[i];
  }
  
  std::cout << solveBottomUp() << std::endl;
  // std::cout << solveTopDown() << std::endl;

  return 0;
}