#include <stdio.h>
#include "game.h"

// 人 vs 人
void
GameManVsMan(void)
{
     BOARD board;
     uint64_t pos, valid;

     Init(&board);
     DispBoard(&board);

     // 石を置く
     while(board.teban != GAME_OVER){
	  // 合法手を得る
	  valid = GenValidMove(&board);
	  // 手を受け取る
	  pos = GetPos();
	  if (pos == INPUT_ERROR){
	       printf("エラーです。\n");
	       continue;
	  }
	  else if((pos & valid) == 0){
	       printf("非合法手です。\n");
	       continue;
	  }
	  Put(&board, pos);
	  DispBoard(&board);

	  CheckFinishPass(&board);
     }

     ShowResult(&board);

     return;
}

// 人 vs AI
void
GameManVsCom(void)
{
  BOARD board;
  uint64_t pos, valid;

  Init(&board);
  DispBoard(&board);
  while(board.teban != GAME_OVER){
    valid = GenValidMove(&board);

    if(board.teban == SENTE){     // 先手（人) の処理
      while(1){
        pos = GetPos();
        if(pos == INPUT_ERROR){
          printf("エラーです。\n");
          continue;
        }
        else if((pos & valid) == 0){
          printf("非合法手です。\n");
          continue;
        }
        break;
      }
    }
    else if(board.teban == GOTE){ // 後手 (AI) の処理
      // コンピュータ側の処理をここにかく
      pos = Think(&board, valid);
    }

    Put(&board, pos);
    DispBoard(&board);
    CheckFinishPass(&board);
  }

  ShowResult(&board);
  return;
}
