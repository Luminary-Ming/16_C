#ifndef LINE_H
#define LINE_H

// 线类
class Line
{
public:
	// 构造函数，初始化列表的方式进行初始化
	Line(int len) : m_len(len) {};
	// 纯虚函数
	virtual int area() = 0;    // 面积
	virtual int volume() = 0;  // 体积
protected:
	int m_len;  // 长
};

#endif