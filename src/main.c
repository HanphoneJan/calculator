#include "calculator.h"
#include "platform.h"
#include "error.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024

int main(int argc, char* argv[]) {
    FILE* history = NULL;
    char input[MAX_INPUT_LEN];

    /* 平台初始化 */
    platform_init();

    /* 打开历史记录文件 */
    history = fopen(HISTORY_FILE, "a+");
    if (history == NULL) {
        fprintf(stderr, "Warning: Cannot open history file %s\n", HISTORY_FILE);
    }

    /* 打印欢迎界面 */
    print_welcome();

    /* 文件批处理模式 */
    if (argc > 1) {
        const char* input_file = argv[1];
        const char* output_file = (argc > 2) ? argv[2] : NULL;

        CalcError err = process_file(input_file, output_file, history);

        if (history != NULL) {
            fclose(history);
        }
        platform_cleanup();
        return (err == CALC_OK) ? 0 : 1;
    }

    /* 交互模式 */
    while (1) {
        /* 读取用户输入 */
        if (read_line(input, MAX_INPUT_LEN) != 0) {
            /* EOF或错误 */
            break;
        }

        /* 处理quit命令 */
        if (strcmp(input, "quit") == 0) {
            printf("Successful Exit.\n");
            if (history != NULL) {
                fprintf(history, "quit\n");
            }
            break;
        }

        /* 处理help命令 */
        if (strcmp(input, "help") == 0) {
            print_help();
            if (history != NULL) {
                fprintf(history, "help\n");
            }
            continue;
        }

        /* 跳过空输入 */
        if (strlen(input) == 0) {
            continue;
        }

        /* 执行计算 */
        CalcResult result = calculate_expression(input, history);

        /* 输出结果 */
        if (result.error == CALC_OK) {
            printf("\t\t\t\tResult:%.6f\n", result.value);
        } else {
            printf("\t\t\t\tError: %s\n", calc_error_string(result.error));
        }
    }

    /* 清理 */
    if (history != NULL) {
        fclose(history);
    }
    platform_cleanup();

    return 0;
}
