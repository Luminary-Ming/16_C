/*
	字符数组
*/
#include <stdio.h>

void print(char *p, int size);

int main(void)
{
	char arr[5];
	char brr[5];

	printf("输入 5 个数字:\n");
	for (int i = 0; i < 5; i++)
	{
		// 注意 char 是 1 字节, 所以用 %hhd
		scanf("%hhd", arr + i);
		// 编译报警告类型不匹配, 执行报核心已转储
		//scanf("%d", arr + i);
	}

	printf("打印 arr 字符数组:\n");
	print(arr, 5);

	printf("=========================\n");

	printf("输入 5 个字母:\n");
	for (int i = 0; i < 5; i++)
	{
		// 录入字符格式的数据时, 空格符, 换行符也算录入的字符, 会存到数组中
		scanf("%c", brr + i);
	}
	printf("打印 brr 字符数组:\n");
	print(brr, 5);

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
	1 2 3 4 5
	打印 arr 字符数组:
	p[0] = 1
	p[1] = 2
	p[2] = 3
	p[3] = 4
	p[4] = 5
	=========================
	输入 5 个字母:
	a b c d e
	打印 brr 字符数组:
	p[0] = 10  ->  输入完 5 后按回车的回车符 '\n'  
	p[1] = 97  ->  a 的 ASCII 码
	p[2] = 32  ->  空格符 " " 的 ASCII 码
	p[3] = 98  ->  b 的 ASCII 码
	p[4] = 32  ->  空格符 " " 的 ASCII 码

	输入 5 个数字:
	1 2 3 4 5
	打印 arr 字符数组:
	p[0] = 1
	p[1] = 2
	p[2] = 3
	p[3] = 4
	p[4] = 5
	=========================
	输入 5 个字母:
	abcdefghijklmopqrstuvwxyz   ->  只会读取数组最大长度的字符, 不会造成数组越界
	打印 brr 字符数组:
	p[0] = 10  ->  输入完 5 后按回车的回车符 '\n' 
	p[1] = 97
	p[2] = 98
	p[3] = 99
	p[4] = 100
*/