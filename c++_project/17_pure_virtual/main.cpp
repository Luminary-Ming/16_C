#include <iostream>
#include "cube.h"
#include "cuboid.h"
#include "rectangle.h"

using namespace std;

/*
	抽象类
	包含纯虚函数的类，叫做抽象类
	如果抽象类中存在没有实现的纯虚函数，则该抽象类不能实例化对象
*/

int main(void)
{
	//Rectangle r1(2, 3);  // 实例化对象 r1
	// error：不允许使用抽象类类型 "Rectangle" 的对象：纯虚拟函数 "Line:volume" 没有强制替代项
	// 因为 Rectangle 类继承了 Line 类中的两个纯虚函数，但只实现了一个 area 纯虚函数，所以不允许实例化对象

	Cuboid cuboid(2, 3, 4); // 实例化立方体对象 cuboid
	Cube cube(2);           // 实例化立方体对象 cube

	cout << "长方体的面积和体积分别是：" << cuboid.area() << " " << cuboid.volume() << endl;
	
	cout << "立方体的面积和体积分别是：" << cube.area() << " " << cube.volume() << endl;
	
	return 0;
}

/*
	长方体的面积和体积分别是：52 24
	立方体的面积和体积分别是：24 8
*/