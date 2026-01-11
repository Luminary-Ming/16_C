/*
	猜数游戏:
		输入一个 n ,随机产生一个 n 以内的数字去猜
		如果大了,打印猜大了
		如果小了,打印猜小了
		如果正确,打印猜对了
		循环知道猜对了结束, 每次打印带上猜的次数
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// 函数声明
void init_random_seed();  // 初始化随机种子
int generate_secret_number(int max_range);  // 生成秘密数字
int get_max_range_from_user();  // 获取最大范围
int get_user_guess(int attempt_count);  // 获取用户猜测
int compare_guess(int guess, int secret);  // 比较猜测结果
void display_hint(int result, int guess);  // 显示提示信息
void display_congratulations(int attempt_count);  // 显示祝贺信息
void wait_for_enter_to_exit();  // // 等待用户回车退出, 便于用户查看结果


int main()
{
	// 初始化
	init_random_seed();

	// 获取游戏范围
	int max_range = get_max_range_from_user();

	// 生成秘密数字
	int secret_number = generate_secret_number(max_range);


	// 游戏主循环
	int attempt_count = 0;      // 尝试次数
	int game_over = 0;          // 游戏结束标志

	while (!game_over) {
		attempt_count++;

		// 获取用户猜测
		int user_guess = get_user_guess(attempt_count);

		// 比较猜测结果
		int result = compare_guess(user_guess, secret_number);

		// 根据结果处理
		if (result == 0) {
			// 猜对了
			display_congratulations(attempt_count);
			game_over = 1;  // 结束游戏
		}
		else {
			// 猜错了，显示提示
			display_hint(result, user_guess);
		}
	}
	wait_for_enter_to_exit();
	return 0;
}

void init_random_seed() {
	srand(time(NULL) + getpid());  // 结合时间和进程ID，增加随机性
}

int get_max_range_from_user() {
	int max_range;
	printf("请输入要猜的数的最大范围 (0-N): ");
	scanf("%d", &max_range);

	// 简单的输入验证
	if (max_range < 1) {
		printf("范围必须大于0，已自动设置为100\n");
		max_range = 100;
	}

	return max_range;
}

int generate_secret_number(int max_range) {
	return rand() % (max_range + 1);  // 0 到 max_range
}

int get_user_guess(int attempt_count) {
	int guess;
	printf("第 %d 次尝试，请输入你猜的数字: ", attempt_count);
	scanf("%d", &guess);
	return guess;
}


int compare_guess(int guess, int secret) {
	if (guess == secret) {
		return 0;       // 猜对了
	}
	else if (guess > secret) {
		return 1;       // 猜大了
	}
	else {
		return -1;      // 猜小了
	}
}

void display_hint(int result, int guess) {
	if (result == 1) {
		printf("猜大了！");
		printf(" (试试比 %d 小的数)\n\n", guess);
	}
	else {
		printf("猜小了！");
		printf(" (试试比 %d 大的数)\n\n", guess);
	}
}

void display_congratulations(int attempt_count) {
	printf("\n🎉 恭喜你！\n");
	printf("你在第 %d 次尝试时猜对了！\n", attempt_count);

	// 根据尝试次数给出评级
	if (attempt_count <= 3) {
		printf("评级：🌟🌟🌟🌟🌟🌟🌟 卧槽牛逼！\n");
	}
	else if (attempt_count <= 7) {
		printf("评级：🌟🌟🌟🌟🌟  牛逼啊！\n");
	}
	else if (attempt_count <= 15) {
		printf("评级：🌟🌟🌟   哎呦不错哦！\n");
	}
	else {
		printf("评级：🌟     差点意思啊, 行不行啊你！\n");
	}
	printf("==============================\n");
}

void wait_for_enter_to_exit() {

	// 清除输入缓冲区中可能残留的字符（比如上次输入答案时的回车）
	while (getchar() != '\n');

	printf("\n请按 Enter 键退出程序...");
	getchar();  // 等待用户按下回车键
}