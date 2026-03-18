# C Calculator

<p align="center">
  <img src="https://img.shields.io/badge/C-99-blue.svg" alt="C99">
  <img src="https://img.shields.io/badge/platform-Linux%20%7C%20Windows-lightgrey.svg" alt="Platform">
  <img src="https://img.shields.io/badge/license-MIT-green.svg" alt="License">
</p>

<p align="center">一个轻量级、跨平台的四则运算计算器</p>

<p align="center">
  <a href="#功能特性">功能特性</a> •
  <a href="#快速开始">快速开始</a> •
  <a href="#使用指南">使用指南</a> •
  <a href="#项目结构">项目结构</a> •
  <a href="#开发">开发</a> •
</p>

---

## 功能特性

- **跨平台支持** - 原生支持 Linux 和 Windows，使用条件编译实现代码复用
- **双运行模式** - 交互式命令行模式和文件批处理模式
- **健壮的错误处理** - 完善的错误码体系和内存安全管理
- **历史记录** - 自动保存计算历史到本地文件
- **运算符优先级** - 完整支持括号和中缀表达式求值
- **负数支持** - 正确处理负数和减法运算

## 快速开始

### 前置要求

| 平台    | 编译器    | 依赖库            |
| ------- | --------- | ----------------- |
| Linux   | GCC 4.8+  | readline, ncurses |
| Windows | MinGW-w64 | 无额外依赖        |
| MSYS2   | GCC       | 无额外依赖        |

### 安装依赖

**Ubuntu/Debian:**

```bash
sudo apt-get update
sudo apt-get install build-essential libreadline-dev libncurses-dev
```

**CentOS/RHEL/Fedora:**

```bash
sudo yum install gcc make readline-devel ncurses-devel
```

**macOS (使用 Homebrew):**

```bash
brew install readline
```

### 构建

```bash
# 克隆仓库
git clone https://github.com/hanphone/calculator.git
cd calculator

# Linux
make -f Makefile.linux

# Windows (MinGW)
make -f Makefile.windows

# MSYS2
make -f Makefile.msys2
```

### 运行

```bash
# 交互模式
./calculator

# 文件批处理模式
./calculator input.txt output.txt
```

## 使用指南

### 交互模式

启动程序后，输入数学表达式即可得到计算结果：

```
$ ./calculator

Welcome to C Calculator
You can enter 'help' to ask for help.
You can quit through entering 'quit'.

Enter: 1+2*3
                        Result:7.000000

Enter: (1+2)*3
                        Result:9.000000

Enter: -5+10
                        Result:5.000000

Enter: quit
Successful Exit.
```

**支持的运算符:**

| 运算符 | 说明 | 优先级 |
| ------ | ---- | ------ |
| `+`  | 加法 | 1 (低) |
| `-`  | 减法 | 1 (低) |
| `*`  | 乘法 | 2 (高) |
| `/`  | 除法 | 2 (高) |
| `()` | 括号 | 最高   |

**内置命令:**

- `help` - 显示帮助信息
- `quit` - 退出程序

### 文件批处理模式

创建包含表达式的输入文件：

```bash
$ cat input.txt
1+2
3*4
(5+6)*2
10/0
```

运行批处理：

```bash
$ ./calculator input.txt output.txt
```

查看结果：

```bash
$ cat output.txt
Result1:3.000000
Result2:12.000000
Result3:22.000000
Result4:Error - Division by zero
```

## 项目结构

```
calculator/
├── src/                      # 源代码目录
│   ├── main.c               # 程序入口和主循环
│   ├── calculator.c/.h      # 表达式解析和计算核心
│   ├── stack.c/.h           # 动态栈数据结构
│   ├── platform.c/.h        # 平台抽象层（输入/输出）
│   └── error.h              # 错误码定义
├── tests/                    # 测试代码
│   └── test_basic.c         # 基础功能单元测试
├── Makefile.linux            # Linux 构建配置
├── Makefile.windows          # Windows 构建配置
├── Makefile.msys2            # MSYS2 构建配置
└── README.md                 # 项目文档
```

### 核心模块说明

**Stack (栈)** - `src/stack.c`

- 动态数组实现的通用栈
- 支持 `double` 和 `char` 两种数据类型
- 自动扩容机制

**Calculator (计算器)** - `src/calculator.c`

- 基于调度场算法（Shunting Yard）的中缀表达式求值
- 完整的错误处理和内存安全
- 支持负数和浮点数运算

**Platform (平台层)** - `src/platform.c`

- 封装平台差异
- Linux 使用 readline 库提供行编辑和历史记录
- Windows 使用原生控制台 API

## 开发

### 运行测试

```bash
cd tests
gcc -o test_basic test_basic.c ../src/stack.c ../src/calculator.c ../src/platform.c -I../src -lm
./test_basic
```

### 调试构建

```bash
# Linux - 启用调试符号
make -f Makefile.linux clean
make -f Makefile.linux CFLAGS="-g -DDEBUG"

# 使用 GDB 调试
gdb ./calculator
```

### 代码规范

- 遵循 C99 标准
- 使用 4 空格缩进
- 函数命名使用小写+下划线（snake_case）
- 宏命名使用全大写+下划线
