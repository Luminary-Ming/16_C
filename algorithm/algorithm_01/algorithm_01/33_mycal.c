#include <stdio.h>
#include <time.h>
#include <string.h>


void mycal(int argc, char *argv[]);  // 打印日历
void print_today_cal();  // 打印今天的日历
void print_month_cal_year_cal(char *argv[]);  // 打印月历或年历
void print_year_and_month_cal(char *argv[]);  // 打印某年某月的日历
const char *get_current_time();  // 获取当前系统时间
int calculate_the_day_of_the_week(int year, int mouth, int day);  // 计算某天是星期几

int main(int argc, char *argv[])
{
	if (argc < 1)
	{
		printf("请输入命令!");
		return 1;
	}
	
	if (argc > 3)  // 超过三个参数报错
	{
		printf("命令最多三个参数! 例如: \"./mycal 2 2026\"");
		return -1;
	}

	mycal(argc, argv);

	return 0;
}

void mycal(int argc, char *argv[])
{
	// ./mycal
	// ./mycal 2  /  ./mycal 2026
	// ./mycal 2 2026
	
	switch (argc) 
	{
		case 1: print_today_cal(); break;  // 打印今天的日历
		case 2: print_month_cal_year_cal(argv); break;  // 打印月历或年历
		case 3: print_year_and_month_cal(argv); break;  // 打印某年某月的日历
	}

}


void print_today_cal()
{
	const char *date_str = get_current_time();
	//printf("当前时间: %s\n", date_str);

	char buffer[20];
	strcpy(buffer, date_str);  // 复制 date_str (因为 strtok 会破坏字符串)

	char *year_str = strtok(buffer, "-");
	char *month_str = strtok(NULL, "-");
	char *day_str = strtok(NULL, "-");

	if (year_str && month_str && day_str) {
		int year = myatoi(year_str);
		int month = myatoi(month_str);
		int day = myatoi(day_str);
	}

	// printf("  year = %s, month = %s, day = %s\n", year_str, month_str, day_str);
	

}
void print_month_cal_year_cal(char *argv[])
{}

void print_year_and_month_cal(char *argv[])
{}


const char *get_current_time()
{
	time_t current_time = time(NULL);  // 获取当前系统时间
	struct tm *timeinfo = localtime(&current_time);  // 转换为本地时间结构体
	static char date_str[20];  // 定义存储字符串的缓冲区
	strftime(date_str, sizeof(date_str), "%Y-%m-%d", timeinfo);  // 格式化为"年-月-日"的字符串格式
	return date_str;
}

int calculate_the_day_of_the_week(int year, int mouth, int day)
{}
