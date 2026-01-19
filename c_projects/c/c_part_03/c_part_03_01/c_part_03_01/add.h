#ifndef __ADD_H  // 如果没有定义过 __ADD_H 这个宏
#define __ADD_H  // 那么就定义 __ADD_H 这个宏

int add(int, int);  // 函数声明时, 可以省略形参的名字, 不能省略形参数据类型


#endif  // 结束 #ifndef 的预处理
/*
	这三行预处理是为了防止重复定义
	避免头文件被重复包含
*/