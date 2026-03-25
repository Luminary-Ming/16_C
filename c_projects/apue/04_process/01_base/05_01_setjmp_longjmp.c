#include <stdio.h>
#include <setjmp.h>

jmp_buf env;  // 存储有关调用环境的各种信息

int div(int num1, int num2);

int main(void)
{
	int num1 = 0, num2 = 0;
	int sum = 0;

	if (setjmp(env) == 0)  // 设置跳转位置
		printf("请输入两个整数 : ");
	else  // 从 longjmp() 跳转过来
		printf("请重新输入两个整数(除数不能为 0) : ");

	scanf("%d %d", &num1, &num2);  // 录入两个整型数

	sum = div(num1, num2);  // 进行除法运算

	printf("%d / %d = %d\n", num1, num2, sum);

	return 0;
}

int div(int num1, int num2)
{
	if (num2 == 0)  // 检出除数是否为 0
		longjmp(env, 1);  // 跳转

	return num1 / num2;
}