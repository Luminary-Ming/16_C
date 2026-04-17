#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "01_fsm.h"

/*
* 初始化有限状态机
* @param f 要初始化的有限状态机
* @param rfd 读文件描述符 (客户指定)
* @param wfd 写文件描述符 (客户指定)
* @return 成功返回 0; 失败返回 <0
*
* 功能:
* [1] 给有限状态机开辟空间
* [2] 初始化有限状态机中的每个成员
* [3] 将 rfd 和 wfd 设置为非阻塞模式 (使用 fcntl 添加 O_NONBLOCK)
*/
int fsm_init(fsm_t **f, int rfd, int wfd)
{
	 // 开辟状态机的空间
	 fsm_t *me = malloc(sizeof(fsm_t)); 
	 if (me == NULL)
		 return -1;

	 // 初始化各成员变量
	 me->rfd = rfd;  // 读文件描述符
	 me->wfd = wfd;  // 写文件描述符
	 memset(me->buf, 0, BUF_SIZE);  // 清空缓冲区
	 me->count = 0;  // 初始无数据
	 me->pos = 0;    // 初始写位置为0
	 me->state = STATE_R;  // 起始状态为读
	 me->errmsg = NULL;    // 错误信息指针置空

	 // 将读文件描述符设置为非阻塞模式
	 int saver = fcntl(me->rfd, F_GETFL);  // 获取读文件的文件状态
	 fcntl(me->rfd, F_SETFL, saver | O_NONBLOCK);  // 在读文件原有的状态上加入非阻塞
	 
	 int savew = fcntl(me->wfd, F_GETFL);  // 获取写文件的文件状态
	 fcntl(me->wfd, F_SETFL, savew | O_NONBLOCK);  // 在写文件原有的状态上加入非阻塞

	 // 将分配好的状态机地址返回给调用者
	 *f = me;  

	 return 0;
}


/*
* 推动有限状态机
* @param f 指向要推动的有限状态机
* @return 成功返回 0; 失败返回 < 0
*
* 状态转移逻辑:
* [R] 把 rfd 文件里的数据读到 buf 中, 判断 read(2) 返回值
*	>0 读成功了去写, 推到 [W]
*	=0 EOF, 读完了该结束了, 推到 [T]
*	<0 读到错误, 检查是不是假错(EAGAIN), 如果是假错就重读, 推到 [E]
* [W] 把 buf 里存储的数据写到 wfd 文件 (检查续写)
* [E] perror(3) 打印错误信息, 推到 [T]
* [T] 终止进程
*/
int fsm_drive(fsm_t *f)
{
	if (f == NULL) return -1;

	switch (f->state)
	{
		case STATE_R:  // 读状态
			f->count = read(f->rfd, f->buf, BUF_SIZE);  // 从 rfd 文件中读取数据
			if (f->count == -1)  // 读取出错
			{
				if (errno != EAGAIN)  // 检查是不是假错 (EAGAIN 表示无数据可读, 是假错)
				{
					f->errmsg = "read()";  // 记录出错函数的函数名
					f->state = STATE_E;  // 转入错误状态
				}
				// 如果是 EAGAIN (假错), 状态保持不变, 下次继续读
			}
			else if(f->count == 0)  // 读到末尾了 (EOF)
				f->state = STATE_T;  // 转入终止状态
			else  // 读取成功, 有数据
			{
				f->pos = 0;  // 重置写位置 (从头开始写)
				f->state = STATE_W;  // 转入写状态
			}
			break;
		
		case STATE_W:  // 写状态
			// 尝试将缓冲区中的数据写入写文件描述符 (支持续写)
			int w_count = write(f->wfd, f->buf + f->pos, f->count - f->pos);
			if (w_count == -1)  // 写入出错
			{
				f->errmsg = "write()";  // 记录出错函数名
				f->state = STATE_E;  // 转入错误状态
			}
			else 
			{
				if (w_count < f->count - f->pos)  // 部分写入, 没有写完
					f->pos += w_count;  // 更新已写位置
				else  // 全部写完
					f->state = STATE_R;  // 返回读状态, 准备读取下一批数据
			}
			break;

		case STATE_E:  // 错误状态
			perror(f->errmsg);  // 打印错误信息
			f->state = STATE_T;  // 打印后转入终止状态
			break;

		case STATE_T:  // 终止状态
			// 什么都不做, 等待主循环结束并销毁状态机
			break;
		
		default: 
			// 未知状态, 不做处理
			break;
	}

	return 0;
}


/*
* 销毁有限状态机, 释放内存
* @param f 指向要销毁的有限状态机
* @return 成功返回 0; 失败返回 <0
*/
int fsm_destroy(fsm_t *f)
{
	free(f);  // 释放有限状态机的结构
	f = NULL;  // 避免出现野指针

	return 0;
}
	