/*
	字符数组
*/
#include <stdio.h>

void print(char *p, int size);

int main(void)
{
	char arr[5];

	printf("输入一个字符串:\n");

	// 录入字符串, 读取单个字符存到数组中
	scanf("%s", arr);  // arr 数组首成员的地址

	printf("打印 arr 字符数组:\n");
	print(arr, 5);

	return 0;
}

void print(char *p, int size)
{
	for (int i = 0; i < size; i++)
	{
		printf("p[%d] = %d\n", i, p[i]);
	}

}
/*
	输入一个字符串:
	abc de                ->  遇到空白符 (空格 Tab 换行符) 就会被截断
	打印 arr 字符数组:
	p[0] = 97
	p[1] = 98
	p[2] = 99
	p[3] = 0
	p[4] = 0

	输入一个字符串:
		abc               ->  跳过前面的空白符, 从第一个非空白字符开始读取
	打印 arr 字符数组:
	p[0] = 97
	p[1] = 98
	p[2] = 99
	p[3] = 0
	p[4] = 0

	输入一个字符串:
	abcdefghijklmnopqrstuvwxyz   ->  字符数组会把这 26 个字符存到数组中, 造成数组越界
	打印 arr 字符数组:
	p[0] = 97
	p[1] = 98
	p[2] = 99
	p[3] = 100
	p[4] = 101
	*** stack smashing detected ***: terminated
*/