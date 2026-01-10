/*
	输入 1~10 之间的一个数字，输出它对应的英文单词
*/
#include <stdio.h>

int main()
{
	int n = 1;
	scanf("%d", &n);

	if (n < 1 || n > 10) {
		printf("请输入 1~10 之间的一个数字");
		return 1;
	}

	switch (n) {
		case 1: printf("one");
			break;
		case 2: printf("two");
			break;
		case 3: printf("three");
			break;
		case 4: printf("four");
			break;
		case 5: printf("five");
			break;
		case 6: printf("six");
			break;
		case 7: printf("seven");
			break;
		case 8: printf("eight");
			break;
		case 9: printf("nine");
			break;
		case 10: printf("ten");
			break;
		//  default 可以省略，因为输入已经验证过了
	}
}