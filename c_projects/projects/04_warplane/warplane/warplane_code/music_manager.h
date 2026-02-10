#ifndef __MUSIC_MANAGER_H
#define __MUSIC_MANAGER_H

#include <unistd.h>

// 启动背景音乐（异步播放，不阻塞游戏）
void start_bgm(const char *file);

// 停止背景音乐并精准清理进程
void stop_bgm();

// 初始化信号处理（核心：确保按 Ctrl+C 强行退出时音乐也能停）
void init_music_signals();

#endif
