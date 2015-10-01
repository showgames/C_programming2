#include <stdio.h>
#include "game.h"

int
main(void)
{
     int game_mode;
     char buf[BUF_SIZE];

     while(1){
	  printf("モードを選択してください。\n 先手 vs 後手\n0: 人 vs 人\n2: 人 vs AI\n9: 終了\n");
	  fgets(buf, sizeof(buf), stdin);
	  sscanf(buf, "%d", &game_mode);

	  switch(game_mode){
	      case 0: GameManVsMan(); break;
        case 2: GameManVsCom(); break;
	      case 9: printf("終了します、お疲れ様でした。\n"); return 0;
	      default: printf("エラー。もう一度入力してください。\n");
	  }
     }

     return 0;
}
