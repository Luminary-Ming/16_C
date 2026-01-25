/*
	逆波兰序 - 后缀表达式
*/
#include "repolish.h"
#include  <stdio.h>
#include  <string.h>

int main(void)
{
	char infix[32] = { 0 };  // 存储客户录入的中缀表达式
	char postfix[32] = { 0 };  // 存储转换之后的后缀表达式

	printf("请输入中缀表达式(表达式中可以录入空格) : ");

	fgets(infix, sizeof(infix), stdin);  // 从标准输入流中获取字符串存储到 infix 数组中
	infix[strcspn(infix, "\n")] = '\0';  // 把换行符换成 '\0'

	printf("infix = %s\n", infix);

	int ret = infix_to_postfix(infix, postfix);  // 前缀表达式转为后缀表达式
	if (ret != 0)
	{
		fprintf(stderr, "infix_to_postfix() Is Failed!\n");
		return -1;
	}

	printf("转换之后的,后缀表达式 : %s\n", postfix);

	int result = 0;
	ret = repolish(postfix, &result);  // 计算结果
	if (ret == 0)
		printf("result = %d\n", result);

	return 0;
}