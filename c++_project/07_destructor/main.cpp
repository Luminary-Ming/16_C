#include "arr.h"

int main(void)
{
	VLA v1(10);  // 实例化 v1 对象, 拥有 10 个元素 (栈区对象, 作用域结束时自动销毁)

	for (int i = 0; i < 10; i++)
		v1.setValue(i, 100 + i);

	v1.show();


	VLA v2(v1);  // 实例化 v2 对象, v2 深拷贝 v1 对象 (栈区对象, 作用域结束时自动销毁)
	
	v2.setValue(0,114511);  // v2 对象中数组下标为 0 的元素设置为 114511 (深拷贝, v1 不受影响)
	v2.show();


	VLA *v3 = new VLA(5);  // 手动开辟堆区空间创建实例对象 v3 (在堆区创建对象)

	for (int i = 0; i < 5; i++)
		v3->setValue(i, 1000 + i);

	v3->show();

	delete v3;  // 堆区的内存不会自动释放, 必须手动用 delete 释放
	// delete 会自动调用析构函数 + 释放内存
	
	// 为什么 delete 后会调用析构函数 ?
	// 这是 C++ 的核心机制, 执行步骤:
	// 1. 调用析构函数: 释放对象内部的资源   
	// 2. 释放对象内存: 释放对象本身占用的堆内存

/*
	为什么 delete 没有用 delete[] ?
	因为 v3 指向的是一个对象
	VLA *v3 = new VLA(5);     // new 单个对象 → delete v3
	int *arr = new int[10];   // new[] 数组   → delete[] arr
*/

	return 0;
}
/*
	构造函数被调用了 !
	arr[0] = 100
	arr[1] = 101
	arr[2] = 102
	arr[3] = 103
	arr[4] = 104
	arr[5] = 105
	arr[6] = 106
	arr[7] = 107
	arr[8] = 108
	arr[9] = 109
拷贝构造函数被调用了 !
	arr[0] = 114511
	arr[1] = 101
	arr[2] = 102
	arr[3] = 103
	arr[4] = 104
	arr[5] = 105
	arr[6] = 106
	arr[7] = 107
	arr[8] = 108
	arr[9] = 109
	构造函数被调用了 !
	arr[0] = 1000
	arr[1] = 1001
	arr[2] = 1002
	arr[3] = 1003
	arr[4] = 1004
	析构函数被调用了 !
	析构函数被调用了 !
	析构函数被调用了 !
*/