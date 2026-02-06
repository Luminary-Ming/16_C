#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <stdlib.h>
#include "warplane.h"
#include <signal.h>
#include <math.h>
#include <string.h>
#include "music_manager.h"
#include "start_screen.h"
#include "home.h" 

// 游戏状态枚举扩展
enum {
    GAME_STATE_START = 5,  // 游戏开始界面
};

// 开始界面按钮位置
#define START_BUTTON_X 600        // 调整X位置
#define START_BUTTON_Y 400        // 调整Y位置
#define START_BUTTON_WIDTH 200    // 调整宽度
#define START_BUTTON_HEIGHT 60    // 调整高度

#define EXIT_BUTTON_X 600         // 调整X位置
#define EXIT_BUTTON_Y 500         // 调整Y位置
#define EXIT_BUTTON_WIDTH 200     // 调整宽度
#define EXIT_BUTTON_HEIGHT 60     // 调整高度

int main(void) {
	init_music_signals();

    // 音乐将在开始游戏时播放

    srand(getpid());

    fb = open("/dev/fb0", O_RDWR);
    if(fb < 0)
    {
        perror("open()");
        stop_bgm();
        return -1;
    }

    ret = ioctl(fb, FBIOGET_VSCREENINFO, &var);
    if(ret < 0)
    {
        perror("ioctl()");
        close(fb);
        stop_bgm();
        return -2;
    }

    bytes = var.bits_per_pixel >> 3;

    ret = ioctl(fb, FBIOGET_FSCREENINFO, &fix);
    if(ret < 0)
    {
        perror("ioctl()");
        close(fb);
        stop_bgm();
        return -2;
    }

    pc = mmap(0, fix.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);
    if(pc == (void *)(-1))
    {
        perror("mmap()");
        close(fb);
        stop_bgm();
        return -3;
    }

//-----------------------------------------------------------

    int kb_fd = 0;
    struct input_event ev;
    struct termios old, new;

    tcgetattr(0, &old);
    new = old;
    new.c_lflag &= ~ICANON;
    new.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &new);

    kb_fd = open("/dev/input/event1", O_RDONLY);
    if(kb_fd < 0)
    {
        perror("open()");
        stop_bgm();
        return -1;
    }
//----------------------------------------------------------
    struct warplane wp;
    int change = 1;  // 控制飞机类型

    // 初始化玩家
    wp.x = 400;
    wp.y = 400;
    wp.dir = 2;  // 初始方向向右
    wp.HP = PLAYER_MAX_HP;  // 玩家血量
    wp.maxHP = PLAYER_MAX_HP;
    wp.active = 1;  // 玩家激活

    struct bullet bullets[MAX_BULLETS];  // 子弹数组（玩家+BOSS）

    // BOSS初始化
    struct boss game_boss;
    reset_boss(&game_boss, 1, var.xres);  // 第一个BOSS

    // 爆炸效果数组
    struct explosion explosions[10];
    for (int i = 0; i < 10; i++) {
        explosions[i].active = 0;
        explosions[i].type = 0;
    }

    // 游戏控制
    struct game_control game;
    game.game_state = GAME_STATE_START;  // 初始状态为开始界面
    game.boss_count = 0;  // 已击败的BOSS数量
    game.total_bosses = TOTAL_BOSSES;
    game.game_time = 0;
    game.player_score = 0;
    game.show_game_over = 0;
    game.game_over_timer = 0;
    game.time_stop_effect = 0;
    game.time_stop_alpha = 0;

    // 调试信息
    printf("游戏初始状态: GAME_STATE_START\n");
    printf("屏幕分辨率: %dx%d, 每像素字节数: %d\n", var.xres, var.yres, bytes);
    printf("home.h图片宽度: 1920, 高度: 1080\n"); 

    // 声明外部图片数组
    extern const unsigned char gImage_warplane1[];
    extern const unsigned char gImage_warplane2[];
    extern const unsigned char gImage_warplane3[];
    extern const unsigned char gImage_warplane4[];
    extern const unsigned char gImage_warplane5[];
    extern const unsigned char gImage_bullet1[];
    extern const unsigned char gImage_bullet2[];
    extern const unsigned char gImage_bullet3[];
    extern const unsigned char gImage_bullet4[];
    extern const unsigned char gImage_shibai[];
    extern const unsigned char gImage_vector[];
    extern const unsigned char gImage_home[];
    // BOSS图片数组已经在warplane.h中声明

    // 初始化子弹数组
    for(int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].x = -1;  // 设置为-1表示子弹未激活
        bullets[i].y = -1;
        bullets[i].color = 0xFFFFFF;
        bullets[i].type = 0;  // 默认玩家子弹
        bullets[i].frozen = 0; // 初始未冻结
    }

    // 清屏为黑色
    draw_rectangle(0, 0, var.xres-1, var.yres-1, 0x000000);

    // 子弹发射计时器
    int shoot_timer = 0;
    int boss_shoot_timer = 0;
    const int SHOOT_INTERVAL = 20;  // 玩家子弹发射间隔
    const int BOSS_SHOOT_INTERVAL = 30;  // BOSS子弹发射间隔

    // 游戏帧数计数器
    int frame_count = 0;


    // 双缓冲变量
    // 在全局变量区域添加
    unsigned char *back_buffer = NULL;
    int back_buffer_size = 0;

    // 在初始化后创建后缓冲区
    back_buffer_size = fix.smem_len;
    back_buffer = malloc(back_buffer_size);
    if (!back_buffer)
    {
        perror("malloc back_buffer");
        stop_bgm();
        return -4;
    }
    memset(back_buffer, 0, back_buffer_size);  // 初始化为黑色


    int game_initialized = 0;  // 标记游戏是否已初始化

    // 游戏主循环
    while(1)
    {

        // 清空后缓冲区
        memset(back_buffer, 0, back_buffer_size);  // 清为黑色

        frame_count++;
        game.game_time++;

        // 如果是开始界面，处理开始界面逻辑
		if (game.game_state == GAME_STATE_START) {
		
            
            // 获取屏幕分辨率
            int screen_width = var.xres;
            int screen_height = var.yres;

            // home图片尺寸
            int pic_width = 1024;
            int pic_height = 600;

            // 居中显示图片
            int x_offset = (screen_width - pic_width) / 2;
            int y_offset = (screen_height - pic_height) / 2;

            // 计算按钮偏移（让按钮显示在图片上）
            int button_y_offset = (y_offset > 0) ? y_offset : 0;
            int button_x_offset = (x_offset > 0) ? x_offset : 0;

            if (x_offset >= 0 && y_offset >= 0)
            {
                // 如果屏幕比图片大，居中显示
                draw_pic_buffer(back_buffer, x_offset, y_offset, pic_width, pic_height, gImage_home);
            }
            else
            {
                // 如果屏幕比图片小，从左上角开始显示
                draw_pic_buffer(back_buffer, 0, 0,
                                (pic_width > screen_width) ? screen_width : pic_width,
                                (pic_height > screen_height) ? screen_height : pic_height,
                                gImage_home);
            }

           
            // ============ 绘制PLAY按钮 ============
            int play_x = button_x_offset + START_BUTTON_X;
            int play_y = button_y_offset + START_BUTTON_Y;

            // 绘制按钮边框（深色）
            draw_rectangle_buffer(back_buffer, play_x, play_y,
                                  play_x + START_BUTTON_WIDTH,
                                  play_y + START_BUTTON_HEIGHT,
                                  0x222222);  // 深灰色边框

            // 绘制按钮主体（渐变色效果）
            // 顶部浅绿色
            draw_rectangle_buffer(back_buffer, play_x + 3, play_y + 3,
                                  play_x + START_BUTTON_WIDTH - 3,
                                  play_y + START_BUTTON_HEIGHT / 2,
                                  0x00FF00);  // 亮绿色
            // 底部深绿色
            draw_rectangle_buffer(back_buffer, play_x + 3, play_y + START_BUTTON_HEIGHT / 2,
                                  play_x + START_BUTTON_WIDTH - 3,
                                  play_y + START_BUTTON_HEIGHT - 3,
                                  0x00AA00);  // 深绿色

            // 绘制高光效果
            draw_rectangle_buffer(back_buffer, play_x + 3, play_y + 3,
                                  play_x + START_BUTTON_WIDTH - 3,
                                  play_y + 5,
                                  0x88FF88);  // 顶部高光

            // 绘制"PLAY"文字（白色）
            int play_text_x = play_x + (START_BUTTON_WIDTH - 110) / 2;  // 精确居中对齐
            int play_text_y = play_y + (START_BUTTON_HEIGHT - 25) / 2;

            // 绘制P - 第1个字母
            // 左侧竖线
            draw_rectangle_buffer(back_buffer, play_text_x, play_text_y,
                                  play_text_x + 4, play_text_y + 25, 0xFFFFFF);
            // 顶部横线
            draw_rectangle_buffer(back_buffer, play_text_x, play_text_y,
                                  play_text_x + 15, play_text_y + 4, 0xFFFFFF);
            // 右侧竖线（上半部分）
            draw_rectangle_buffer(back_buffer, play_text_x + 15, play_text_y,
                                  play_text_x + 19, play_text_y + 13, 0xFFFFFF);
            // 中间横线
            draw_rectangle_buffer(back_buffer, play_text_x, play_text_y + 10,
                                  play_text_x + 15, play_text_y + 14, 0xFFFFFF);

            // 绘制L - 第2个字母（P和L之间留5像素间距）
            play_text_x += 24;
            // 左侧竖线
            draw_rectangle_buffer(back_buffer, play_text_x, play_text_y,
                                  play_text_x + 4, play_text_y + 25, 0xFFFFFF);
            // 底部横线
            draw_rectangle_buffer(back_buffer, play_text_x, play_text_y + 21,
                                  play_text_x + 15, play_text_y + 25, 0xFFFFFF);

            // 绘制A - 第3个字母（L和A之间留5像素间距）
           // 绘制A 点阵A（15x25像素）
            play_text_x += 24;

            // A字母的点阵表示
            int a_matrix[25][15] = {
                {0,0,0,0,1,1,1,1,1,1,1,0,0,0,0},  // 第0行
                {0,0,0,1,1,1,1,1,1,1,1,1,0,0,0},
                {0,0,1,1,1,0,0,0,0,0,1,1,1,0,0},
                {0,1,1,1,0,0,0,0,0,0,0,1,1,1,0},
                {0,1,1,0,0,0,0,0,0,0,0,0,1,1,0},
                {1,1,1,0,0,0,0,0,0,0,0,0,1,1,1},
                {1,1,0,0,0,0,0,0,0,0,0,0,0,1,1},
                {1,1,0,0,0,0,0,0,0,0,0,0,0,1,1},
                {1,1,0,0,0,0,0,0,0,0,0,0,0,1,1},
                {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},  // 中间横线位置（第10行）
                {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                {1,1,0,0,0,0,0,0,0,0,0,0,0,1,1},
                {1,1,0,0,0,0,0,0,0,0,0,0,0,1,1},
                {1,1,0,0,0,0,0,0,0,0,0,0,0,1,1},
                {1,1,0,0,0,0,0,0,0,0,0,0,0,1,1},
                {1,1,0,0,0,0,0,0,0,0,0,0,0,1,1},
                {1,1,0,0,0,0,0,0,0,0,0,0,0,1,1},
                {1,1,0,0,0,0,0,0,0,0,0,0,0,1,1},
                {1,1,0,0,0,0,0,0,0,0,0,0,0,1,1},
                {1,1,0,0,0,0,0,0,0,0,0,0,0,1,1},
                {1,1,0,0,0,0,0,0,0,0,0,0,0,1,1},
                {1,1,0,0,0,0,0,0,0,0,0,0,0,1,1},
                {1,1,0,0,0,0,0,0,0,0,0,0,0,1,1},
                {1,1,0,0,0,0,0,0,0,0,0,0,0,1,1},
                {1,1,0,0,0,0,0,0,0,0,0,0,0,1,1}   // 第24行
            };

            // 根据点阵绘制A
            for (int row = 0; row < 25; row++)
            {
                for (int col = 0; col < 15; col++)
                {
                    if (a_matrix[row][col] == 1)
                    {
                        draw_rectangle_buffer(back_buffer,
                                              play_text_x + col,
                                              play_text_y + row,
                                              play_text_x + col + 1,
                                              play_text_y + row + 1,
                                              0xFFFFFF);
                    }
                }
            }
           

            // 绘制Y - 第4个字母（A和Y之间留5像素间距）
            play_text_x += 24;
            // 左上斜线
            for (int i = 0; i < 4; i++)
            {
                draw_rectangle_buffer(back_buffer, play_text_x + i, play_text_y + i,
                                      play_text_x + i + 1, play_text_y + 13 + i, 0xFFFFFF);
            }
            // 右上斜线
            for (int i = 0; i < 4; i++)
            {
                draw_rectangle_buffer(back_buffer, play_text_x + 15 - i, play_text_y + i,
                                      play_text_x + 16 - i, play_text_y + 13 + i, 0xFFFFFF);
            }
            // 底部竖线
            draw_rectangle_buffer(back_buffer, play_text_x + 7, play_text_y + 13,
                                  play_text_x + 11, play_text_y + 25, 0xFFFFFF);

            // ============ 绘制EXIT按钮 ============
            int exit_x = button_x_offset + EXIT_BUTTON_X;
            int exit_y = button_y_offset + EXIT_BUTTON_Y;

            // 绘制按钮边框
            draw_rectangle_buffer(back_buffer, exit_x, exit_y,
                                  exit_x + EXIT_BUTTON_WIDTH,
                                  exit_y + EXIT_BUTTON_HEIGHT,
                                  0x222222);

            // 绘制按钮主体（渐变色效果）
            // 顶部浅红色
            draw_rectangle_buffer(back_buffer, exit_x + 3, exit_y + 3,
                                  exit_x + EXIT_BUTTON_WIDTH - 3,
                                  exit_y + EXIT_BUTTON_HEIGHT / 2,
                                  0xFF4444);  // 浅红色
            // 底部深红色
            draw_rectangle_buffer(back_buffer, exit_x + 3, exit_y + EXIT_BUTTON_HEIGHT / 2,
                                  exit_x + EXIT_BUTTON_WIDTH - 3,
                                  exit_y + EXIT_BUTTON_HEIGHT - 3,
                                  0xCC0000);  // 深红色

            // 绘制高光效果
            draw_rectangle_buffer(back_buffer, exit_x + 3, exit_y + 3,
                                  exit_x + EXIT_BUTTON_WIDTH - 3,
                                  exit_y + 5,
                                  0xFF8888);  // 顶部高光

            // 绘制"EXIT"文字（白色）
            int exit_text_x = exit_x + (EXIT_BUTTON_WIDTH - 110) / 2;  // 精确居中对齐
            int exit_text_y = exit_y + (EXIT_BUTTON_HEIGHT - 25) / 2;

            // 绘制E - 第1个字母
            // 左侧竖线
            draw_rectangle_buffer(back_buffer, exit_text_x, exit_text_y,
                                  exit_text_x + 4, exit_text_y + 25, 0xFFFFFF);
            // 顶部横线
            draw_rectangle_buffer(back_buffer, exit_text_x, exit_text_y,
                                  exit_text_x + 15, exit_text_y + 4, 0xFFFFFF);
            // 中间横线
            draw_rectangle_buffer(back_buffer, exit_text_x, exit_text_y + 10,
                                  exit_text_x + 15, exit_text_y + 14, 0xFFFFFF);
            // 底部横线
            draw_rectangle_buffer(back_buffer, exit_text_x, exit_text_y + 21,
                  
                                  exit_text_x + 15, exit_text_y + 25, 0xFFFFFF);

            // 绘制X - 第2个字母（E和X之间留5像素间距）
           // 绘制X - 方法3：精确点阵
            exit_text_x += 24;

            // X字母的15x25点阵
            int x_matrix[35][17] = {
                // 上半部分（15行）
          {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
            {0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0},
            {0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0},
            {0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0},
            {0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0},
            {0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},
            {0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},

            // 顶部到中间过渡
            {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},
            {0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},
            {0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0},

            // 中间部分（加长）
            {0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0},
            {0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0},
            {0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0},
            {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
            {0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0},
            };

            // 根据点阵绘制X
            for (int row = 0; row < 35; row++)
            {
                for (int col = 0; col < 17; col++)
                {
                    if (x_matrix[row][col] == 1)
                    {
                        draw_rectangle_buffer(back_buffer,
                                              exit_text_x + col,
                                              exit_text_y + row,
                                              exit_text_x + col + 1,
                                              exit_text_y + row + 1,
                                              0xFFFFFF);
                    }
                }
            }


            // 绘制I - 第3个字母（X和I之间留5像素间距）
            exit_text_x += 24;
            // 顶部横线
            draw_rectangle_buffer(back_buffer, exit_text_x, exit_text_y,
                                  exit_text_x + 18, exit_text_y + 4, 0xFFFFFF);
            // 中间竖线
            draw_rectangle_buffer(back_buffer, exit_text_x + 7, exit_text_y,
                                  exit_text_x + 11, exit_text_y + 25, 0xFFFFFF);
            // 底部横线
            draw_rectangle_buffer(back_buffer, exit_text_x, exit_text_y + 21,
                                  exit_text_x + 18, exit_text_y + 25, 0xFFFFFF);

            // 绘制T - 第4个字母（I和T之间留5像素间距）
            exit_text_x += 24;
            // 顶部横线
            draw_rectangle_buffer(back_buffer, exit_text_x, exit_text_y,
                                  exit_text_x + 18, exit_text_y + 4, 0xFFFFFF);
            // 中间竖线
            draw_rectangle_buffer(back_buffer, exit_text_x + 7, exit_text_y,
                                  exit_text_x + 11, exit_text_y + 25, 0xFFFFFF);

            // 可选：添加感叹号！在EXIT后面
            exit_text_x += 24;
            // 竖线部分
            draw_rectangle_buffer(back_buffer, exit_text_x + 7, exit_text_y,
                                  exit_text_x + 11, exit_text_y + 18, 0xFFFFFF);
            // 点部分
            draw_rectangle_buffer(back_buffer, exit_text_x + 7, exit_text_y + 21,
                                  exit_text_x + 11, exit_text_y + 25, 0xFFFFFF);

            // 将后缓冲区复制到屏幕
            memcpy(pc, back_buffer, back_buffer_size);


			// 处理开始界面的按键
			fd_set fds;
			struct timeval tv;
			int retval;

			FD_ZERO(&fds);
			FD_SET(kb_fd, &fds);
			tv.tv_sec = 0;
			tv.tv_usec = 0;

			retval = select(kb_fd + 1, &fds, NULL, NULL, &tv);

			if (retval == 1) {
				read(kb_fd, &ev, sizeof(ev));

				if(ev.type == EV_KEY && ev.value == 1) {
					switch(ev.code) {
					    case 28:  // 回车键开始游戏
					        game.game_state = GAME_STATE_PLAYING;
					        start_bgm("1.mp3");
					        // 清屏
                            memset(back_buffer, 0, back_buffer_size);  // 清空后缓冲区
                            memcpy(pc, back_buffer, back_buffer_size);  // 复制到屏幕
                            game_initialized = 1;  // 标记游戏已初始化

                            // 游戏初始化 - 清空子弹、重置位置等
                            for (int i = 0; i < MAX_BULLETS; i++)
                            {
                                bullets[i].x = -1;
                                bullets[i].y = -1;
                                bullets[i].frozen = 0;
                            }
                            shoot_timer = 0;
                            boss_shoot_timer = 0;

                            // 重置玩家位置
                            wp.x = var.xres / 2;
                            wp.y = var.yres - 100;
                            wp.HP = PLAYER_MAX_HP;
                            wp.active = 1;

					        break;
					    case 1:  // ESC键退出游戏
					        printf("游戏退出!\n");
					        tcsetattr(0, TCSANOW, &old);
                            free(back_buffer);  // 释放后缓冲区内存
					        munmap(pc, fix.smem_len);
					        close(fb);
					        close(kb_fd);
					        stop_bgm();
					        return 0;
					}
				}
			}

			usleep(8000);
			continue;
		}

        // 如果游戏结束，处理游戏结束逻辑
		if (game.game_state == GAME_STATE_GAME_OVER || game.game_state == GAME_STATE_WIN) {
			game.game_over_timer++;

			// 游戏结束时停止音乐
			if (game.game_over_timer == 1) {
				stop_bgm();
				// 清屏
				draw_rectangle(0, 0, var.xres - 1, var.yres - 1, 0x000000);
			}

			// 绘制游戏结束画面
			if (game.game_state == GAME_STATE_GAME_OVER) {
				// 计算居中位置
				int pic_x = (var.xres - 899) / 2;
				int pic_y = (var.yres - 406) / 2;
				draw_pic(pic_x, pic_y, 899, 406, gImage_shibai);

			} else if (game.game_state == GAME_STATE_WIN) {
				// 计算居中位置
				int pic_x = (var.xres - 899) / 2;
				int pic_y = (var.yres - 406) / 2;
				draw_pic(pic_x, pic_y, 899, 406, gImage_vector);

			}


            // 将后缓冲区复制到屏幕
            memcpy(pc, back_buffer, back_buffer_size);

            // 处理游戏结束时的按键
            fd_set fds;
            struct timeval tv;
            int retval;

            FD_ZERO(&fds);
            FD_SET(kb_fd, &fds);
            tv.tv_sec = 0;
            tv.tv_usec = 0;

            retval = select(kb_fd + 1, &fds, NULL, NULL, &tv);

            if (retval == 1) {
                read(kb_fd, &ev, sizeof(ev));

                if(ev.type == EV_KEY && ev.value == 1) {
                    if(ev.code == 1) {  // ESC键退出
                        printf("游戏结束!\n");
                        tcsetattr(0, TCSANOW, &old);
                        munmap(pc, fix.smem_len);
                        close(fb);
                        close(kb_fd);
                        stop_bgm();
                        return 0;
                    }
                }
            }

            // 显示一段时间后自动退出
            if (game.game_over_timer > GAME_OVER_DELAY * 2) {
                printf("游戏结束!\n");
                tcsetattr(0, TCSANOW, &old);
                munmap(pc, fix.smem_len);
                close(fb);
                close(kb_fd);
                stop_bgm();
                return 0;
            }

            usleep(8000);
            continue;
        }

        // 更新时间静止效果
        update_boss_time_stop(&game_boss, &game, bullets, MAX_BULLETS);

        // 根据时间静止状态设置游戏状态
        if (game.time_stop_effect)
        {
            game.game_state = GAME_STATE_TIME_STOP;
        }
        else if (game.game_state == GAME_STATE_TIME_STOP)
        {
            game.game_state = GAME_STATE_PLAYING;
        }

        // 清除旧飞机
        if (wp.active)
        {
            draw_rectangle_buffer(back_buffer, wp.x, wp.y,
                                  wp.x + WARFLANE, wp.y + WARFLANE, 0x000000);
        }

        // 清除旧BOSS - 使用BOSS的图片尺寸进行清除
        if (game_boss.alive)
        {
            draw_rectangle_buffer(back_buffer, game_boss.x, game_boss.y,
                                  game_boss.x + game_boss.pic_width,
                                  game_boss.y + game_boss.pic_height,
                                  0x000000);
        }

        // 清除旧子弹（只在非时间静止状态下清除，时间静止时子弹保持不动）
        if (!game.time_stop_effect)
        {
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (bullets[i].x >= 0 && bullets[i].y >= 0)
                {
                    if (bullets[i].type == 0)
                    {  // 玩家子弹
                        int bullet_w = 8, bullet_h = 16;  // 根据change调整
                        draw_rectangle_buffer(back_buffer, bullets[i].x, bullets[i].y,
                                              bullets[i].x + bullet_w,
                                              bullets[i].y + bullet_h,
                                              0x000000);
                    }
                    else
                    {  // BOSS子弹
                        draw_rectangle_buffer(back_buffer, bullets[i].x, bullets[i].y,
                                              bullets[i].x + 8,
                                              bullets[i].y + 8,
                                              0x000000);
                    }
                }
            }
        }

        // 清除旧爆炸效果
        for (int i = 0; i < 10; i++)
        {
            if (explosions[i].active)
            {
                int radius = explosions[i].frame * 5;
                draw_rectangle_buffer(back_buffer,
                                      explosions[i].x - radius,
                                      explosions[i].y - radius,
                                      explosions[i].x + radius,
                                      explosions[i].y + radius,
                                      0x000000);
            }
        }

        // 处理键盘输入（时间静止时仍然可以处理ESC退出）
        fd_set fds;
        struct timeval tv;
        int retval;

        FD_ZERO(&fds);
        FD_SET(kb_fd, &fds);
        tv.tv_sec = 0;
        tv.tv_usec = 0;

        retval = select(kb_fd + 1, &fds, NULL, NULL, &tv);

        if (retval == 1) {
            read(kb_fd, &ev, sizeof(ev));

            if(ev.type == EV_KEY && ev.value == 1) {
                switch(ev.code) {
                    case 103: if (!game.time_stop_effect) wp.dir = 1; break;  // 上（时间静止时不能移动）
                    case 108: if (!game.time_stop_effect) wp.dir = 2; break;  // 下
                    case 105: if (!game.time_stop_effect) wp.dir = 3; break;  // 左
                    case 106: if (!game.time_stop_effect) wp.dir = 4; break;  // 右
                    case 2: if (!game.time_stop_effect) change = 1; break;
                    case 3: if (!game.time_stop_effect) change = 2; break;
                    case 4: if (!game.time_stop_effect) change = 3; break;
                    case 5: if (!game.time_stop_effect) change = 4; break;
                    case 6: if (!game.time_stop_effect) change = 5; break;
                    case 1:  // ESC键（任何状态下都可以退出）
                        printf("游戏退出!\n");
                        tcsetattr(0, TCSANOW, &old);
                        munmap(pc, fix.smem_len);
                        close(fb);
                        close(kb_fd);
                        stop_bgm();
                        return 0;
                }
            }
        }

        // 如果玩家存活且不在时间静止状态，更新玩家位置
        if (wp.active && !game.time_stop_effect) {
            switch(wp.dir) {
                case 1: if(wp.y - WARFLANE > 0) wp.y -= 3; break;        // 上
                case 2: if(wp.y + WARFLANE < var.yres) wp.y += 3; break; // 下
                case 3: if(wp.x - WARFLANE > 0) wp.x -= 3; break;        // 左
                case 4: if(wp.x + WARFLANE < var.xres) wp.x += 3; break; // 右
            }
        }

        // 更新BOSS位置（使用多种轨迹），时间静止时BOSS仍然可以移动（作为特殊效果）
        if (game_boss.alive && wp.active) {
            update_boss_movement(&game_boss, var.xres, var.yres, &wp, frame_count);
        }

        // 玩家发射子弹（时间静止时不能发射新子弹）
        if (wp.active && !game.time_stop_effect) {
            shoot_timer++;
            if(shoot_timer >= SHOOT_INTERVAL) {
                shoot_timer = 0;

                for(int i = 0; i < MAX_BULLETS; i++) {
                    if(bullets[i].x < 0 || bullets[i].y < 0) {
                        bullets[i].x = wp.x + WARFLANE/2 - BULLET_SIZE/2;
                        bullets[i].y = wp.y - BULLET_H;
                        bullets[i].color = 0xFFFFFF;  // 白色子弹
                        bullets[i].type = 0;  // 玩家子弹
                        bullets[i].frozen = 0;
                        break;
                    }
                }
            }
        }

        // BOSS发射子弹（时间静止时不能发射新子弹）
        if (game_boss.alive && wp.active && !game.time_stop_effect) {
            boss_shoot_timer++;
            if(boss_shoot_timer >= BOSS_SHOOT_INTERVAL) {
                boss_shoot_timer = 0;

                // BOSS根据编号调整子弹数量
                int boss_bullet_count = 3;  // 基础子弹数量
                if (game_boss.boss_index == 2) {
                    boss_bullet_count = 5;  // 第二个BOSS子弹更多
                }

                for(int i = 0; i < boss_bullet_count; i++) {
                    for(int j = 0; j < MAX_BULLETS; j++) {
                        if(bullets[j].x < 0 || bullets[j].y < 0) {
                            bullets[j].x = game_boss.x + game_boss.width/2 - 4;
                            bullets[j].y = game_boss.y + game_boss.height;
                            bullets[j].color = BOSS_BULLET_COLOR;
                            bullets[j].type = 1;  // BOSS子弹
                            bullets[j].frozen = 0;

                            // 根据BOSS编号调整子弹散射
                            int offset = (i - boss_bullet_count/2) * 10;
                            if (game_boss.boss_index == 2) {
                                offset = (i - boss_bullet_count/2) * 15;  // 第二个BOSS散射更大
                            }
                            bullets[j].x += offset;
                            break;
                        }
                    }
                }
            }
        }

        // 更新所有子弹位置（时间静止时子弹不移动）
        if (!game.time_stop_effect) {
            for(int i = 0; i < MAX_BULLETS; i++) {
                if(bullets[i].x >= 0 && bullets[i].y >= 0 && !bullets[i].frozen) {
                    if (bullets[i].type == 0) {  // 玩家子弹向上移动
                        bullets[i].y -= 5;

                        // 如果子弹飞出屏幕，则标记为未激活
                        if(bullets[i].y + BULLET_H < 0) {
                            bullets[i].x = -1;
                            bullets[i].y = -1;
                        }
                    } else {  // BOSS子弹向下移动
                        // 根据BOSS编号调整子弹速度
                        int bullet_speed = 4;
                        if (game_boss.boss_index == 2) {
                            bullet_speed = 6;  // 第二个BOSS子弹更快
                        }

                        bullets[i].y += bullet_speed;

                        // 如果子弹飞出屏幕，则标记为未激活
                        if(bullets[i].y > var.yres) {
                            bullets[i].x = -1;
                            bullets[i].y = -1;
                        }
                    }
                }
            }
        }

        // 伤害判定模块（时间静止时暂停伤害判定）
        if (!game.time_stop_effect) {
            for(int i = 0; i < MAX_BULLETS; i++) {
                if(bullets[i].x >= 0 && bullets[i].y >= 0) {
                    // 玩家子弹击中BOSS
                    if (bullets[i].type == 0 && game_boss.alive &&
                        check_collision(bullets[i].x, bullets[i].y, BULLET_SIZE, BULLET_H,
                                      game_boss.x, game_boss.y, game_boss.pic_width, game_boss.pic_height)) {

                        game_boss.HP -= 10;  // BOSS受伤
                        bullets[i].x = -1;   // 子弹消失
                        bullets[i].y = -1;

                        // 添加爆炸效果
                        create_explosion(explosions, bullets[i].x, bullets[i].y, 0);

                        // 增加分数
                        game.player_score += 10;

                        if (game_boss.HP <= 0) {
                            game_boss.alive = 0;
                            game_boss.HP = 0;
                            game.boss_count++;
                            game.player_score += 100;  // 击败BOSS额外加分

                            // BOSS死亡时的大爆炸
                            for (int j = 0; j < 10; j++) {
                                create_explosion(explosions,
                                              game_boss.x + game_boss.pic_width/2,
                                              game_boss.y + game_boss.pic_height/2,
                                              2);
                            }

                            // 检查是否所有BOSS都被击败
                            if (game.boss_count >= game.total_bosses) {
                                // 所有BOSS都被击败，游戏胜利
                                game.game_state = GAME_STATE_WIN;
                                continue;
                            } else {
                                // 生成下一个BOSS
                                reset_boss(&game_boss, 2, var.xres);  // 生成第二个BOSS
                            }
                        }
                    }

                    // BOSS子弹击中玩家
                    if (bullets[i].type == 1 && wp.active && wp.HP > 0 &&
                        check_collision(bullets[i].x, bullets[i].y, 8, 8,
                                      wp.x, wp.y, WARFLANE, WARFLANE)) {

                        wp.HP -= 20;  // 玩家受伤
                        bullets[i].x = -1;  // 子弹消失
                        bullets[i].y = -1;

                        // 添加爆炸效果
                        create_explosion(explosions, bullets[i].x, bullets[i].y, 0);

                        if (wp.HP <= 0) {
                            wp.HP = 0;
                            wp.active = 0;  // 玩家死亡

                            // 玩家死亡爆炸效果
                            for (int j = 0; j < 10; j++) {
                                create_explosion(explosions,
                                              wp.x + WARFLANE/2,
                                              wp.y + WARFLANE/2,
                                              1);
                            }

                            // 玩家死亡，游戏结束
                            game.game_state = GAME_STATE_GAME_OVER;
                            continue;
                        }
                    }
                }
            }
        }

        // 更新爆炸效果（时间静止时爆炸效果也暂停）
        if (!game.time_stop_effect) {
            for (int i = 0; i < 10; i++) {
                if (explosions[i].active) {
                    explosions[i].frame++;
                    if (explosions[i].frame > 10) {  // 爆炸持续10帧
                        explosions[i].active = 0;
                    }
                }
            }
        }

        // 绘制新飞机
        if (wp.active)
        {
            const unsigned char *plane_img = NULL;
            switch (change)
            {
                case 1: plane_img = gImage_warplane1; break;
                case 2: plane_img = gImage_warplane2; break;
                case 3: plane_img = gImage_warplane3; break;
                case 4: plane_img = gImage_warplane4; break;
                case 5: plane_img = gImage_warplane5; break;
            }
            if (plane_img)
            {
                draw_pic_buffer(back_buffer, wp.x, wp.y, WARFLANE, WARFLANE, plane_img);
            }
        }

        // 绘制BOSS - 使用draw_pic函数绘制BOSS图片
        if (game_boss.alive && game_boss.boss_pic != NULL)
        {
            draw_pic_buffer(back_buffer, game_boss.x, game_boss.y,
                            game_boss.pic_width, game_boss.pic_height,
                            game_boss.boss_pic);

            // 如果BOSS在闪烁（时间静止期间）
            if (game_boss.flashing && game_boss.flash_state) {
                // 闪烁状态：绘制一个高亮的白色边框
                draw_rectangle(game_boss.x - 3, game_boss.y - 3,
                              game_boss.x + game_boss.pic_width + 2,
                              game_boss.y + game_boss.pic_height + 2,
                              0x000000);
            }

            // 如果BOSS血量较低且不在时间静止状态，添加普通闪烁效果
            if (game_boss.HP < game_boss.maxHP / 4 && !game.time_stop_effect) {
                // 低血量时闪烁红色边框
                if (frame_count % 8 < 4) {  // 闪烁效果
                    draw_rectangle(game_boss.x - 2, game_boss.y - 2,
                                  game_boss.x + game_boss.pic_width + 1,
                                  game_boss.y + game_boss.pic_height + 1,
                                  0x000000);
                }
            }
        }

        // 绘制所有激活的子弹
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (bullets[i].x >= 0 && bullets[i].y >= 0)
            {
                if (bullets[i].type == 0)
                {  // 玩家子弹
                    const unsigned char *bullet_img = NULL;
                    switch (change)
                    {
                        case 1: bullet_img = gImage_bullet1; break;
                        case 2: bullet_img = gImage_bullet2; break;
                        case 3: bullet_img = gImage_bullet3; break;
                        case 4: bullet_img = gImage_bullet4; break;
                        case 5: bullet_img = gImage_bullet2; break;
                    }
                    if (bullet_img)
                    {
                        int bullet_w = 4, bullet_h = 16;
                        if (change == 2 || change == 4 || change == 5) { bullet_w = 8; bullet_h = 14; }
                        if (change == 3) { bullet_w = 8; bullet_h = 8; }
                        draw_pic_buffer(back_buffer, bullets[i].x, bullets[i].y,
                                        bullet_w, bullet_h, bullet_img);
                    }
                }
                else
                {  // BOSS子弹
                    draw_rectangle_buffer(back_buffer, bullets[i].x, bullets[i].y,
                                          bullets[i].x + 8, bullets[i].y + 8,
                                          BOSS_BULLET_COLOR);
                }
            }
        }

        // 绘制爆炸效果
        for (int i = 0; i < 10; i++)
        {
            if (explosions[i].active)
            {
                int radius = explosions[i].frame * 2;
                int color = 0xFF0000;  // 红色爆炸

                // 绘制圆形爆炸效果
                for (int dy = -radius; dy <= radius; dy++)
                {
                    for (int dx = -radius; dx <= radius; dx++)
                    {
                        if (dx * dx + dy * dy <= radius * radius)
                        {
                            // 需要实现draw_point_buffer函数
                            draw_point_buffer(back_buffer,
                                              explosions[i].x + dx,
                                              explosions[i].y + dy,
                                              color);
                        }
                    }
                }
            }
        }

        // 绘制游戏信息（血条、分数等）
        draw_game_info_buffer(back_buffer, &game, &wp, &game_boss, var.xres);

        // 绘制时间静止特效（半透明蓝色覆盖层）
        if (game.time_stop_effect && game.time_stop_alpha > 0)
        {
            // 使用back_buffer绘制特效
            for (int i = 0; i < var.xres; i += 2)
            {
                for (int j = 0; j < var.yres; j += 2)
                {
                    if ((i + j) % 4 < 2)
                    {
                        draw_point_buffer(back_buffer, i, j, 0x0000FF);
                    }
                }
            }

            // 在屏幕中央显示"TIME STOP"文字
            int center_x = var.xres / 2;
            int center_y = var.yres / 2;
            draw_rectangle(center_x - 60, center_y - 20, center_x + 60, center_y - 10, 0x000000);
            draw_rectangle(center_x - 60, center_y, center_x + 60, center_y + 10, 0x000000);
        }

        // 将后缓冲区复制到屏幕
        memcpy(pc, back_buffer, back_buffer_size);

        // 延迟
        usleep(8000);
    }

    tcsetattr(0, TCSANOW, &old);
    munmap(pc, fix.smem_len);
    close(fb);
    close(kb_fd);
    stop_bgm();

    return 0;
}
