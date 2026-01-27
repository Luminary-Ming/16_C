#include "contact.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>  // isalpha toupper 函数的头文件

// 判断字符串是不是空串, 空白字符
static int is_blank_str(const char *str)
{
	if (str == NULL)
		return 1;
	for (int i = 0; str[i] != '\0'; i++)
	{
		if (!isblank(str[i]))
			return 0;  // 发现有非空白字符, 返回假
	}
	return 1;
}

// 判断字符串开头是否是 UTF-8 编码的汉字
static int is_utf8_chinese(const char *str) 
{
	if (str == NULL || str[0] == '\0')
		return 0;

	unsigned char first = (unsigned char)str[0];
	// UTF-8 汉字是 3 字节编码，首字节范围：0xE4-0xE9
	if (first >= 0xE4 && first <= 0xE9)
	{
		if (str[1] != '\0' && str[2] != '\0')
		{
			unsigned char second = (unsigned char)str[1];
			unsigned char third = (unsigned char)str[2];

			// UTF-8 编码规则：后续字节以 10 开头（0x80-0xBF）
			if ((second & 0xC0) == 0x80 && (third & 0xC0) == 0x80)
				return 1;	
		}
	}
	return 0;
}

// 获取名字首字母
char get_initial(const char *name)
{}

// 打印联系人
void contact_print(Contact *contact)
{}