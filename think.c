#include <stdio.h>
#include "game.h"

const int BOARD_SCORE[64] = {
  30, -20, 0, -1, -1, 0, -20, 30,
  -20, -25, -3, -3, -3, -3, -25, -20,
  0, -3, 0, -1, -1, 0, -3, 0,
  -1, -3, -1, -1, -1, -1, -3, -1,
  -1, -3, -1, -1, -1, -1, -3, -1,
  0, -3, 0, -1, -1, 0, -3, 0,
  -20, -25, -3, -3, -3, -3, -25, -20,
  30, -20, 0, -1, -1, 0, -20, 30,
};


/* 手を取り消す関数 */
void
UndoPut(BOARD* board, uint64_t pos, uint64_t rev)
{
  board->move_num--; /* まず手数を一つ戻す */
  switch(board->teban)
  {
    case SENTE:
      board->black |= rev;
      board->white ^= pos | rev;
      board->teban = GOTE;
      break;
    case GOTE:
      board->white |= rev;
      board->black ^= pos | rev;
      board->teban = SENTE;
      break;
    default:
      break;
  }
}

/* 局面の点数を計算する関数 */
void
CaluMoveScore(BOARD* board)
{
  int i;
  board->black_score = 0;
  board->white_score = 0;

  for(i = 0; i <= MAX_SHIFT; i++){
    if((board->black & ((uint64_t)1 << i)) != 0){
      board->black_score += BOARD_SCORE[i];
    }
  }

  for(i = 0; i <= MAX_SHIFT; i++){
    if((board->white & ((uint64_t)1 << i)) != 0){
      board->white_score += BOARD_SCORE[i];
    }
  }
}


/* 重み付けされた点数を用いて盤面評価する思考エンジン */
uint64_t
Think(BOARD* board, uint64_t valid)
{
  int i,max_score = -999;
  uint64_t tmp_pos, pos, rev;

  for(i = 0; i <= MAX_SHIFT; i++){
    if((tmp_pos = (valid & ((uint64_t)1 << i))) != 0){
      rev = GetReverse(board, tmp_pos); /* 後で手を取り消すために */
      Put(board, tmp_pos);
      CaluMoveScore(board);
      UndoPut(board, tmp_pos, rev);

      if((board->teban) == SENTE){           // 先手
        if(board->black_score > max_score){
          max_score = board->black_score;
          pos = (valid & ((uint64_t)1 << i));
        }
        printf("%d - ", board->black_score);
      }
      else if((board->teban) == GOTE){       // 後手
        if(board->white_score > max_score){
          max_score = board->white_score;
          pos = (valid & ((uint64_t)1 << i));
        }
        printf("%d - ", board->white_score);
      }
      CorTranslate(tmp_pos);
    }
  }
  printf("------\n");
  CorTranslate(pos);
  return pos;
}
