/*
    字符指针
*/
#include <stdio.h>

int main(void)
{
    char arr[10] = "hello";  // 存储字符串中的每个字符 ('h' 'e' 'l' 'l' 'o' '\0' '0' '0' '0'....)
    //char *str = "world";
    const char *str = "world";  // str 是一个指针, 存储的是字符串常量(read only)的首地址

    printf("arr = %s\n", arr);  // arr = hello
    printf("str = %s\n", str);  // str = world

    scanf("%s", arr);  // ✔️  录入数组成员的值
    scanf("%s", str);  // ❌ str 指向字符串常量区, 只读不能修改

    //arr = "narcissus";  // ❌ 数组名是指针常量, 不能做赋值运算
    str = "narcissus";  // ✔️  str 是一个指针, 指向了一个新的字符串常量

    printf("str = %s\n", str);  // str = narcissus


    arr[1] = '$';  //  ✔️ 修改数组成员的值
    //str[1] = '$';  // ❌ str 指向字符串常量区, 不能修改, 不能做赋值运算

    return 0;
}