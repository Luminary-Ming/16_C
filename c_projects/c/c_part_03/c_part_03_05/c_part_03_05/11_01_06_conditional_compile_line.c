#include <stdio.h>

#line 9527 "myline.c"  // 手动指定后续代码的行号 9527 以及文件名为 myline.c
// 行号从这行开始是 9527
int main(void)  // 9528
{   // 9529
	printf("Filename : %s Line : %d\n", __FILE__, __LINE__);  // Filename : myline.c Line : 9530
    // 9531
	printf("Filename : %s Line : %d\n", __FILE__, __LINE__);  // Filename : myline.c Line : 9532
	return 0;
}