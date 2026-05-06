#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "line.h"

// 矩形类，继承线类
class Rectangle : public Line
{
public:
	// 构造函数，初始化列表的方式进行初始化
	Rectangle(int len, int width) : Line(len), m_width(width) {};
	// 实现基类中的 area 面积虚函数
	int area();  // 面积
protected:
	int m_width;  // 宽
};

#endif