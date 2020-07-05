#include <iostream>
#include <vector>
#include <queue>
#include <cassert>
#include <set>

using namespace std;

enum class Action {
  UP = 0,
  RIGHT,
  DOWN,
  LEFT,
  NUM
};
Action begin(Action) { return Action::UP; }
Action end(Action) { return Action::NUM; }
Action operator*(Action action) { return action; }
Action operator++(Action& action) { return action = Action(underlying_type<Action>::type(action) + 1); }
ostream& operator<<(ostream& os, Action action) {
  switch (action) {
    case Action::UP:   return os << "UP";
    case Action::DOWN:  return os << "DOWN";
    case Action::LEFT:  return os << "LEFT";
    case Action::RIGHT: return os << "RIGHT";
    default: return os;
  }
}

class State {
public:
  State(int width = 3) : width_(width) {
    board_ = vector<vector<int>>(width, vector<int>(width));
    Reset();
  }
  bool CanMove(Action dir) const {
    if (emptyX_ == 0 && dir == Action::LEFT) return false;
    if (emptyX_ == width_ - 1 && dir == Action::RIGHT) return false;
    if (emptyY_ == 0 && dir == Action::UP) return false;
    if (emptyY_ == width_ - 1 && dir == Action::DOWN) return false;
    return true;
  }
  State GetNextState(Action dir) const {
    State nextState(*this);
    if (nextState.CanMove(dir)) nextState.DoMove(dir);
    else nextState.actions_.push_back(dir);
    return nextState;
  }
  void DoMove(Action dir) {
    const static int dx[] = {0, 1, 0, -1};
    const static int dy[] = {-1, 0, 1, 0};
    int nextX = emptyX_;
    int nextY = emptyY_;
    nextX += dx[static_cast<int>(dir)];
    nextY += dy[static_cast<int>(dir)];
    swap(board_[emptyY_][emptyX_], board_[nextY][nextX]);
    emptyX_ = nextX;
    emptyY_ = nextY;
    actions_.push_back(dir);

    assert(board_[emptyY_][emptyX_] == 0);
  }
  void Shuffle(int moveCount = 10000, int seed = -1) {
    if (seed == -1) srand(time(0));
    for (int i = 0; i < moveCount;) {
      Action act = (Action)(rand() % 4);
      if (CanMove(act)) {
        DoMove(act);
        i++;
      }
    }
    ResetActions();
  }
  void Reset() {
    int number = 1;
    for (int y = 0; y < width_; y++) {
      for (int x = 0; x < width_; x++) {
        board_[y][x] = number;
        number++;
      }
    }
    board_[width_ - 1][width_ - 1] = 0;
    emptyX_ = emptyY_ = width_ - 1;

    actions_ = vector<Action>();
  }
  void ResetActions() {
    actions_ = vector<Action>();
  }
  bool IsGoal() const {
    int n = width_ * width_;
    for (int i = 0; i < n - 1; i++) {
      int x = i % width_, y = i / width_;
      if (board_[y][x] != i + 1) return false;
    }
    if (board_[width_ - 1][width_ - 1] != 0) return false;
    return true;
  }
  long long GetHash() const {
    int n = width_ * width_;
    long long hashValue = static_cast<long long>(width_ * width_);
    
    for (int i = 0; i < n; i++) {
      int x = i % width_, y = i / width_;
      int val = board_[y][x];
      hashValue ^= val + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
    }
    return hashValue;
  }
  void Show() const {
    int n = width_ * width_;
    printf("================\n");
    printf("Move: %5d\n", static_cast<int>(actions_.size()));
    for (int i = 0; i < n; i++) {
      int x = i % width_, y = i / width_;
      cout << board_[y][x] << " ";
      if (x == width_ - 1) cout << endl;
    }
    printf("================\n");
  }
  vector<Action> GetActions() {
    return actions_;
  }
  bool SetBoard(const vector<int>& board1D) {
    if (board1D.size() != width_ * width_) return false;
    int facts = 1, product = 0;
    for (int i = 0; i < static_cast<int>(board1D.size()); i++) {
      int x = i % width_, y = i / width_;
      board_[y][x] = board1D[i];
      if (board_[y][x] > 0) product *= board_[y][x];
      if (i > 0) facts *= i;
    }
    return facts == product;
  }
private: 
  int width_;
  int emptyX_, emptyY_;
  vector<vector<int>> board_;
  vector<Action> actions_;
};

class VisitedList {
public:
  void Add(const State& state) {
    visitedHash_.insert(state.GetHash());
  }
  bool IsVisited(const State& state) {
    if (visitedHash_.find(state.GetHash()) != visitedHash_.end()) {
      return true;
    }
    return false;
  }
private:
  set<long long> visitedHash_;
};

class Queue {
public:
  void Put(const State& state) {
    que_.push(state);
  }
  State Get() {
    State state = que_.front(); que_.pop();
    return state;
  }
  bool Empty() {
    return que_.empty();
  }
private:
  queue<State> que_;
};


void Example1() {
  // シャッフル
  printf("******************** Example1 ********************\n");
  State state;
  state.Show();     // 今の状態を表示
  state.Shuffle();  // 10000回ランダムに移動
  state.Show();     // 今の状態を表示
  printf("**************************************************\n\n");
}
void Example2() {
  // 空白(0)を右下から左上まで移動させてみる
  printf("******************** Example2 ********************\n");
  State state;
  state.Show();
  state = state.GetNextState(Action::UP);   // 上に移動した状態を取得
  state.Show();
  state = state.GetNextState(Action::UP);   // 上に移動した状態を取得
  state.Show();
  state = state.GetNextState(Action::LEFT); // 左に移動した状態を取得
  state.Show();
  state = state.GetNextState(Action::LEFT); // 左に移動した状態を取得
  state.Show();
  printf("**************************************************\n\n");
}
void Example3() {
  // 上に動かした後，下に動かして元の位置へ戻した時に，ゴールとなるか確認
  printf("******************** Example3 ********************\n");
  State state;
  state.Show();     // 初期状態を表示
  
  state.DoMove(Action::UP);
  state.Show();     
  cout << "Goal? : " << state.IsGoal() << endl; // 上に動かしたのでクリアにならない

  state.DoMove(Action::DOWN);
  state.Show();
  cout << "Goal? : " << state.IsGoal() << endl; // 元の位置に戻したのでクリア
  printf("**************************************************\n\n");
}
void Hint1() {
  // 1手分動かしてキューに入れる
  printf("********************** Hint1 **********************\n");
  Queue que;

  State state;
  state.Show();     // 初期状態を表示

  for (const auto& action : Action()) {
    State nextState = state.GetNextState(action);
    que.Put(state); // 次の状態をキューへ追加
  }

  // キューが空になるまで取り出し続ける（1手動かした場合の，全ての盤面が取得できる）
  while (!que.Empty()) {
    State nextState = que.Get();
    nextState.Show();
  }
  printf("**************************************************\n\n");
}

int main(void) {
  // 幅優先探索で8パズルをクリアしよう
  // +++++++++++++ クラス説明（下記クラスを利用して幅優先探索を書きます） +++++++++++++
  /*
    State クラス
      bool           CanMove(Action)      : actionの方向に動かせるかを取得
      State          GetNextState(Action) : actionの方向に動かした時の状態を生成
      bool           IsGoal()             : 8パズルが完成したか取得
      void           Show()               : 今の盤面を画面に出力 (答えには使わないがデバッグには使っても良い)
      void           Shuffle()            : 盤面をシャッフルする (答えには使わない) 
      void           DoMove(action)       : actionの方向に動かした時の状態に更新 (答えには使わない)
      vector<Action> GetActions()         : 今までに動かした方向の履歴を取得 (答えには使わない)
    
    Queue クラス
      void Put(State)  : stateをキューへの追加
      State Get(State) : stateをキューから取り出し
      bool Empty()     : キューが空か返す

    VisitedList クラス
      void Add(State)       : 探索済み盤面としてStateの盤面を追加
      bool IsVisited(State) : Stateの盤面が既に探索済みか取得
  */

  // +++++++++++++ クラス使用例とヒント（コメントアウトを外して中身を確認して下さい） +++++++++++++
  // Example1();
  // Example2();
  // Example3();
  // Hint1();


  // ************ ここから下の (A)~(F) の部分のコードを埋めてみよう  ***************
  bool isGoal = false;
  State initState, goalState;
  Queue que;
  VisitedList visitedList;

  // シャッフル
  initState.Shuffle();

  // 初期状態をキューに追加
  que.Put(initState);

  // 初期状態は探索済みにしておく
  visitedList.Add(initState);

  while(!que.Empty()) {
    // キューから状態を取り出し
    State currState = que.Get();

    // もしパズルが完成していたら,「isGoalにtrue」，「goalStateにAで取り出した状態を入れて」break (A)
    

    // 取りうる全ての次の状態を生成して，キューに追加
    for (const auto& action : Action()) {
      // このループで actionが UP, DOWN, LEFT, RIGHT の順で回ってくる(下記コメントアウトを外せば確認できます)
      // cout << "action = " << action << endl; // actionに上下左右の順で入る
      
      // 動かせない方向の時continue (B)
      

      // 次の状態を生成 (C)
      
      
      // 今生成した次の状態が探索済みであればcontinue (D)
      

      // 生成した状態の盤面を探索済みとしておく (E)
      
      
      // 生成した状態をキューに追加 (F)
      
    }
  }
  // ************ ここから上の (A)~(F) の部分のコードを埋めてみよう  ***************

  // シャッフル直後の状態とクリアした状態を表示，その後，クリアまでの手順を表示
  if (isGoal) {
    // シャッフル直後の状態を出力
    cout << "Initial State" << endl;
    initState.Show();
    cout << endl;

    // クリアした状態を出力
    cout << "Goal State!!" << endl;
    goalState.Show();
    cout << endl;

    // クリアまでの手順を出力
    vector<Action> actions = goalState.GetActions();
    State state = initState;

    cout << "Show Clear Movement!" << endl;
    state.Show();
    for (const auto& action : actions) {
      cout << action << endl;
      state.DoMove(action);
      state.Show();
    }
  }
  else {
    cout << "Sorry, it may be a my program bug X(" << endl;
  }

  return 0;
}