#include <stdio.h>
/*
	成绩评定：
		不及格：0-59
		及格：60-69
		中等：70-79
		良好：80-89
		优秀：90-100
*/
int main()
{
	int score = 0;
	scanf("%d", &score);

	if (score >= 0 && score < 60) {
		printf("不及格");
	}
	else if (score >= 60 && score < 70) {
		printf("及格");
	}
	else if (score >= 70 && score < 80) {
		printf("中等");
	}
	else if (score >= 80 && score < 90) {
		printf("良好");
	}
	else if (score >= 90 && score <= 100) {
		printf("优秀");
	}
	else {
		printf("输入错误");
	}
}