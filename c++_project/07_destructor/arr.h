#ifndef ARR_H
#define ARR_H

class VLA
{
public:
	VLA(int len = 0);   // 构造函数 (带默认参数)
	VLA(const VLA &v);  // 拷贝构造函数 (深拷贝)
	~VLA();             // 析构函数 (析构函数不能重载)

	int at(int i);                   // 返回下标为 i 的元素
	int setValue(int i, int value);  // 给下标为 i 的元素赋 value 值
	void show();                     // 遍历可变长数组

private:
	int *m_a;   // 保存可变长数组的起始地址
	int m_len;  // 保存可变长数组的长度
};

#endif