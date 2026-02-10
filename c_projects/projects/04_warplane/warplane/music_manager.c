#include "music_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
//#include"boss_images.h"
static pid_t music_pid = -1; // 全局变量，记录播放器进程ID

// 内部信号处理函数
static void handle_exit_signal(int sig) {
    stop_bgm(); // 捕获到退出信号时先杀掉音乐
    // 不直接exit(0)，而是让信号继续传递，这样main函数中的清理代码也能执行
    signal(sig, SIG_DFL);
    raise(sig);
}

void init_music_signals() {
    // 捕获 Ctrl+C (SIGINT) 和 kill 命令 (SIGTERM)
    signal(SIGINT, handle_exit_signal);
    signal(SIGTERM, handle_exit_signal);
}

void start_bgm(const char *file) {
    music_pid = fork(); // 分裂出一个子进程来跑音乐
    
    if (music_pid < 0) {
        perror("Fork music error");
    } 
    else if (music_pid == 0) {
        // --- 子进程环境 ---
        // 使用 execlp 替换当前镜像为 madplay 程序
        // -r 循环播放, -q 静音模式（不让播放器信息干扰控制台）
        execlp("madplay", "madplay", file, "-r", "-q", NULL);
        
        // 如果执行到这里说明 madplay 没装或路径不对
        perror("Exec madplay failed");
        exit(1);
    }
    // 父进程继续运行
}

void stop_bgm() {
    if (music_pid > 0) {
        printf("正在关闭背景音乐 (PID: %d)...\n", music_pid);
        kill(music_pid, SIGKILL);      // 强制杀死音乐进程
        waitpid(music_pid, NULL, 0);   // 回收子进程，防止产生僵尸进程
        music_pid = -1;
    }
}
