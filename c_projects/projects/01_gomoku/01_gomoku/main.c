/*
	五子棋
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARD_SIZE 15  // 棋盘大小
#define EMPTY "\033[1m\033[30m●\033[0m"  // 空位置(黑棋)     
#define RED   "\033[1m\033[31m●\033[0m"  // 红棋    
#define BLUE  "\033[1m\033[34m●\033[0m"  // 蓝棋    

void init_board(char (*p)[BOARD_SIZE]);  // 初始化棋盘
void clear_screen();  // 清屏
void show_rules();  // 游戏规则
void display_board(char (*p)[BOARD_SIZE]);  // 显示棋盘
int is_valid_move(char (*p)[BOARD_SIZE], int row, int col);  // 检查下棋位置是否有效
int is_valid_and_oneself(char (*p)[BOARD_SIZE], int row, int col, char current_piece);  // 检查下棋位置是否有效并且是己方棋子
int is_win(char (*p)[BOARD_SIZE], int row, int col, char current_piece);  // 检查是否获胜
int is_board_full(char (*p)[BOARD_SIZE]);  // 下满了, 平局

int main(void)
{
	char board[BOARD_SIZE][BOARD_SIZE];  // 棋盘
	int current_player = 1;  // 玩家1(黑子) 玩家2(白子)
	int game_over = 0;  // 游戏是否结束: 1结束 0未结束
	int row, col;  // 行, 列
	char input_row[2], input_col[2];  // 输入坐标

	init_board(board);  // 初始化棋盘

#if 1
	while (!game_over)  // 进行游戏
	{
		clear_screen();  // 清屏
		show_rules();  // 游戏规则
		display_board(board);  // 显示棋盘

		char current_piece = ((current_player == 1) ? 1 : 2);  // 当前棋子
		
		printf("\n    玩家%d (%s) 的回合 : ", current_player, current_player == 1 ? RED : BLUE);

		scanf("%2s %2s", input_row, input_col);  // 下棋位置

		if (input_row[0] >= 'A' && input_row[0] <= 'F')
			row = input_row[0] - 'A' + 10;
		else if(input_row[0] >= 'a' && input_row[0] <= 'f')
			row = input_row[0] - 'a' + 10;
		else
			row = input_row[0] - '0';
			
		if (input_col[0] >= 'A' && input_col[0] <= 'F')
			col = input_col[0] - 'A' + 10;
		else if (input_col[0] >= 'a' && input_col[0] <= 'f')
			col = input_col[0] - 'a' + 10;
		else
			col = input_col[0] - '0';
			
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
#else	
#endif	
	printf("\n感谢游玩五子棋游戏!\n");
	return 0;
}

void init_board(char (*p)[BOARD_SIZE]) {
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			p[i][j] = 0;  // black ⚫
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