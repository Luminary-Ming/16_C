/*
    电子口算卡
    随机生成 10 道 100以内加减乘除和取模的口算题目，用户输入答案，答对得 10 分，答错不扣分，最后显示总得分。
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>  Linux/Unix

int main()
{

    srand(time(NULL) + getpid());  // 把当前调用进程的 ID 作为随机数种子

    int a = 0, b = 0, c = 0, score = 0;

    int result = 0;

    system("clear");  // 清屏

    for (int i = 0; i < 10; i++) {

        a = rand() % 101; // 生成一个 0 到 100 之间的随机数
        b = rand() % 101; // 生成一个 0 到 100 之间的随机数
        c = rand() % 5;   // 生成一个 0 到 4 之间的随机数 0表示加法，1表示减法，2表示乘法，3表示除法，4表示取模

        switch (c) {
        case 0:
            printf("计算题目: %d + %d = ", a, b);
            scanf("%d", &result);  // 输入结果
            if (result == a + b) {
                printf("Bingo!    +10 分 \n\n");
                score += 10;  // 加 10 分
            }
            else {
                printf("Error!，正确答案是: %d\n\n", a + b);
            }
            break;

        case 1:
            printf("计算题目: %d - %d = ", a, b);
            scanf("%d", &result);  // 输入结果
            if (result == a - b) {
                printf("Bingo!    +10 分 \n\n");
                score += 10;  // 加 10 分
            }
            else {
                printf("Error!，正确答案是: %d\n\n", a - b);
            }
            break;

        case 2:
            printf("计算题目: %d * %d = ", a, b);
            scanf("%d", &result);  // 输入结果
            if (result == a * b) {
                printf("Bingo!    +10 分 \n\n");
                score += 10;  // 加 10 分
            }
            else {
                printf("Error!，正确答案是: %d\n\n", a * b);
            }
            break;

        case 3:
            // 防止除数是 0
            while (b == 0) {
                b = rand() % 101;
            }
            printf("计算题目: %d / %d = ", a, b);
            scanf("%d", &result);  // 输入结果
            if (result == a / b) {
                printf("Bingo!    +10 分 \n\n");
                score += 10;  // 加 10 分
            }
            else {
                printf("Error!，正确答案是: %d\n\n", a / b);
            }
            break;

        default:
            // 防止除数是 0
            while (b == 0) {
                b = rand() % 101;
            }
            printf("计算题目: %d %% %d = ", a, b);
            scanf("%d", &result);  // 输入结果
            if (result == a % b) {
                printf("Bingo!    +10 分 \n\n");
                score += 10;  // 加 10 分
            }
            else {
                printf("Error!，正确答案是: %d\n\n", a % b);
            }
            break;
        }
    }

    printf("你的总得分是: %d 分\n", score);

    return 0;
}