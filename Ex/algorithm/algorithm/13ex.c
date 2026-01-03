/*
	输入某三角形的三个边的长度，判断出这是个什么三角形（等腰、等边、任意，或不能构成）。
*/

#include <stdio.h>

int main()
{
	int a, b, c;
	scanf("%d %d %d", &a, &b, &c);

	if ((a + b > c) && (a + c > b) && (b + c > a)) {
		
		if ((a * a + b * b == c * c) || (a * a + c * c == b * b) || (b * b + c * c == a * a)) {
			printf("能构成直角三角形");
		}
		else if ((a == b) || (a == c) || (b == c)) {
			if ((a == b) && (a == c)) {
				printf("能构成等边三角形");
			}
			else {
				printf("能构成等腰三角形");
			}
		}
		else { 
			printf("能构成三角形"); 
		}
	}
	else {
		printf("不能构成三角形");
	}

	return 0;
}