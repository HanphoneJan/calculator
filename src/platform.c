#include "platform.h"
#include <stdlib.h>
#include <string.h>

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#include <conio.h>
#else
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#endif

/* ANSI颜色代码 - Linux/macOS使用，Windows可支持 */
#define COLOR_RESET   "\033[0m"
#define COLOR_BOLD    "\033[1m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_WHITE   "\033[37m"

void platform_init(void) {
    /* 平台特定的初始化 */
#ifdef PLATFORM_WINDOWS
    /* Windows: 启用ANSI转义序列支持 */
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif
}

void platform_cleanup(void) {
    /* 平台特定的清理 */
}

void clear_screen(void) {
#ifdef PLATFORM_WINDOWS
    system("cls");
#else
    printf("\033[2J\033[H");
#endif
}

void set_cursor_position(int x, int y) {
#ifdef PLATFORM_WINDOWS
    COORD c;
    c.X = (SHORT)x;
    c.Y = (SHORT)y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
#else
    printf("\033[%d;%dH", y + 1, x + 1);
#endif
}

int read_line(char* buffer, int max_len) {
    if (buffer == NULL || max_len <= 0) {
        return -1;
    }

#ifdef PLATFORM_WINDOWS
    /* Windows: 使用getche实现带编辑功能的输入 */
    printf("Enter: ");
    int i = 0;
    int ch;

    while (i < max_len - 1) {
        ch = getch();

        /* Enter键结束输入 */
        if (ch == '\r' || ch == '\n') {
            break;
        }

        /* Ctrl+C */
        if (ch == 3) {
            buffer[0] = '\0';
            return -1;
        }

        /* Backspace */
        if (ch == 8) {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
            continue;
        }

        /* Ctrl+D 或 'd'/'D' 清屏 */
        if (ch == 4 || ch == 'd' || ch == 'D') {
            if (i == 0) {
                printf("\r                                  \rEnter: ");
                continue;
            }
        }

        /* 可打印字符 */
        if (ch >= 32 && ch < 127) {
            buffer[i++] = (char)ch;
            putchar(ch);
        }
    }

    buffer[i] = '\0';
    printf("\n");
    return 0;
#else
    /* Linux: 使用readline */
    char* line = readline("\033[1;34mEnter:\033[0m ");
    if (line == NULL) {
        buffer[0] = '\0';
        return -1;
    }

    /* 复制到缓冲区 */
    strncpy(buffer, line, max_len - 1);
    buffer[max_len - 1] = '\0';

    /* 添加到历史记录 */
    if (line[0] != '\0') {
        add_history(line);
    }

    free(line);
    return 0;
#endif
}

void print_colored(const char* text, int color_code) {
    (void)color_code; /* 暂时不使用 */

#ifdef PLATFORM_WINDOWS
    /* Windows可以使用SetConsoleTextAttribute，这里简化为直接输出 */
    printf("%s", text);
#else
    printf("%s", text);
#endif
}

void platform_sleep(int milliseconds) {
#ifdef PLATFORM_WINDOWS
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}

void print_welcome(void) {
#ifdef PLATFORM_WINDOWS
    clear_screen();
    set_cursor_position(39, 5);
    printf("Welcome to Hanphone's quadratic operator.\n");
    set_cursor_position(39, 6);
    printf("You can enter 'help' to ask for help.\n");
    set_cursor_position(39, 7);
    printf("You can quit through entering 'quit'.\n");
#else
    printf("\033[1;36mWelcome to Hanphone's quadratic operator.\033[0m\n");
    printf("You can enter '\033[1mhelp\033[0m' to ask for help.\n");
    printf("You can quit through entering '\033[1mquit\033[0m'.\n");
#endif
}

void print_help(void) {
    printf("\n");
    printf("\t\t\t\tYou will enter the default mode if you don't import any other file,\n");
    printf("\t\t\t\twhich means that you must input one infix expression every time by yourself.\n");
    printf("\t\t\t\tYou will enter the second mode if you import another file\n");
    printf("\t\t\t\twhich means that you input infix expression from a file.txt.\n");
    printf("\t\t\t\tThe first file should include expression and the second one will be input the result.\n");
    printf("\t\t\t\tThe second file is not necessary but once it exists,the first one should exist.\n");

#ifdef PLATFORM_WINDOWS
    printf("\t\t\t\tYou can press 'd' or 'D' to clear your input when you enter.\n");
#else
    printf("\t\t\t\tYou can press 'ctrl+d' or 'd' to clear your input when you enter.\n");
    printf("\t\t\t\tYou can press '↑' or '↓' to read historical expression when you enter.\n");
#endif

    printf("\t\t\t\tYou can enter 'help' to ask for help when you enter your expression.\n");
    printf("\t\t\t\tRemember, the expression must be infix.\n");
    printf("\t\t\t\tYou can exit through entering 'quit'.\n");
    printf("\n");
}
