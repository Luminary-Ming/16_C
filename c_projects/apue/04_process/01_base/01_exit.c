#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
	printf("Hello World!");

	/*
		return 0;
		内核 -> C启动例程 -> main() -> printf() -> main()
		-> return -> C启动例程 -> exit(3) -> 标准 IO 清理程序
		-> exit(3) -> _exit(2) 或 _Exit(2) -> 内核
	*/

	/*
		exit(0);
		内核 -> C启动例程 -> main() -> printf() -> main()
		-> exit(3) -> 标准 IO 清理程序
		-> exit(3) -> _exit(2) 或 _Exit(2) -> 内核
	*/

	/*
		_exit(2);
		内核 -> C启动例程 -> main() -> printf() -> main()
		-> _exit(2) 或 _Exit(2) -> 内核
	*/

	/*
		如果 main() 最后什么也没有写,等同于 return
	*/
}


