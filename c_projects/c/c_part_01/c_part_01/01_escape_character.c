/*
	转义字符
*/
#include <stdio.h>

int main(void)
{
	// '\0' 尾零, 它是字符串结束符
	printf("Hello\0World!");  // 打印 : Hello
	printf("\n\n");

	// '\n' 换行符, 将光标移动到下一行的行首
	printf("Hello\nWorld!");  
	// 打印 :	Hello
	//	        World!
	printf("\n\n");

	// '\r' 回车符, 将光标移动到当前行的行首 
	/*
		执行过程 :
		第1步：打印 "0123456789Hello"
		控制台显示：0123456789Hello, 光标位置：在 'o' 后面

		第2步：遇到 \r, 光标移动到行首（第一个字符 '0' 的位置）

		第3步：打印 "World!"
		从光标当前位置（行首）开始覆盖：
		覆盖：World!6789Hello
		win10 终端打印完后面会有一个空格
		所有最终显示结果为：World! 789Hello
	*/
	printf("0123456789Hello\rWorld!");  // 打印 :	World! 789Hello
	printf("\n\n");

	// '\t' 制表符(相当于 Tab 键), 将光标移动到下一个制表位, 一般为8个字符
	// 制表符（Tab）不是简单地替换为固定数量的空格，而是跳到下一个制表位（通常每8个字符一个制表位）。
	// "He" 占 2 个字符, 所以 \t 会填充 6 个字符宽度到达 8
	// "o Wo" 占 4 个字符, 所以 \t 会填充 4 个字符宽度到 8 
	printf("He\tll\to Wo\trld!");  // 打印 : He      ll      o Wo    rld!
	printf("\n\n");

	// '\v' 垂直制表符, 将光标移动到下一个垂直制表位
	printf("He\vll\voWo\vrld!");	
	// 打印 : He
	//          ll
	//	          oWo
	//               rld!

	return 0;
}