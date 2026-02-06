
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
#include "music_manager.h"
#include "warplane.h"
#include "start_screen.h"
// 游戏状态枚举扩展
enum {
    GAME_STATE_START = 5,  // 游戏开始界面
};

// 开始界面按钮位置
#define START_BUTTON_X 760
#define START_BUTTON_Y 600
#define START_BUTTON_WIDTH 400
#define START_BUTTON_HEIGHT 80

#define EXIT_BUTTON_X 760
#define EXIT_BUTTON_Y 720
#define EXIT_BUTTON_WIDTH 400
#define EXIT_BUTTON_HEIGHT 80

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

    // 游戏主循环
    while(1)
    {
        frame_count++;
        game.game_time++;

        // 如果是开始界面，处理开始界面逻辑
		if (game.game_state == GAME_STATE_START) {
			// 绘制开始界面背景（使用蓝色背景）
			draw_rectangle(0, 0, var.xres - 1, var.yres - 1, 0x000080);

			// 绘制开始按钮
			draw_rectangle(START_BUTTON_X, START_BUTTON_Y,
			              START_BUTTON_X + START_BUTTON_WIDTH,
			              START_BUTTON_Y + START_BUTTON_HEIGHT,
			              0xFF0000);

			// 绘制退出按钮
			draw_rectangle(EXIT_BUTTON_X, EXIT_BUTTON_Y,
			              EXIT_BUTTON_X + EXIT_BUTTON_WIDTH,
			              EXIT_BUTTON_Y + EXIT_BUTTON_HEIGHT,
			              0x0000FF);

			// 绘制标题文字
			draw_rectangle(600, 300, 1320, 350, 0xFFFF00);
			draw_rectangle(650, 370, 1270, 400, 0xFFFF00);

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
					        draw_rectangle(0, 0, var.xres - 1, var.yres - 1, 0x000000);
					        break;
					    case 1:  // ESC键退出游戏
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
        if (game.time_stop_effect) {
            game.game_state = GAME_STATE_TIME_STOP;
        } else if (game.game_state == GAME_STATE_TIME_STOP) {
            game.game_state = GAME_STATE_PLAYING;
        }

        // 清除旧飞机
        if (wp.active) {
            draw_rectangle(wp.x, wp.y, wp.x + WARFLANE, wp.y + WARFLANE, 0x000000);
        }

        // 清除旧BOSS - 使用BOSS的图片尺寸进行清除
        if (game_boss.alive) {
            draw_rectangle(game_boss.x, game_boss.y,
                          game_boss.x + game_boss.pic_width,
                          game_boss.y + game_boss.pic_height,
                          0x000000);
        }

        // 清除旧子弹（只在非时间静止状态下清除，时间静止时子弹保持不动）
        if (!game.time_stop_effect) {
            for(int i = 0; i < MAX_BULLETS; i++) {
                if(bullets[i].x >= 0 && bullets[i].y >= 0) {
                    if (bullets[i].type == 0) {  // 玩家子弹
                        switch(change) {
                            case 1:
                                draw_rectangle(bullets[i].x, bullets[i].y,
                                             bullets[i].x + BULLET_SIZE - 1,
                                             bullets[i].y + BULLET_H - 1,
                                             0x000000);
                                break;
                            case 2:
                                draw_rectangle(bullets[i].x, bullets[i].y,
                                             bullets[i].x + BULLET_SIZE + 3,
                                             bullets[i].y + BULLET_H - 3,
                                             0x000000);
                                break;
                            case 3:
                                draw_rectangle(bullets[i].x, bullets[i].y,
                                             bullets[i].x + BULLET_SIZE + 3,
                                             bullets[i].y + BULLET_H - 9,
                                             0x000000);
                                break;
                            case 4:
                            case 5:
                                draw_rectangle(bullets[i].x, bullets[i].y,
                                             bullets[i].x + BULLET_SIZE + 3,
                                             bullets[i].y + BULLET_H - 3,
                                             0x000000);
                                break;
                        }
                    } else {  // BOSS子弹
                        // BOSS子弹用红色方块表示
                        draw_rectangle(bullets[i].x, bullets[i].y,
                                      bullets[i].x + 8,  // BOSS子弹大一些
                                      bullets[i].y + 8,
                                      0x000000);
                    }
                }
            }
        }

        // 清除旧爆炸效果
        for (int i = 0; i < 10; i++) {
            if (explosions[i].active) {
                int radius = explosions[i].frame * 5;
                draw_rectangle(explosions[i].x - radius, explosions[i].y - radius,
                              explosions[i].x + radius, explosions[i].y + radius,
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
        if (wp.active) {
            switch(change) {
                case 1: draw_pic(wp.x, wp.y, WARFLANE, WARFLANE, gImage_warplane1); break;
                case 2: draw_pic(wp.x, wp.y, WARFLANE, WARFLANE, gImage_warplane2); break;
                case 3: draw_pic(wp.x, wp.y, WARFLANE, WARFLANE, gImage_warplane3); break;
                case 4: draw_pic(wp.x, wp.y, WARFLANE, WARFLANE, gImage_warplane4); break;
                case 5: draw_pic(wp.x, wp.y, WARFLANE, WARFLANE, gImage_warplane5); break;
            }
        }

        // 绘制BOSS - 使用draw_pic函数绘制BOSS图片
        if (game_boss.alive && game_boss.boss_pic != NULL) {
            // 正常绘制BOSS图片
            draw_pic(game_boss.x, game_boss.y,
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
        for(int i = 0; i < MAX_BULLETS; i++) {
            if(bullets[i].x >= 0 && bullets[i].y >= 0) {
                if (bullets[i].type == 0) {  // 玩家子弹
                    switch(change) {
                        case 1: draw_pic(bullets[i].x, bullets[i].y, 4, 16, gImage_bullet1); break;
                        case 2: draw_pic(bullets[i].x, bullets[i].y, 8, 14, gImage_bullet2); break;
                        case 3: draw_pic(bullets[i].x, bullets[i].y, 8, 8, gImage_bullet3); break;
                        case 4: draw_pic(bullets[i].x, bullets[i].y, 8, 14, gImage_bullet4); break;
                        case 5: draw_pic(bullets[i].x, bullets[i].y, 8, 14, gImage_bullet2); break;
                    }
                } else {  // BOSS子弹
                    // 红色矩形表示BOSS子弹
                    draw_rectangle(bullets[i].x, bullets[i].y,
                                  bullets[i].x + 8,
                                  bullets[i].y + 8,
                                  BOSS_BULLET_COLOR);
                }

                // 如果子弹被冻结（时间静止），添加特效
                if (bullets[i].frozen) {
                    // 绘制一个蓝色的冻结光环
                    draw_rectangle(bullets[i].x - 2, bullets[i].y - 2,
                                  bullets[i].x + 10,
                                  bullets[i].y + 10,
                                  0x000000);
                }
            }
        }

        // 绘制爆炸效果
        for (int i = 0; i < 10; i++) {
            if (explosions[i].active) {
                int radius = explosions[i].frame * 2;
                int color;

                // 根据爆炸类型选择颜色
                switch (explosions[i].type) {
                    case 1: color = 0x000000; break;  // 玩家死亡爆炸 - 红色
                    case 2: color = 0x000000; break;  // BOSS死亡爆炸 - 橙色
                    default: color = 0x000000 + explosions[i].frame * 0x000000; break;  // 普通爆炸
                }

                // 绘制圆形爆炸效果
                for (int dy = -radius; dy <= radius; dy++) {
                    for (int dx = -radius; dx <= radius; dx++) {
                        if (dx*dx + dy*dy <= radius*radius) {
                            draw_point(explosions[i].x + dx, explosions[i].y + dy, color);
                        }
                    }
                }
            }
        }

        // 绘制游戏信息（血条、分数等）
        draw_game_info(&game, &wp, &game_boss, var.xres);

        // 绘制时间静止特效（半透明蓝色覆盖层）
        if (game.time_stop_effect && game.time_stop_alpha > 0) {
            // 绘制一个半透明的蓝色覆盖层，表示时间静止
            for (int i = 0; i < var.xres; i += 2) {
                for (int j = 0; j < var.yres; j += 2) {
                    if ((i + j) % 4 < 2) {  // 棋盘格效果
                        draw_point(i, j, 0x000000);
                    }
                }
            }

            // 在屏幕中央显示"TIME STOP"文字
            int center_x = var.xres / 2;
            int center_y = var.yres / 2;
            draw_rectangle(center_x - 60, center_y - 20, center_x + 60, center_y - 10, 0x000000);
            draw_rectangle(center_x - 60, center_y, center_x + 60, center_y + 10, 0x000000);
        }

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
