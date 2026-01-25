#include <stdio.h>
#include <stdlib.h>

struct hanoi  // 汉诺塔的结构
{
    int hanoi_arr[7];  // 栈式存储空间
    int hanoi_ind;  // 栈顶元素
}hanoi[3] = { {{7,6,5,4,3,2,1}, 7}, {}, {} };  // 分别表示3个柱子


void draw(void)
{
    int i = 0, j = 0;  // 循环变量

    system("clear");
    for (i = 0; i < 3; i++)  // i循环, 表示3个柱子
    {
        for (j = 0; j < hanoi[i].hanoi_ind; j++)
            printf("%d", hanoi[i].hanoi_arr[j]);
        for (; j < 10; j++)
            printf("-");
        printf("\n");
    }
    getchar();
}

void swap(int src, int dest)
{
    hanoi[src].hanoi_ind--;  // 源柱子的栈顶标记下移
    hanoi[dest].hanoi_arr[hanoi[dest].hanoi_ind] = hanoi[src].hanoi_arr[hanoi[src].hanoi_ind];
    hanoi[dest].hanoi_ind++;
    draw();
}

void play(int src, int dest, int tmp, int num)
{
    if (num == 1)  // 如果只有一个铁饼
    {
        swap(src, dest);  // 把src上的铁饼放到dest上
        return;
    }
    play(src, tmp, dest, num - 1);
    swap(src, dest);
    play(tmp, dest, src, num - 1);
}

int main(void)
{
    draw();

    play(0, 1, 2, 7);  // 0 1 2 分别表示 3 个柱子的下标, 7 表示有7 个铁饼

    return 0;
}
