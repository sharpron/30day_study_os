#include "stdbool.h"

// 初始化可编程中断控制器
void init_pic(void);
// 从缓冲区获取按键编码，返回-1则表示没有
int get_keycode(void);
// 从缓冲区获取鼠标代码
int get_mousecode(void);
// 初始化键盘控制器
void init_keyboard(void);
// 启动鼠标
void enable_mouse(void);

bool has_keycode(void);

bool has_mousecode(void);
