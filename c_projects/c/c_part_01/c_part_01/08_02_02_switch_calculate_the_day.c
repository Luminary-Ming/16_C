/*
    录入年月日, 计算是这一年的第几天
*/
#include <stdio.h>

int main(void)
{
    int year = 0, month = 0, day = 0;  // 分别存储录入的年 月 日
    int days = 0;  // 存储计算的结果
    int flag = 0;  // 用来标记是否是闰年( 闰年 flag=1  平年 flag=0)

    scanf("%d-%d-%d", &year, &month, &day);  // 录入年月日

    flag = (!(year % 4) && (year % 100)) || !(year % 400);
/*
    switch (month)
    {
    case 1: days += day; break;
    case 2: days += (31 + day); break;
    case 3: days += (59 + flag + day); break;
    case 4: days += (90 + flag + day); break;
    case 5: days += (120 + flag + day); break;
    case 6: days += (151 + flag + day); break;
    case 7: days += (181 + flag + day); break;
    case 8: days += (212 + flag + day); break;
    case 9: days += (243 + flag + day); break;
    case 10: days += (273 + flag + day); break;
    case 11: days += (304 + flag + day); break;
    case 12: days += (334 + flag + day); break;
    }
*/
    switch (month)
    {
        case 12: days += 30;
        case 11: days += 31;
        case 10: days += 30;
        case 9: days += 31;
        case 8: days += 31;
        case 7: days += 30;
        case 6: days += 31;
        case 5: days += 30;
        case 4: days += 31;
        case 3: days += (28 + flag);
        case 2: days += 31;
        case 1: days += day;
    }

    printf("%d年%d月%d日是这一年的第%d天!\n", year, month, day, days);

    return 0;
}
/*
    2026-12-31
    2026年12月31日是这一年的第365天!

    2000-12-31
    2000年12月31日是这一年的第366天!
*/