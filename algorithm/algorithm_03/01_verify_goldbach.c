/*
	验证输入任意偶数都满足哥德巴赫猜想

	哥德巴赫猜想的内容是：任一大于 2 的偶数都可写成两个质数之和

	思路:
	● 判断质数: 写一个函数来检查一个数是否为质数
	● 拆分偶数: 对于给定偶数 num, 将其拆分为 i 和 num-i
	● 循环验证: 如果 i 和 num-i 同为质数, 则猜想在该数字上成立
*/
#include <stdio.h>

//判断一个数是否是质数
int is_prime(int num)
{
	if (num <= 1) return 0;  // 0 和 1 不是质数
	if (num == 2) return 1;  // 2 是最小的质数
	if (num % 2 == 0) return 0;  // 偶数不是质数

	for (int i = 3; i * i <= num; i += 2)  // 检查奇数因子
		if (num % i == 0)
			return 0;

	return 1;
}

// 验证哥德巴赫猜想的核心逻辑
void verify_goldbach(int num)
{
	if (num <= 2 || num % 2 != 0)
	{
		printf("请输入一个大于 2 的偶数!\n");
		return;
	}

	int flag = 0;  // 验证标志: 猜想错误为 0, 猜想正确为 1

	// 循环只到 num/2 就行, 因为对称性, 例如验证 10 时，找到 3+7 就不必再去找 7+3 了
	for (int i = 2; i <= num / 2; i++)
	{
		if (is_prime(i) && is_prime(num - i))  // 任一大于 2 的偶数都可写成两个质数之和, 有一对符合就行
		{
			printf("%d = %d + %d\n", num, i, num - i);
			flag = 1;  // 猜想正确
			break; // 找到一组即可
		}

	}


	if (flag == 1)
		printf("验证成功！4 到 %d 之间的所有偶数均满足哥德巴赫猜想。\n", num);
}

int main(void)
{
	printf("请输入一个大于 2 的偶数: ");

	int num;
	if (scanf("%d", &num) != 1)  // 检查输入是否成功, scanf 返回成功读取的变量个数
	{
		printf("输入无效，请输入一个整数！\n");
		return -1;
	}

	verify_goldbach(num);

	return 0;
}
/*
	for (int i = 3; i * i <= num; i += 2)
		if (num % i == 0)
			return 0;

	为什么只需要检查到 sqrt(num) ?
	如果一个数 num 是合数，它必然可以分解为两个因数 a 和 b, 即 a * b = num
	● 对称性: 这两个因子中必然有一个 ≤ √num, 另一个 ≥ √num
	● 示例: num = 33,  1 3 √33 11 33
	● 结论: 所以你在 3 到 √33 之间没有找到能整除 33 的数, 那么在 √33 之后也就绝不可能找到
	● i += 2 只检查奇数因子, 因为只有 奇数 = 奇数 * 奇数

	打根号: 按住 Alt 键小键盘输入 41420 然后松开 Alt
	小于等于号: 41436, 大于等于号 41437
*/