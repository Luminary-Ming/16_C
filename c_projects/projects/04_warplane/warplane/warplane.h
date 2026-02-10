#ifndef __WARPLANE_H
#define __WARPLANE_H

// 游戏状态枚举
enum {
    GAME_STATE_PLAYING = 0,   // 游戏进行中
    GAME_STATE_GAME_OVER = 1, // 游戏结束（玩家失败）
    GAME_STATE_WIN = 2,       // 游戏胜利
    GAME_STATE_PAUSE = 3,     // 游戏暂停
    GAME_STATE_TIME_STOP = 4  // 时间静止状态
};

// 全局变量声明
extern int fb;
extern int ret;
extern struct fb_var_screeninfo var;
extern struct fb_fix_screeninfo fix;
extern unsigned char *pc;
extern unsigned int *pi;
extern unsigned int bytes;
extern unsigned long offset;

struct warplane
{
	int x;
	int y;
	int dir;
	int HP;
    int maxHP;
    int active;  // 是否激活
};

struct bullet
{
	int x;
	int y;
	int color;
    int type;  // 0:玩家子弹, 1:BOSS子弹
    int frozen; // 是否被冻结（时间静止时）
};

// BOSS结构体
struct boss
{
    int x;
    int y;
    int width;
    int height;
    int HP;
    int maxHP;
    int alive;  // 是否存活
    int move_dir;  // 移动方向
    int boss_index; // BOSS编号（1:第一个BOSS, 2:第二个BOSS）
    
    // 新增：运动轨迹相关
    int move_pattern;     // 运动模式 0:左右移动 1:上下移动 2:圆周运动 3:正弦波 4:8字运动
    int pattern_timer;    // 模式计时器
    int pattern_param1;   // 模式参数1（用于存储角度、半径等）
    int pattern_param2;   // 模式参数2
    float move_angle;     // 用于圆周/正弦运动的当前角度
    float move_speed;     // 运动速度
    float pattern_speed;  // 模式变化速度
    
    // 新增：BOSS图片相关
    const unsigned char* boss_pic;  // BOSS图片指针
    int pic_width;         // 图片宽度
    int pic_height;        // 图片高度
    
    // 新增：时间静止效果相关
    int time_stop_active;     // 是否激活时间静止
    int time_stop_timer;      // 时间静止计时器
    int time_stop_duration;   // 时间静止持续时间（帧数）
    int time_stop_cooldown;   // 时间静止冷却时间
    int flashing;             // 是否在闪烁
    int flash_timer;          // 闪烁计时器
    int flash_state;          // 闪烁状态（0:正常，1:高亮）
};

// 爆炸效果结构体
struct explosion
{
    int x;
    int y;
    int frame;
    int active;
    int type; // 0:普通爆炸, 1:玩家死亡爆炸, 2:BOSS死亡爆炸
};

// 游戏控制结构体
struct game_control
{
    int game_state;      // 游戏状态
    int boss_count;      // 已击败的BOSS数量
    int total_bosses;    // 总BOSS数量
    int game_time;       // 游戏时间（帧数）
    int player_score;    // 玩家分数
    int show_game_over;  // 显示游戏结束画面
    int game_over_timer; // 游戏结束计时器
    
    // 新增：时间静止全局控制
    int time_stop_effect;  // 是否处于时间静止效果中
    int time_stop_alpha;   // 时间静止特效的透明度
};

// 运动模式枚举
enum {
    BOSS_MOVE_LEFT_RIGHT = 0,    // 0:左右移动
    BOSS_MOVE_UP_DOWN = 1,       // 1:上下移动
    BOSS_MOVE_CIRCLE = 2,        // 2:圆周运动
    BOSS_MOVE_SINE = 3,          // 3:正弦波
    BOSS_MOVE_EIGHT = 4,         // 4:8字运动
    BOSS_MOVE_SPIRAL = 5,        // 5:螺旋运动
    BOSS_MOVE_RANDOM = 6,        // 6:随机移动
    BOSS_MOVE_CHASE = 7,         // 7:追逐玩家
    BOSS_MOVE_PATTERNS_COUNT     // 运动模式总数
};

// 常量定义
#define WARFLANE 70
#define MAX_BULLETS 200
#define BULLET_SIZE 4
#define BULLET_H 16
#define BOSS_WIDTH 120
#define BOSS_HEIGHT 100
#define BOSS_MAX_HP 500
#define BOSS_BULLET_COLOR 0xFF0000
#define PLAYER_MAX_HP 500
#define PI 3.14159265358979323846
#define TOTAL_BOSSES 2  // 总BOSS数量
#define GAME_OVER_DELAY 300  // 游戏结束画面显示帧数

// 时间静止效果常量
#define TIME_STOP_DURATION 120   // 时间静止持续时间（约2秒）
#define TIME_STOP_COOLDOWN 300   // 时间静止冷却时间（约5秒）
#define FLASH_DURATION 5         // 单次闪烁持续时间（帧）

// 声明BOSS图片数组
extern const unsigned char gImage_boss1[];
extern const unsigned char gImage_boss2[];

// 函数声明
void draw_point(int x, int y, int color);
void draw_rectangle(int x1, int y1, int x2, int y2, int color);
void draw_rectangle_alpha(int x1, int y1, int x2, int y2, int color, int alpha); // 带透明度的矩形
void draw_circle(int x, int y, int r, int color);
void draw_pic(int x, int y, int logo_w, int logo_h, const unsigned char *p);
void draw_health_bar(int x, int y, int width, int height, int currentHP, int maxHP, int border_color, int bg_color, int health_color);
void draw_text_simple(int x, int y, const char* text, int color);  // 简单文本绘制（使用矩形模拟）
int check_collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
void update_boss_movement(struct boss *boss, int screen_width, int screen_height, struct warplane *player, int frame_count);
void reset_boss(struct boss *boss, int boss_index, int screen_width);  // 重置BOSS
void create_explosion(struct explosion explosions[], int x, int y, int type);  // 创建爆炸效果
void draw_game_over_screen(struct game_control *game, int screen_width, int screen_height);  // 绘制游戏结束画面
void draw_game_info(struct game_control *game, struct warplane *player, struct boss *boss, int screen_width);  // 绘制游戏信息
void update_boss_time_stop(struct boss *boss, struct game_control *game, struct bullet bullets[], int bullet_count);  // 更新BOSS时间静止效果


// 双缓冲绘制函数
void draw_point_buffer(unsigned char *buffer, int x, int y, int color);
void draw_rectangle_buffer(unsigned char *buffer, int x1, int y1, int x2, int y2, int color);
void draw_pic_buffer(unsigned char *buffer, int x, int y, int logo_w, int logo_h, const unsigned char *p);
void draw_game_info_buffer(unsigned char *buffer, struct game_control *game,
                           struct warplane *player, struct boss *boss, int screen_width);

#endif
