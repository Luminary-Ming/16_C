#ifndef CUBOID_H
#define CUBOID_H

#include "rectangle.h"

// 长方体类，继承矩形类
class Cuboid : public Rectangle
{
public:
	// 构造函数，初始化列表的方式进行初始化
	Cuboid(int len, int width, int hight) :Rectangle(len, width), m_hight(hight) {}
	// 实现基类中的 area volume 面积、体积虚函数
	int area();   // 面积
	int volume(); // 体积
private:
	int m_hight;
};

#endif