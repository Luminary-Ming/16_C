#include <iostream>

using namespace std;  // 打开命名空间

// 函数重载
// 编译器会根据实参的类型、个数、顺序自动决定调用哪个函数
void print(int id, char *name, int math);
void print(int id, char *name, float math);

int main(void)
{
    int id = 0;
    char name[32] = { 0 };
    int math_i = 0;
    float math_f = 0.0f;

    cout << "ID : ";
    cin >> id;
    cout << "NAME : ";
    cin >> name;
    cout << "MATH : ";
    cin >> math_f;

    print(id, name, math_f);

    cout << "MATH : ";
    cin >> math_i;

    print(id, name, math_i);

    return 0;
}

void print(int id, char *name, int math)
{
    cout << "ID = " << id << endl;
    cout << "NAME = " << name << endl;
    cout << "MATH = " << math << endl;
}

void print(int id, char *name, float math)
{
    cout << "ID = " << id << endl;
    cout << "NAME = " << name << endl;
    cout << "MATH = " << math << endl;
}