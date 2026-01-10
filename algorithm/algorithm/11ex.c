/*
	两个乒乓球队进行比赛，各出三人。
	甲队为 a,b,c 三人，乙队为 x,y,z 三人。
	已抽签决定比赛名单。有人向队员打听比赛的名单。
	a 说他不和x 比，c 说他不和 x , z 比，请编程序找出三队赛手的名单。
*/

#include <stdio.h>

int main()
{
	char a, b, c;  // a, b, c的对手

	for (a = 'x'; a <= 'z'; a++) {
		for (b = 'x'; b <= 'z'; b++) {
			for (c = 'x'; c <= 'z'; c++) {
				// a, b, c的对手各不相同
				if (a != b && a != c && b != c) {
					// a 不和x 比，c 不和 x , z 比
					if ((a != 'x') && c != 'x' && c != 'z') {
						printf("比赛对阵名单：\n");
						printf("a vs %c\n", a);
						printf("b vs %c\n", b);
						printf("c vs %c\n", c);
					}
				}
			}
		}
	}

	return 0;
}