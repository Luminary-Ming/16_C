/*
	装箱与分配问题
	把 1000 个 cpu 装入 10 个箱子里面, 将来有人来取 CPU 时，输入一个数字 (要取的数量),
	都可以整箱的给这个人（不能开箱零散取）, 编写程序实现 10 个箱子的存储，以及取 CPU 的过程

	思路 : 二进制拆分法, 前九个箱子是 2 的幂次, 最后一个是剩余值
*/
#include <stdio.h> 

#define BOX_SIZE 10  // 十个箱子
#define CPU 1000  // cpu 数量

void init_box(int *p, int num, int size);  // 装箱
void take_out(int *p, int num, int size);  // 分配的情况

int main(void)
{
	int box[10] = { 0 };  // 十个箱子
	int take_the_cpu;  // 拿走的 cpu 数量

	init_box(box, CPU, BOX_SIZE);

	printf("请输入要取走的 CPU 数量( 1 ~ 1000) : ");
	scanf("%d", &take_the_cpu);

	take_out(box, take_the_cpu, BOX_SIZE);
	return 0;
}

void init_box(int *p, int cpu_totals, int size) {

	int sum = 0;  // 已装箱的 cpu 数量
	int n = 1;  // 给箱子里装入的 cpu 数量

	for (int i = 0; i < size; i++) {
		// 最后一个箱子装剩值
		if (i == size - 1) {
			p[i] = cpu_totals - sum;
			break;  // 最后一个箱子装完跳出循环
		}

		p[i] = n;  // 第一个箱子装 1 个
		n <<= 1;  // 左移一位(相当于 * 2), 给下一个箱子装的数量
		sum += p[i];
	}
}

void take_out(int *p, int num, int size) {
	// 从大箱到小箱遍历
	for (int i = size - 1; i >= 0; i--) {
		if (p[i] <= num) {
			printf("box[%d] = %d\n", i, p[i]);
			num -= p[i];
		}
	}
}