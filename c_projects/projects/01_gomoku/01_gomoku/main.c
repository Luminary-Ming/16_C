/*
	五子棋
*/
#include <stdio.h>
#include "gomoku.h"

int main(void)
{
	char board[BOARD_SIZE][BOARD_SIZE];  // 棋盘
	int current_player = 1;  // 玩家1  玩家2
	int game_over = 0;  // 游戏是否结束: 1结束 0未结束
	int row, col;  // 行, 列
	char input_row[2], input_col[2];  // 输入坐标

	init_board(board);  // 初始化棋盘

	while (!game_over)  // 进行游戏
	{
		clear_screen();  // 清屏
		show_rules();  // 游戏规则
		display_board(board);  // 显示棋盘

		char current_piece = ((current_player == 1) ? 1 : 2);  // 当前棋子
		
		printf("\n    玩家%d (%s) 的回合 : ", current_player, current_player == 1 ? RED : BLUE);

		scanf("%1s %1s", input_row, input_col);  // 下棋位置

		handle_input(&row, &col, input_row, input_col);
	
		// 退出游戏特殊指令 (人性化,不强制玩家玩完)
		if((input_row[0] == 'g' && input_col[0] == 'g'))
		{
			printf("    特殊指令执行!, 游戏结束!\n");
			break;
		}

		// 检查下棋位置是否有效
		if (!is_valid_move(board, row, col))
		{
			printf("    无效的移动! 请重新输入。\n");
			printf("    按Enter键继续...");
			getchar();  // 吃掉用户输入坐标按下后的回车
			getchar();  // 吃掉 "按Enter键继续..."
			continue;
		}

		// 放置棋子
		board[row - 1][col - 1] = current_piece;

		// 检查是否获胜
		if (is_win(board, row, col, current_piece))
		{
			clear_screen();  // 清屏
			display_board(board);  // 显示棋盘
			printf("\n恭喜! 玩家%d (%s) 获胜!\n", current_player, current_piece == 1 ? RED : BLUE);
			game_over = 1;  // 游戏结束
		}
		else if(is_board_full(board))  // 下满了, 平局
		{
			clear_screen();  // 清屏
			display_board(board);  // 显示棋盘
			printf("\n平局! 一个师傅交的, 破不了招啊!");
			game_over = 1;
		}
		else  // 切换玩家
		{
			current_player = (current_player == 1) ? 2 : 1;
		}
	}
	
	printf("\n感谢游玩五子棋游戏!\n");
	return 0;
}