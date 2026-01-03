/*
	输入一行字符，分别统计出其中英文字母、空格、数字和其它字符的个数。
*/
#include <stdio.h>

int main()
{
	
	char ch;
	int alphabet = 0, space = 0, number = 0, other = 0;


	// getchar() 读取后赋值给 ch, 然后 ch 判断是否等于 '\n'
	while ((ch = getchar()) != '\n') {
		if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
			alphabet++;
		}
		else if (ch == ' ') {
			space++;
		}
		else if (ch >= '0' && ch <= '9') {
			number++;
		}
		else {
			other++;
		}
	}

	printf("\n=== 统计结果 ===\n");
	printf("英文字母：%d 个\n", alphabet);
	printf("数字字符：%d 个\n", number);
	printf("空格字符：%d 个\n", space);
	printf("其他字符：%d 个\n", other);

	return 0;
}
/*
	区别 :
		函数		getchar()									scanf()
		功能		只能读取单个字符							可以读取多种类型数据（整数、浮点数、字符串等）
		参数		无参数										需要格式字符串和变量地址
		返回值		返回读取的字符（int类型）					返回成功读取的项目数量
		输入缓冲	会读取缓冲区的所有字符（包括空格、换行）	根据格式符跳过空白字符
		效率		较高（简单）								较低（需要解析格式）
		使用场景	字符输入、菜单选择、逐字符处理				格式化输入、多种类型数据输入


	while(getchar() != '\n');  //  // 清空缓冲区
	getchar()被调用 → 读取一个字符
	判断该字符是否等于'\n'
	如果不等，继续循环
	如果等于，退出循环
	当getchar()读取到换行符 '\n' 时：
	然后判断'\n' != '\n' → false
	循环结束
	此时换行符已经被消耗掉了，不在缓冲区里！


	假设用户输入："Tom"然后回车
	缓冲区初始状态：'T' 'o' 'm' '\n'
	scanf("%s", name);  // 读取"Tom"（遇到空格/换行停止）
	缓冲区剩余：'\n'
	while(getchar() != '\n');
	判断：'\n' != '\n' → false
	循环结束
	'\n'已经被读取并丢弃
	ch = getchar();  // 现在缓冲区是空的！所以会等待用户输入
*/