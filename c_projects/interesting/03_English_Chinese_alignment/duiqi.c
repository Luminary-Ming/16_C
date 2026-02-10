#include <stdio.h>
#include <locale.h>
#include <wchar.h>

// 计算字符串在终端中的显示宽度（中文字符算2个宽度）
int get_display_width(const char *str)
{
    int width = 0;
    unsigned char *p = (unsigned char *)str;

    while (*p)
    {
        // 判断是否为UTF-8中文字符（首字节 >= 0xE0）
        if (*p >= 0xE0)
        {
            width += 2;  // 中文字符在终端显示占2个英文字符宽度
            p += 3;      // 跳过3个UTF-8字节
        }
        else
        {
            width += 1;
            p += 1;
        }
    }
    return width;
}

// 打印带对齐的单元格
void print_cell(const char *str, int width)
{
    int display_width = get_display_width(str);
    printf("%s", str);
    // 补齐空格
    for (int i = 0; i < width - display_width; i++)
    {
        printf(" ");
    }
}

void print_cell_int(int num, int width)
{
    printf("%d", num);
    // 计算数字占用的宽度（假设最大9999）
    int num_width = 0;
    int temp = num;
    if (temp == 0) num_width = 1;
    while (temp != 0)
    {
        num_width++;
        temp /= 10;
    }
    // 补齐空格
    for (int i = 0; i < width - num_width; i++)
    {
        printf(" ");
    }
}




typedef struct
{
    int id;
    char name[50];
    int year;
    int episodes;
    int duration;
} Film;

int main()
{
    Film films[] = {
        {1, "嗨嗨嗨", 1877, 2, 90},
        {2, "飞驰人生2", 2024, 1, 121},
        {3, "jojo的奇妙冒险", 2012, 26, 24},
        {4, "轻音少女", 2011, 25, 23},
    };
    int count = 4;

    // 定义每列的宽度（根据实际内容调整）
    int col_widths[] = { 4, 20, 10, 6, 10 };

    // 打印表头
    printf("| ");
    print_cell("id", col_widths[0]);
    printf(" | ");
    print_cell("作品名字", col_widths[1]);
    printf(" | ");
    print_cell("上映年份", col_widths[2]);
    printf(" | ");
    print_cell("集数", col_widths[3]);
    printf(" | ");
    print_cell("每集时长", col_widths[4]);
    printf(" |\n");

    // 打印分隔线
    printf("|");
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < col_widths[i] + 2; j++)
        {
            printf("-");
        }
        printf("|");
    }
    printf("\n");

    // 打印数据行
    for (int i = 0; i < count; i++)
    {
        printf("| ");
        print_cell_int(films[i].id, col_widths[0]);
        printf(" | ");
        print_cell(films[i].name, col_widths[1]);
        printf(" | ");
        print_cell_int(films[i].year, col_widths[2]);
        printf(" | ");
        print_cell_int(films[i].episodes, col_widths[3]);
        printf(" | ");
        print_cell_int(films[i].duration, col_widths[4]);
        printf(" |\n");
    }

    return 0;
}