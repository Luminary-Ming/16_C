#ifndef CUBE_H
#define CUBE_H

#include "cuboid.h"

// 立方体类，继承长方体类
class Cube : public Cuboid
{
public:
	// 构造函数，初始化列表的方式进行初始化
	Cube(int a) : Cuboid(a, a, a) {};
	// 面积和体积函数直接从 Cuboid 继承了，直接调用
};

#endif