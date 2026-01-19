#ifndef __GONOKU_H
#define __GONOKU_H

#define BOARD_SIZE 15  // 棋盘大小
#define EMPTY "\033[1m\033[30m●\033[0m"  // 空位置(黑棋)     
#define RED   "\033[1m\033[31m●\033[0m"  // 红棋    
#define BLUE  "\033[1m\033[34m●\033[0m"  // 蓝棋  

void init_board(char (*p)[BOARD_SIZE]);  // 初始化棋盘
void clear_screen();  // 清屏
void show_rules();  // 游戏规则
void display_board(char (*p)[BOARD_SIZE]);  // 显示棋盘
void handle_input(int *row, int *col, char *input_row, char *input_col);  // 处理输入
int is_valid_move(char (*p)[BOARD_SIZE], int row, int col);  // 检查下棋位置是否有效
int is_valid_and_oneself(char (*p)[BOARD_SIZE], int row, int col, char current_piece);  // 检查下棋位置是否有效并且是己方棋子
int is_win(char (*p)[BOARD_SIZE], int row, int col, char current_piece);  // 检查是否获胜
int is_board_full(char (*p)[BOARD_SIZE]);  // 下满了, 平局

#endif