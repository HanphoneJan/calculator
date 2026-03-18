#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdio.h>

/* 平台检测 */
#ifdef _WIN32
    #define PLATFORM_WINDOWS
#elif defined(__linux__)
    #define PLATFORM_LINUX
#else
    #error "Unsupported platform"
#endif

/* 路径分隔符和历史文件路径 */
#ifdef PLATFORM_WINDOWS
    #define PATH_SEP "\\"
    #define HISTORY_FILE ".\\history.txt"
#else
    #define PATH_SEP "/"
    #define HISTORY_FILE "./history.txt"
#endif

/* 初始化/清理 */
void platform_init(void);
void platform_cleanup(void);

/* 控制台操作 */
void clear_screen(void);
void set_cursor_position(int x, int y);

/* 输入处理 - 读取一行用户输入 */
/* 返回值: 0成功, -1失败(EOF或其他错误) */
int read_line(char* buffer, int max_len);

/* 打印带颜色的文本 */
void print_colored(const char* text, int color_code);

/* 休眠（毫秒） */
void platform_sleep(int milliseconds);

/* 打印欢迎界面 */
void print_welcome(void);

/* 打印帮助信息 */
void print_help(void);

#endif /* PLATFORM_H */
