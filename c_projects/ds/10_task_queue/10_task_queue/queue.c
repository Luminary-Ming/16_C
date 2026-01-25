#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

/*
功能 : 队列初始化
参数 : q 回填开辟出的地址
        capacity 客户指定的容量
        size 客户指定一个空间的大小
返回值 : 成功返回0,失败返回 <0
*/
int queue_init(queue_t **q, int capacity, int size) 
{
    *q = malloc(sizeof(queue_t));  // 开辟队列结构的空间
    if (*q == NULL)
        return -1;
    // capacity + 1, 客户指定的容量是 capacity, 但是为了循环队列要牺牲一个空间, 所有开辟的大小就是 capacity + 1
    (*q)->s = calloc(capacity + 1, size);  // 开辟队列的空间(数据域)
    if ((*q)->s == NULL)
    {
        free(*q);  // 释放队列结构的空间
        *q = NULL; // 避免使用野指针
        return -2;
    } 
    // 初始化
    (*q)->front = (*q)->rear = 0;  // 对头队尾默认标记在 0 的位置
    (*q)->capacity = capacity + 1;  // 队列结构中存储客户指定的容量 + 1
    (*q)->size = size;  // 队列结构中存储客户指定一个空间的大小

    return 0;
}
/* 
    q 是一级指针的地址, *q 是一级指针, 
    释放 *q 后, 因为不是局部变量, 函数结束后, 
    在调用 queue_init 地方可能会误用已释放的指针
*/

/*
功能 : 判断队列是否为空
参数 : q 指向要判断的队列
返回值 : 为空返回1,不为空返回0
*/
int queue_is_empty(const queue_t *q)
{
    return q->rear == q->front;
}

/*
功能 : 判断队列是否为满
参数 : q 指向要判断的队列
返回值 : 为满返回1,不为满返回0
*/
int queue_is_full(const queue_t *q)
{
    return ((q->rear + 1) % q->capacity) == q->front;
}

/*
功能 : 入队
参数 : q 指向要入队的队列
        data 指向要入队的数据
返回值 : 成功返回0,失败返回<0
*/
int queue_en(queue_t *q, const void *data)
{
    if (queue_is_full(q))
        return -1;

    q->rear = (q->rear + 1) % q->capacity;  // 移动队尾标记
    memcpy((char *)q->s + q->rear * q->size, data, q->size);  // 入队数据
    // q->s 强转为 char * 就能1字节偏移
    // q->rear * q->size 队尾标记 * 一个空间的大小 = 要访问的元素在内存中的字节偏移量
    // (char *)q->s + q->rear * q->size  将基地址加上偏移量，得到目标元素的内存地址

    return 0;
}

/*
功能 : 出队
参数 : q 指向要出队的队列
       save 指向存储空间(存储出队的数据)
返回值 : 成功返回0,失败返回<0
*/
int queue_de(queue_t *q, void *save)
{
    if (queue_is_empty(q))
        return -1;
    q->front = (q->front + 1) % q->capacity;
    memcpy(save, (char *)q->s + q->front * q->size, q->size);
    memset((char *)q->s + q->front * q->size, '\0', q->size);  // 清空脏数据(清空出队数据)
    return 0;
}

/*
功能 : 释放队列
参数 : q 指向要释放的队列
返回值 : 空
*/
void queue_destroy(queue_t *q)
{
    free(q->s);  // 先释放队列空间
    free(q);  // 再释放队列结构体空间
}