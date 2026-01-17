
#include <stdio.h>
#include <time.h>

int main(void)
{
	time_t current_time = time(NULL);  // 获取当前系统时间
	struct tm *timeinfo = localtime(&current_time);
	char date_str[20];
	strftime(date_str, sizeof(date_str), "%Y-%m-%d", timeinfo);
	printf("当前时间: %s\n", date_str);

	return 0;
}

