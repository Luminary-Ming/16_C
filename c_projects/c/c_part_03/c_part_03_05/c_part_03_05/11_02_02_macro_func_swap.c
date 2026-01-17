/*
	宏函数
	实现交换两个变量的值
*/
#include <stdio.h>

#define SWAP1(A,B) {(A)=(A)+(B); (B)=(A)-(B); (A)=(A)-(B); }
#define SWAP2(A,B) {(A)=(A)^(B); (B)=(A)^(B); (A)=(A)^(B); }

// '\'（反斜杠）表示续行符
#define SWAP3(A,B) \  
	{ \
		typeof (A) (TMP); \
		(TMP) = (A); \
		(A) = (B); \
		(B) = (TMP); \
	}

int main(void)
{
	int a = 13, b = 7;
	printf("a = %d, b = %d\n", a, b);
	SWAP3(a, b);
	printf("a = %d, b = %d\n", a, b);

	return 0;
}