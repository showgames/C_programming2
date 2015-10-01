#include <stdint.h>

#define INPUT_ERROR 3

#define BUF_SIZE 1024
#define MAX_SHIFT 63
#define MAX_REVERSE 18 /* オセロにおけるひっくり返せる石の最大個数 */

#define PASS 1
#define FINISH 2

// 手番を表す列挙型
typedef enum TEBAN
  {
    SENTE = -1,
    GOTE  = 1,
    GAME_OVER = 0
  }TEBAN;

// 局面を表す構造体
typedef struct BOARD
{
  uint64_t black, white;
  TEBAN teban;
  int move_num;
  int black_score;
  int white_score;
}BOARD;

// 基本関数群
void Init(BOARD* board); // 初期化関数
void DumpBoardStr(BOARD* board);   // BOARD構造体表示関数(デバッグ専用)
void DispBoard(BOARD* board); // 盤面表示関数
uint64_t PosTranslate(char file, int rank); // 座標をuint64_tのposに変換する関数
void CorTranslate(uint64_t pos); // uint64_t型変数を盤面座標(f 5)に変換して表示する関数
uint64_t GetPos(); // 入力から座標を受け取る関数
void Put(BOARD* board, uint64_t pos); // 石をおく関数
uint64_t GenValidMove(const BOARD* board); // 合法手を生成する関数
uint64_t GetReverse(BOARD* board, uint64_t pos); // 反転パターンを求める関数
int CheckFinishPass(BOARD* board); // 終了・パス判定関数
int NumOfStone(uint64_t bits); // 石の数をかぞえる関数(立っているビットの数をかぞえる)
int ShowResult(BOARD* board); // 結果を表示する関数

// AI用関数
uint64_t Think(BOARD* board, uint64_t valid);

// ゲームモード
void GameManVsMan(void); // 人 vs 人
void GameManVsCom(void); // 人 vs AI
