/*
	输入某年某月某日，判断这一天是这一年的第几天？
*/
#include <stdio.h>

int main()
{
	int year, month, day, total_days;  // 年 月 日 第几天(总天数)
	scanf("%d-%d-%d", &year, &month, &day);

	// 判断是否是闰年
	int is_leap = 0;
	if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) {
		is_leap = 1;
	}

	// 计算天数, 默认是平年
	switch (month) {
	case 1: total_days = day; break;
	case 2: total_days = 1 * 31 + day; break;
	case 3: total_days = 1 * 31 + 28 + day; break;
	case 4: total_days = 2 * 31 + 28 + day; break;
	case 5: total_days = 2 * 31 + 1 * 30 + 28 + day; break;
	case 6: total_days = 3 * 31 + 1 * 30 + 28 + day; break;
	case 7: total_days = 3 * 31 + 2 * 30 + 28 + day; break;
	case 8: total_days = 4 * 31 + 2 * 30 + 28 + day; break;
	case 9: total_days = 5 * 31 + 2 * 30 + 28 + day; break;
	case 10: total_days = 5 * 31 + 3 * 30 + 28 + day; break;
	case 11: total_days = 6 * 31 + 3 * 30 + 28 + day; break;
	case 12: total_days = 6 * 31 + 4 * 30 + 28 + day; break;
	default:
		printf("请输入正确的月份 1- 12");
		break;
	}

	// 闰年
	if (is_leap) {
		// 从 3 月开始
		if (month >= 3) {
			total_days++;  // 加一天 (2月29日)
		}
	}
	// 平年
	else {
		if (month == 2 && day == 29) {
			printf("%d年不是闰年, 没有2月29日", year);
			return 1;
		}
	}

	printf("%d年%d月%d日是今年的第 %d 天", year, month, day, total_days);
	return 0;
}