#include <stdio.h>
#include "game.h"

// 初期化関数
void
Init(BOARD* board)
{
     board->black = ((uint64_t)1<<28) + ((uint64_t)1<<35);
     board->white = ((uint64_t)1<<27) + ((uint64_t)1<<36);
     board->teban = SENTE;
     board->move_num = 0;

     board->black_score = 0;
     board->white_score = 0;
}

// BOARD構造体表示関数 (デバッグ用)
void
DumpBoardStr(BOARD* board)
{
     printf("board->black: %llu\n", board->black);
     printf("board->white: %llu\n", board->white);
     printf("board->teban: %d\n", board->teban);
     printf("board->move_num: %d\n", board->move_num);
}

// 盤面表示関数
void
DispBoard(BOARD* board)
{
     int i, rank = 1;
     uint64_t pos = (uint64_t)1<<63;

     printf(" a b c d e f g h\n");
     for(i = 0; i < 64; i++){
	  // 行番号
	  if(i % 8 == 0) printf("%d", rank++);
	  // 盤面
	  if((board->black & pos) != 0) printf("黒");
	  else if((board->white & pos) != 0) printf("白");
	  else printf("・");
	  // 改行
	  if (i % 8 == 7) printf("\n");
	  // 次のマスにビットシフト
	  pos >>= 1;
     }
     // 石数表示
     printf("黒石: %d個, 白石: %d個\n", NumOfStone(board->black), NumOfStone(board->white));

     // 手番
     printf("\n手番: ");
     switch(board->teban){
     case SENTE: printf("先手\n"); break;
     case GOTE:  printf("後手\n"); break;
     default:    break;
     }
}

// 座標をuint64_tのposに変換する関数
uint64_t
PosTranslate(char file, int rank)
{
  int file_num;
  uint64_t pos;

  file_num = (int)(7 - file + 'a');
  pos = ((uint64_t)1<<(file_num + 8 * (8 - rank)));
  return pos;
}

// 座標を受け取り、posを返す関数
uint64_t
GetPos()
{
  char file; // 列番号(アルファベット)
  int rank;  // 行番号(数字)
  uint64_t pos; // 指定個所を示すビットボード
  char buf[BUF_SIZE];

  printf("座標を入力してください。(例:f 5)\n");
  fgets(buf, sizeof(buf), stdin);
  sscanf(buf, "%s%d", &file, &rank);


  // 不正値ならエラーを返す
  if(file < 'a' || file > 'h' || rank < 1 || rank > 8) return INPUT_ERROR;

  // 受け取った座標からビットボードを生成
  pos = PosTranslate(file, rank);
  return pos;
}

// 石を置く関数 posは絶対に合法手
void
Put(BOARD* board, uint64_t pos)
{
     uint64_t rev;

     // 反転パターン取得
     rev = GetReverse(board, pos);

  switch(board->teban)
    {
    case SENTE:
	 board->black ^= pos | rev; // 黒石を増やす
	 board->white ^= rev; // 白石を消す
	 board->teban = GOTE;
	 break;
    case GOTE:
	 board->white ^= pos | rev;
	 board->black ^= rev;
	 board->teban = SENTE;
	 break;
    default:
	 break;
    }
  board->move_num++;
  return;
}

// 合法手を生成する関数
uint64_t
GenValidMove(const BOARD* board)
{
  int i;
  uint64_t me, enemy, masked_enemy, t, valid = 0, blank;

  // 現在手番の方をme, 相手をenemyにする
  if(board->teban == SENTE)
    {
      me = board->black;
      enemy = board->white;
    }
  else
    {
      me = board->white;
      enemy = board->black;
    }

  // 空マスのビットボードを（黒または白)のビットNOTで得る
  blank = ~(board->black | board->white);

  // 右方向にひっくり返せる着手可能点を探す
  masked_enemy = enemy & 0x7e7e7e7e7e7e7e7e; // 端列を除く敵石
  t = masked_enemy & (me << 1); // 右隣に自石がある敵石を調べる
  for(i = 0; i < 5; i++) // 一方向で取れる石は最大6個なので6回繰り返す
    {
      t |= masked_enemy & (t << 1);
    }
  // validに追加する
  valid = blank & (t << 1);

  // 左方向
  masked_enemy = enemy & 0x7e7e7e7e7e7e7e7e;
  t = masked_enemy & (me >> 1);
  for(i = 0; i < 5; i++)
    {
      t |= masked_enemy & (t >> 1);
    }
  valid |= blank & (t >> 1);

  // 上方向
  masked_enemy = enemy & 0x00ffffffffffff00;
  t = masked_enemy & (me << 8);
  for(i = 0; i < 5; i++)
    {
      t |= masked_enemy & (t << 8);
    }
  valid |= blank & (t << 8);

  // 下方向
  masked_enemy = enemy & 0x00ffffffffffff00;
  t = masked_enemy & (me >> 8);
  for(i =0; i < 5; i++)
    {
      t |= masked_enemy & (t >> 8);
    }
  valid |= blank & (t >> 8);

  // 右上方向
  masked_enemy = enemy & 0x007e7e7e7e7e7e00;
  t = masked_enemy & (me << 7);
  for(i = 0; i < 5; i++)
    {
      t |= masked_enemy & (t << 7);
    }
  valid |= blank & (t << 7);

  // 左上方向
  masked_enemy = enemy & 0x007e7e7e7e7e7e00;
  t = masked_enemy & (me << 9);
  for(i = 0; i < 5; i++)
    {
      t |= masked_enemy & (t << 9);
    }
  valid |= blank & (t << 9);

  // 右下方向
  masked_enemy = enemy & 0x007e7e7e7e7e7e00;
  t = masked_enemy & (me >> 9);
  for(i = 0; i < 5; i++)
    {
      t |= masked_enemy & (t >> 9);
    }
  valid |= blank & (t >> 9);

  // 左下方向
  masked_enemy = enemy & 0x007e7e7e7e7e7e00;
  t = masked_enemy & (me >> 7);
  for(i = 0; i < 5; i++)
    {
      t |= masked_enemy & (t >> 7);
    }
  valid |= blank & (t >> 7);

  return valid;
}

// 反転パターンを求める関数
uint64_t
GetReverse(BOARD* board, uint64_t pos)
{
     int i;
     uint64_t me, enemy, mask, rev = 0, rev_cand;

     if(board->teban == SENTE){
	  me = board->black;
	  enemy = board->white;
     }
     else{
	  me = board->white;
	  enemy = board->black;
     }

     // 右方向
     rev_cand = 0;
     mask = 0x7e7e7e7e7e7e7e7e;
     for(i = 1; ((pos >> i) & mask & enemy) != 0; i++){
	  rev_cand |= (pos >> i);
     }
     if(((pos >> i) & me) != 0) rev |= rev_cand;

     // 左方向
     rev_cand = 0;
     mask = 0x7e7e7e7e7e7e7e7e;
     for(i = 1;((pos << i) & mask & enemy) != 0; i++){
	  rev_cand |= (pos << i);
     }
     if(((pos << i) & me) != 0) rev |= rev_cand;

     // 上方向
     rev_cand = 0;
     mask = 0x00ffffffffffff00;
     for(i = 1;((pos << 8 * i) & mask & enemy) != 0; i++){
	  rev_cand |= (pos << 8 * i);
     }
     if(((pos << 8 * i) & me) != 0) rev |= rev_cand;

     // 下方向
     rev_cand = 0;
     mask = 0x00ffffffffffff00;
     for(i = 1;((pos >> 8 * i) & mask & enemy) != 0; i++){
	  rev_cand |= (pos >> 8 * i);
     }
     if(((pos >> 8 * i) & me) != 0) rev |= rev_cand;

     // 右上方向
     rev_cand = 0;
     mask = 0x007e7e7e7e7e7e00;
     for(i = 1;((pos << 7 * i) & mask & enemy) != 0; i++){
	  rev_cand |= (pos << 7 * i);
     }
     if(((pos << 7 * i) & me) != 0) rev |= rev_cand;

     // 左上方向
     rev_cand = 0;
     mask = 0x007e7e7e7e7e7e00;
     for(i = 1;((pos << 9 * i) & mask & enemy) != 0; i++){
	  rev_cand |= (pos << 9 * i);
     }
     if(((pos << 9 * i) & me) != 0) rev |= rev_cand;

     // 右下方向
     rev_cand = 0;
     mask = 0x007e7e7e7e7e7e00;
     for(i = 1;((pos >> 9 * i) & mask & enemy) != 0; i++){
	  rev_cand |= (pos >> 9 * i);
     }
     if(((pos >> 9 * i) & me) != 0) rev |= rev_cand;

     // 左下方向
     rev_cand = 0;
     mask = 0x007e7e7e7e7e7e00;
     for(i = 1;((pos >> 7 * i) & mask & enemy) != 0; i++){
	  rev_cand |= (pos >> 7 * i);
     }
     if(((pos >> 7 * i) & me) != 0) rev |= rev_cand;

     return rev;
}

// 終了・パス判定関数
int
CheckFinishPass(BOARD* board)
{
     uint64_t valid;

     valid = GenValidMove(board);

     // 終了・パス判定
     if(valid == 0){
	  board->teban *= -1;
	  if(GenValidMove(board) == 0){
	       // 終了
	       board->teban = GAME_OVER;
	       return FINISH;
	  }
	  printf("パス\n");
	  DispBoard(board);
	  return PASS;
     }
     return 0;
}

// 石の数を数える関数(立っているビットの数を数える)
int
NumOfStone(uint64_t bits)
{
  bits = bits - (bits >> 1 & 0x5555555555555555); // 2bitごと
  bits = (bits & 0x3333333333333333) + (bits >> 2 & 0x3333333333333333); // 4bitごと
  bits = (bits & 0x0f0f0f0f0f0f0f0f) + (bits >> 4 & 0x0f0f0f0f0f0f0f0f); // 8bitごと
  bits = (bits & 0x00ff00ff00ff00ff) + (bits >> 8 & 0x00ff00ff00ff00ff); // 16bitごと
  bits = (bits & 0x0000ffff0000ffff) + (bits >> 16 & 0x0000ffff0000ffff); // 32bitごと
  return (bits + (bits >> 32)) & 0x000000000000007f;
}

// 結果表示用関数
int
ShowResult(BOARD* board)
{
  if(NumOfStone(board->black) > NumOfStone(board->white))
    {
      printf("黒の勝ちです。\n");
      return 1;
    }
  else if(NumOfStone(board->black) < NumOfStone(board->white))
    {
      printf("白の勝ちです。\n");
      return -1;
    }
  else
    {
      printf("引き分けです。\n");
      return 0;
    }
}

/* 64bit変数を盤面座標(例. f 5)に変換して表示する */
void
CorTranslate(uint64_t pos)
{
  int rank, i = 0;
  char file;

  for (i = 0; i <= MAX_SHIFT; i++){
    if((pos & ((uint64_t)1 << i)) != 0){
      rank = 8 - (i / 8);
      file = 7 - (i % 8) + 'a';
      printf("%c %d\n", file, rank);
      break;
    }
  }
}
