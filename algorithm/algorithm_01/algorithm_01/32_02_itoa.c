/*
	实现 myitoa() 函数, 整型数字转为字符串形式的数字
*/
#include <stdio.h>
#include <string.h>

int calculate_digits(int num);  // 计算数字位数
char *myitoa(char *str, int num, int digits, int is_negative);


int main(void)
{
	int num;
	printf("\n请输入数字: ");
	scanf("%d", &num);

	int is_negative = 0;  // 负数标志

	if (num < 0) {
		is_negative = 1;  // 是负数
		num = -num;
	}

	int digits = calculate_digits(num);

	char str[digits + 1];  // 加上一个尾零

	printf("%d %d -> \"%s\" = \"%s\"", digits, num, myitoa(str, num, digits, is_negative), str);

	return 0;
}

int calculate_digits(int num)
{
	int digits = num == 0 ? 1 : 0;  // 如果 num 是 0 , 那么 digits 就是 1 位
	int temp = num;
	while (temp)
	{
		temp /= 10;
		digits++;
	}
	return digits;
}

char *myitoa(char *str, int num, int digits, int is_negative)
{
	if (num == 0) str[0] = '0';
		
	int i = 0;
	while (num)
	{
		str[digits - 1 - i] = num % 10 + '0';
		num /= 10;
		i++;
	}

	str[digits] = '\0';

	if (is_negative)  // 如果是负数
	{
		for (int j = digits; j >= 0; j--)  // 把所有字符向后移动一位
		{
			str[j + 1] = str[j];
		}
		str[0] = '-';  // 补充负号
	}
	return str;
}