/*
    电子口算卡
    随机生成 10 道 100以内加减乘除和取模的口算题目，用户输入答案，答对得 10 分，答错不扣分，最后显示总得分。
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>  Linux/Unix


/* 函数定义 */
void init_rand_seed();  // 初始化随机种子
void clear_screen();  // 清屏
int generate_number();  // 生成随机数
int generate_operator();  // 生成运算符
void handle_add(int a, int b, int *score);  // 处理加法
void handle_subtraction(int a, int b, int *score);  // 处理减法
void handle_multiplication(int a, int b, int *score);  // 处理乘法
void handle_division(int a, int b, int *score);  // 处理除法
void handle_modulo(int a, int b, int *score);  // 处理取模 
void print_final_score(int score);  // 最终分数
void wait_for_enter_to_exit();  // 等待用户回车退出, 便于用户查看错题和得分


/* 全局常量 */
#define MAX_QUESTIONS 10  // 最大道数,  10 道题
#define MAX_NUMBER 100  // 最大计算数, 100 以内
#define SCORE_PER_QUESTION 10  // 每题得分, 10分


int main()
{
    init_rand_seed();
    clear_screen();

    int score = 0;  // 得分

    printf("========== 电子口算卡 ==========\n");
    printf("将生成 %d 道 100 以内的加减乘除和取模运算题\n", MAX_QUESTIONS);
    printf("每答对一题得 %d 分，答错不扣分\n\n", SCORE_PER_QUESTION);

    for (int i = 1; i <= MAX_QUESTIONS; i++) {
        printf("第 %d 题: ", i);

        int a = generate_number();
        int b = generate_number();
        int op = generate_operator();

        switch (op) {
            case 0: handle_add(a, b, &score); break;
            case 1: handle_subtraction(a, b, &score); break;
            case 2: handle_multiplication(a, b, &score); break;
            case 3: handle_division(a, b, &score); break;
            case 4: handle_modulo(a, b, &score); break;
        }
    }

    print_final_score(score);
    wait_for_enter_to_exit();  // 等待用户按回车键退出
    return 0;
}


/* 函数实现 */
void init_rand_seed() {
    srand(time(NULL) + getpid());  // 把当前调用进程的 ID 作为随机数种子
}

void clear_screen() {
    system("clear");  // 清屏
}

int generate_number() {
    return rand() % (MAX_NUMBER + 1);
}

// 生成随机运算符 ( 0->+    1->-    2->*   3->/   4->% )
int generate_operator() {
    return rand() % 5;
}


void handle_add(int a, int b, int *score) {
    printf("%d + %d = ", a, b);

    int result;  // 输入你的结果
    scanf("%d", &result);
    
    int correct_answer = a + b;  // 答案

    if (result == correct_answer) {
        printf("✔️ 正确！ +%d 分\n\n", SCORE_PER_QUESTION);
        *score += SCORE_PER_QUESTION;
    }
    else {
        printf("❌ 错误！正确答案是: %d\n\n", correct_answer);
    }

}

void handle_subtraction(int a, int b, int *score) {
    printf("%d - %d = ", a, b);

    int result;
    scanf("%d", &result);

    int correct_answer = a - b;

    if (result == correct_answer) {
        printf("✔️ 正确！ +%d 分\n\n", SCORE_PER_QUESTION);
        *score += SCORE_PER_QUESTION;
    }
    else {
        printf("❌ 错误！正确答案是: %d\n\n", correct_answer);
    }
}


void handle_multiplication(int a, int b, int* score) {
    printf("%d × %d = ", a, b);

    int result;
    scanf("%d", &result);

    int correct_answer = a * b;

    if (result == correct_answer) {
        printf("✔️ 正确！ +%d 分\n\n", SCORE_PER_QUESTION);
        *score += SCORE_PER_QUESTION;
    }
    else {
        printf("❌ 错误！正确答案是: %d\n\n", correct_answer);
    }
}
void handle_division(int a, int b, int* score) {
    // 确保除数不为0
    while (b == 0) {
        b = generate_number();  // 一直到生成的除数不为零
    }

    printf("%d ÷ %d = ", a, b);

    int result;
    scanf("%d", &result);

    int correct_answer = a / b;  // 整数除法

    if (result == correct_answer) {
        printf("✔️ 正确！ +%d 分\n\n", SCORE_PER_QUESTION);
        *score += SCORE_PER_QUESTION;
    }
    else {
        printf("❌ 错误！正确答案是: %d\n\n", correct_answer);
    }
}

void handle_modulo(int a, int b, int* score) {
    while (b == 0) {
        b = generate_number();
    }

    printf("%d %% %d = ", a, b);

    int result;
    scanf("%d", &result);

    int correct_answer = a % b;

    if (result == correct_answer) {
        printf("✔️ 正确！ +%d 分\n\n", SCORE_PER_QUESTION);
        *score += SCORE_PER_QUESTION;
    }
    else {
        printf("❌ 错误！正确答案是: %d\n\n", correct_answer);
    }
}

void print_final_score(int score) {
    printf("\n==============================\n");
    printf("答题结束！\n");
    printf("你的总得分是: %d 分 (满分: %d 分)\n",
        score, MAX_QUESTIONS * SCORE_PER_QUESTION);
    printf("==============================\n");
}

void wait_for_enter_to_exit() {

    // 清除输入缓冲区中可能残留的字符（比如上次输入答案时的回车）
    while (getchar() != '\n');

    printf("\n请按 Enter 键退出程序...");
    getchar();  // 等待用户按下回车键
}