#ifndef FSM_H
#define FSM_H

#define BUF_SIZE 1024  // 缓冲区大小

// 定义状态
enum
{ 
	STATE_R,  // 读状态: 从读文件描述符读取数据 
	STATE_W,  // 写状态: 向写文件描述符写入数据
	STATE_E,  // 错误状态: 打印错误信息
	STATE_T   // 终止状态: 结束状态机
};

// 定义有限状态机的结构
typedef struct
{
	int rfd;  // 读文件的文件描述符
	int wfd;  // 写文件的文件描述符
	char buf[BUF_SIZE];  // 存储读取到的数据
	int count;  // 成功读取的字节数
	int pos;  // 已写入的字节数 (用于续写)
	int state;  // 当前状态 (R / W / E / T)
	char *errmsg;  // 出错时记录系统调用函数名 (用于 perror)
}fsm_t;

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
extern int fsm_init(fsm_t **f, int rfd, int wfd);


/*
* 推动有限状态机
* @param f 指向要推动的有限状态机
* @return 成功返回 0; 失败返回 < 0
*
* 状态转移逻辑:
* [R] 把 rfd 文件里的数据读到 buf 中, 判断 read(2) 返回值
*	>0 读取成功有数据, 推到 [W]
*	=0 EOF, 读完了该结束了, 推到 [T]
*	<0 读到错误, 检查是不是假错(EAGAIN), 如果是假错就重读, 推到 [E]
* [W] 把 buf 里存储的数据写到 wfd 文件 (检查续写)
* [E] perror(3) 打印错误信息, 推到 [T]
* [T] 终止进程
*/
extern int fsm_drive(fsm_t *f);


/*
* 销毁有限状态机, 释放内存
* @param f 指向要销毁的有限状态机
* @return 成功返回 0; 失败返回 <0
*/
extern int fsm_destroy(fsm_t *f);

#endif
