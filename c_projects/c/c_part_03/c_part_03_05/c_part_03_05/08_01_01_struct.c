#include <stdio.h>
#include <string.h>

struct Student
{
	int id;
	char name[20];
	int age;
};

int main(void)
{
	struct Student s;

	s.id = 10;
	printf("id = %d\n", s.id);

	// s.name = "无双剑姬";  // error, 不能给数组名做赋值运算
	//strcpy(s.name, "无极剑圣");
	sprintf(s.name, "疾风剑豪");
	printf("name = %s\n", s.name);

	s.age = 21;
	printf("age = %d\n", s.age);

	scanf("%d", &s.id);
	scanf("%s", s.name);
	scanf("%d", &s.age);

	printf("id = %d\n", s.id);
	printf("name = %s\n", s.name);
	printf("age = %d\n", s.age);

	return 0;
}