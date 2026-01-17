#include <stdio.h>
#include <string.h>

struct aaa
{
    int id;
    char name[20];
    int age;
};

struct bbb
{
    int id;
    char name[20];
    int age;
};

int main(void)
{
    struct aaa a;
    struct bbb b;

    a.id = 10010;
    strcpy(a.name, "liantong");
    a.age = 19;

    b.id = 10086;
    strcpy(b.name, "yidong");
    b.age = 59;

    printf("a => id = %d name = %s age = %d\n", a.id, a.name, a.age);
    printf("b => id = %d name = %s age = %d\n", b.id, b.name, b.age);

    memcpy(&a, &b, sizeof(a));

    printf("a => id = %d name = %s age = %d\n", a.id, a.name, a.age);
    printf("b => id = %d name = %s age = %d\n", b.id, b.name, b.age);

    //int temp = a.id;
    //a.id = b.id;
    //b.id = temp;

    memcpy(&a.id, &b.id, sizeof(a.id));


    printf("a => id = %d name = %s age = %d\n", a.id, a.name, a.age);
    printf("b => id = %d name = %s age = %d\n", b.id, b.name, b.age);

    return 0;
}