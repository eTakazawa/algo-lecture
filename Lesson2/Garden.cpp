#include <iostream>
#include <vector>
using namespace std;

// pより西の花の総数がK以上であるか判定する関数
// 最大でcount=N*p(N<=1e5, p<=2e18) なのでオーバーフロー対策が必要
bool check(long long p, long long K, const vector<long long>& w, const vector<long long>& d) {
  int N = (int)w.size();
  long long count = 0;
  // 各花iのpより西にある数を求める
  for (int i = 0; i < N; i++) {
    if (p >= w[i]) {
      count += (p - w[i]) / d[i] + 1; // w[i]から，d[i]間隔であるので，d[i]で割れば良い
      if (count >= K) return true;    // オーバーフロー対策
    }
  }
  return count >= K;
}

int main(void) {
  long long N, K;
  cin >> N >> K;
  vector<long long> w(N), d(N);
  for (int i = 0; i < N; i++) {
    cin >> w[i] >> d[i];
  }

  long long ng = 0;
  long long ok = 2e18; // ok側の最大値を見積もる
                       // 1つの花が10^18の位置から10^9間隔であると
                       // K=10^9番目の位置は，10^18 + (10^9 * 10^9)となる → 2e18
  // 二分探索
  while (abs(ok - ng) > 1) {
    long long mid = (ok + ng) / 2;
    if (check(mid, K, w, d)) {
      ok = mid;  // Kを超えていればok側を縮める
    }
    else {
      ng = mid;
    }
  }
  cout << ok << endl;
  return 0;
}