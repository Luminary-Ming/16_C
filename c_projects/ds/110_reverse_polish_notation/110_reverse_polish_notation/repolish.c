#include "repolish.h"
#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int get_priority(char op)
{
	switch (op)
	{
		case '(': return 0;  // 优先级低
		case '+':
		case '-': return 1;  // 优先级中
		case '*': 
		case '/': return 2;  // 优先级高
		default: return -1;
	}
}

/*
	前缀表达式转为后缀表达式
*/
int infix_to_postfix(const char *infix, char *postfix)
{
	STACK *op_stack = NULL;  // op_stack 指向运算符的栈
	op_stack = stack_create();  // 开辟运算符栈的空间
	if (op_stack == NULL)
	{
		fprintf(stderr, "stack_create() Is Failed!\n");
		return -1;
	}

	int i = 0, j = 0;  // 循环变量
	int len = strlen(infix);  // 获取中缀表达式的字符串长度
	int op = 0;  // 存储出栈的运算符
	int cur_op = 0;  // 存储当前运算符
	int top_op = 0;  // 存储栈顶运算符
	while (i < len)
	{
		// 1. 跳过空格
		if (infix[i] == ' ')  
		{
			i++;
			continue;
		}

		// 2. 当前字符是数字
		if (infix[i] >= '0' && infix[i] < '9')
		{
			while (i < len && infix[i] >= '0' && infix[i] < '9')  // 处理多位数
			{
				postfix[j++] = infix[i++];  // 写入到后缀表达式中
			}
			postfix[j++] = ' ';  // 用空格隔开数字和运算符
		}
		// 3. 当前字符是 '('
		else if(infix[i]=='(')
		{
			stack_push(op_stack, infix[i]);  // 入栈
			i++;
		}
		// 4. 当前字符是 ')'
		else if(infix[i] == ')')
		{
			while (!is_empty(op_stack))  // 循环出栈,  
			{
				stack_pop(op_stack, &op);  // 出栈
				if (op == '(')
					break;
				postfix[j++] = op;  // 运算符
				postfix[j++] = ' ';  // 用空格隔开数字和运算符
			}
			i++;
		}
		// 4. 当前字符是 "+-*/"
		else if(strchr("+-*/",infix[i]))
		{
			cur_op = infix[i];  // 存储当前运算符
			while (!is_empty(op_stack))  // 循环栈中的所有运算符和 cur_op 作比较, 直到 cur_op > top_op 为止, 循环结束, cur_op 入栈
			{
				stack_pop(op_stack, &top_op);  // 弹出栈顶运算符
				if (get_priority(top_op) < get_priority(cur_op))  // 运算符比较优先级
				{
					stack_push(op_stack, top_op);  // 刚才弹出栈的运算符再入栈
					break;
				}
				postfix[j++] = top_op;  // 栈顶运算符优先级更大, 出栈存入后缀表达式中
				postfix[j++] = ' ';  // 用空格隔开数字和运算符
			}
			stack_push(op_stack, cur_op);  // cur_op 入栈
			i++;
		}
		else
		{
			printf("出现错误的字符 : %c\n", infix[i]);  // 打印
			stack_destroy(op_stack);  // 销毁栈
			return -2;
		}
	}

	while (!is_empty(op_stack))  // 再遍历栈中的元素, 弹出剩余元素
	{
		stack_pop(op_stack, &op);  // 出栈

		// 检查是否是合法的运算符
		if (op == '(')
		{
			printf("错误：括号不匹配！\n");
			stack_destroy(op_stack);
			return -1;
		}

		postfix[j++] = op;  // 判断
		postfix[j++] = ' ';
	}

	postfix[j - 1] = '\0';  // 把后缀表达式中的最后的空格替换成 '\0'
	stack_destroy(op_stack);  // 销毁栈

	return 0;
}

/*
	后缀表达式计算结果
*/
int repolish(const char *postfix, int *result)
{
	STACK *num_stack = NULL;
	num_stack = stack_create();  // 创建操作数的栈
	if (num_stack == NULL)
	{
		fprintf(stderr, "stack_create() Is Failed!\n");
		return -1;
	}

	int i = 0, j = 0;
	int len = strlen(postfix);
	int num = 0;  // 存储字符串转换后的整数
	int num1 = 0, num2 = 0;  // 存储两个操作数
	while (i < len)  // 遍历后缀表达式中的元素
	{
		if (postfix[i] == ' ')  // 跳过空格
		{
			i++;
			continue;
		}
		if (postfix[i] >= '0' && postfix[i] < '9')
		{
			num = 0;  // 每次使用清零
			while (i < len && postfix[i] >= '0' && postfix[i] < '9')  // 处理多位数
			{
				num = num * 10 + (postfix[i] - '0');  // 字符串转整型
				i++;
			}
			stack_push(num_stack, num);  // 入栈
		}
		else if (strchr("+-*/", postfix[i]))  // 当前字符是 "+-*/"
		{
			if (stack_pop(num_stack, &num2) == -1)  // 先弹出第二个操作数
			{
				printf("表达式有问题,(操作数少了)\n");  // 当前字符是 "+-*/" , 要出栈两个操作数做计算, 但是现在 -1 栈空了, 没操作数了, 所以说操作数少了
				stack_destroy(num_stack);
				return -2;
			}

			if (stack_pop(num_stack, &num1) == -1)  // 再弹出第一个操作数
			{
				printf("表达式有问题,(操作数少了)\n");
				stack_destroy(num_stack);
				return -3;
			}

			switch (postfix[i])
			{
				case '+': stack_push(num_stack, num1 + num2); break;
				case '-': stack_push(num_stack, num1 - num2); break;
				case '*': stack_push(num_stack, num1 * num2); break;
				case '/': 
					if (num2 == 0)//判断除数是否为0
					{
						printf("表达式中除数为0\n");
						stack_destroy(num_stack);
						return -4;
					}
					stack_push(num_stack, num1 / num2); break;
				default: break;
			}
			i++;
		}
	}

	if (stack_pop(num_stack, result) == -1)  // 弹出计算结果
	{
		printf("表达式有问题!\n");
		stack_destroy(num_stack);
		return -5;
	}

	if (!is_empty(num_stack))  // 如果栈中还有操作数, 说明操作数多了, 返回错误码
	{
		printf("表达式有问题,(操作数过多)!\n");
		stack_destroy(num_stack);
		return -6;
	}

	stack_destroy(num_stack);
	return 0;
}