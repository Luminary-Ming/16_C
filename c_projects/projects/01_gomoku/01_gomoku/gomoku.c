#include "gomoku.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_board(char (*p)[BOARD_SIZE])
{
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			p[i][j] = 0; 
		}
	}
}

void clear_screen()
{
	system("clear");
}

void print_rainbow_title_basic()
{
	printf("========== ");
	printf("\033[5m");  // 开启闪烁
	printf("\033[31m超\033[32m级\033[33m无\033[34m敌\033[35m炫\033[36m酷\033[31m牛\033[32m逼\033[33m五\033[34m子\033[35m棋\033[0m ==========\n");
}

void show_rules()
{
	printf("\n\n");
	print_rainbow_title_basic();
	printf("规则:\n");
	printf(" 1. 棋盘大小为 15 × 15\n");
	printf(" 2. 玩家1 使用 %s ，玩家2 使用 %s\n", RED, BLUE);
	printf(" 3. 输入坐标格式: 行 列 (如: 7 7, 7 A, A A)\n");
	printf(" 4. 输入 g g 可以退出游戏\n");
	printf(" 5. 管你看没看懂!!!玩就完了!!!\n");
	print_rainbow_title_basic();
	printf("\n");
}

void handle_input(int *row, int *col, char *input_row, char *input_col)
{
	// 处理行输入
	if (input_row[0] >= 'A' && input_row[0] <= 'O')
		*row = input_row[0] - 'A' + 10;
	else if (input_row[0] >= 'a' && input_row[0] <= 'o')
		*row = input_row[0] - 'a' + 10;
	else if (input_row[0] >= '0' && input_row[0] <= '9')
		*row = input_row[0] - '0';
	else
		*row = -1;  // 无效输入标记

	// 处理列输入
	if (input_col[0] >= 'A' && input_col[0] <= 'O')
		*col = input_col[0] - 'A' + 10;
	else if (input_col[0] >= 'a' && input_col[0] <= 'o')
		*col = input_col[0] - 'a' + 10;
	else if (input_col[0] >= '0' && input_col[0] <= '9')
		*col = input_col[0] - '0';
	else
		*col = -1;  // 无效输入标记
}

void display_board(char (*p)[BOARD_SIZE])
{
	printf("%5d", 0);  // 左上角显示 0
	for (int i = 1; i <= BOARD_SIZE; i++)  // 打印列号
	{
		if (i == 1)
		{
			printf("%2d", 1);
			continue;
		}
		if (i >= 10)
			printf("%2c", i - 10 + 'A');
		else
			printf("%2d", i);
	}

	printf("\n");

	for (int i = 1; i <= BOARD_SIZE; i++)  // 打印棋盘
	{
		// 行号
		if (i >= 10)
			printf("%5c ", i - 10 + 'A');
		else
			printf("%5d ", i);

		for (int j = 1; j <= BOARD_SIZE; j++)
		{
			switch (p[i - 1][j - 1])
			{
				case 1: printf("%s ", RED); break;  // 打印红棋
				case 2: printf("%s ", BLUE); break;  // 打印蓝棋
				default:printf("%s ", EMPTY); break;  // 打印黑棋
			}
		}
		printf("\n");
	}
}

int is_valid_move(char (*p)[BOARD_SIZE], int row, int col)
{
	return row >= 1 && row <= BOARD_SIZE &&
		col >= 1 && col <= BOARD_SIZE &&
		p[row - 1][col - 1] == 0;
}

int is_valid_and_oneself(char (*p)[BOARD_SIZE], int row, int col, char current_piece)
{
	return row >= 1 && row <= BOARD_SIZE &&
		col >= 1 && col <= BOARD_SIZE &&
		p[row - 1][col - 1] == current_piece;
}

int is_win(char (*p)[BOARD_SIZE], int row, int col, char current_piece)
{
	// 定义四个方向
	int directions[4][2] = {
		{0, 1},  // 水平列增量 →
		{1, 0},  // 垂直行增量 ↓
		{1, 1},  // 对角线增量 ↘︎
		{1, -1}  // 对角线增量 ↙︎
	};

	for (int i = 0; i < 4; i++)  // 遍历四种方向
	{
		int row_x = directions[i][0];  // 行增量
		int col_y = directions[i][1];  // 列增量

		int count = 1;  // 棋子数量 (当前位置已经有一个棋子)

		for (int j = 1; j < 5; j++)  // 正方向检查 4 次 (看正方向是否已自己的 4 颗棋子)
		{
			int new_row = row + j * row_x;
			int new_col = col + j * col_y;

			if (is_valid_and_oneself(p, new_row, new_col, current_piece))
				count++;  // 位置有效, 且是己方棋子, 数量 + 1
			else
				break;  // 跳过这个方向的检查 
		}

		for (int j = 1; j < 5; j++)  // 反方向检查 4 次 (看反方向是否已自己的 4 颗棋子)
		{
			int new_row = row - j * row_x;
			int new_col = col - j * col_y;

			if (is_valid_and_oneself(p, new_row, new_col, current_piece))
				count++;  // 位置有效, 且是己方棋子, 数量 + 1
			else
				break;  // 跳过这个方向的检查 
		}

		if (count == 5)
			return 1;  // 有连续的 5 棋, 获胜!
	}
	return 0;  // 未获胜

}

int is_board_full(char (*p)[BOARD_SIZE])
{
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (p[i][j] == 0)
				return 0;
		}
	}
	return 1;
}