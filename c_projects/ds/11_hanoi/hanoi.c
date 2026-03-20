#include <stdio.h>
#include <stdlib.h>

struct hanoi  // 汉诺塔的结构
{
	int hanoi_arr[7];  // 栈式存储空间 (用数组模拟栈, 存储每个柱子上的盘子数量)
	int hanoi_ind;  // 栈顶元素
}hanoi[3] = { {{7,6,5,4,3,2,1}, 7}, {}, {} };  // 创建了 3 个柱子的数组

void draw(void)
{
	system("clear");  // 清屏

	int j = 0;
	for (int i = 0; i < 3; i++)  // i 循环, 表示 3 个柱子
	{
		for (j = 0; j < hanoi[i].hanoi_ind; j++)  // 打印当前柱子上的所有盘子（数字表示盘子大小）
			printf("%d", hanoi[i].hanoi_arr[j]);
		
		for (; j < 10; j++)  // 用 "-" 填充剩余位置，保证每行固定 10 个字符，便于对齐显示
			printf("-");
		
		printf("\n");  // 每个柱子打印完后换行
	}
	getchar();  // 等待用户按键，实现分步显示
}

void swap(int src, int dest)
{
	// 源柱子栈顶指针减 1（弹出盘子）
	hanoi[src].hanoi_ind--; 

	// 将源柱子栈顶的盘子复制到目标柱子的栈顶位置
	hanoi[dest].hanoi_arr[hanoi[dest].hanoi_ind] = hanoi[src].hanoi_arr[hanoi[src].hanoi_ind];

	// 目标柱子栈顶指针加 1 (压入盘子)
	hanoi[dest].hanoi_ind++;

	draw();  // 移动后重新绘制界面
}

void play(int num, int src, int dest, int tmp)
{
	if (num == 1)  // 如果只有一个盘子
	{
		swap(src, dest);  // 直接把 src 上的盘子放到 dest 上
		return;
	}

	// 将 num-1 个盘子从 src 移到 tmp, 借助 dest
	play(num - 1, src, tmp, dest);

	// 将最大的盘子从 src 移到 dest
	swap(src, dest);

	// 将 num-1 个盘子从 tmp 移到 dest, 借助 src
	play(num - 1, tmp, dest, src);
}

int main(void)
{
	draw();  // 显示初始状态

	// 开始游戏: 移动 7 个盘子, 从柱子 0 移到柱子 1, 借助柱子 2
	play(7, 0, 1, 2);

	return 0;
}
/*
	play(3, 0, 1, 2)          // 移动 3 个盘子：从 0 到 1，借助 2
	├── play(2, 0, 2, 1)      // 移动 2 个盘子：从 0 到 2，借助 1
	│   ├── play(1, 0, 1, 2)  // 移动 1 个盘子：从 0 到 1，借助 2
	│   │   └── swap(0, 1)    // 实际移动：1 从 0→1
	│   ├── swap(0, 2)        // 移动：2 从 0→2
	│   └── play(1, 1, 2, 0)  // 移动 1 个盘子：从 1到 2，借助 0
	│       └── swap(1, 2)    // 实际移动：1 从 1→2
	├── swap(0, 1)            // 移动：3 从 0→1
	└── play(2, 2, 1, 0)      // 移动 2 个盘子：从 2 到 1，借助 0
		├── play(1, 2, 0, 1)  // 移动 1 个盘子：从 2 到 0，借助 1
		│   └── swap(2, 0)    // 实际移动：1 从 2→0
		├── swap(2, 1)        // 移动：2 从 2→1
		└── play(1, 0, 1, 2)  // 移动 1 个盘子：从 0 到 1，借助 2
			└── swap(0, 1)    // 实际移动：1 从 0→1
*/