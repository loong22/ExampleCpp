// mpi_crash_cross_platform.cpp
// 跨平台 MPI 崩溃处理示例程序(例如: 执行Vector越界访问引发的崩溃)
// ==========================================
// 使用方法:
// 1. 编译时确保链接 MPI 库 (如 mpicxx mpi_crash_cross_platform.cpp -o mpi_crash)
// 2. 运行时使用 mpirun 或 mpiexec 启动多个进程 (如 mpirun -n 4 ./mpi_crash)
// 3. 该程序会在 Rank 0 上触发一个崩溃，展示跨平台的堆栈打印与 MPI 崩溃处理。
// 4. Windows平台使用Debug模式编译程序或Release模式(-o0优化关闭)以以获得更好的符号信息。
// 5. Linux平台使用Debug模式(-g)编译程序或Release模式(-o0优化关闭)以获得更好的符号信息。
//    可添加编译选项: -rdynamic (会输出报错函数名)
//    不添加 -rdynamic 也能工作，执行程序后需要使用 addr2line -e a.out -f 0x2bee3c 来手动解析地址。
// ==========================================

#include <mpi.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string>
#include <cstring>

// ==========================================
// 跨平台头文件与定义
// ==========================================
#ifdef _WIN32
#include <windows.h>
#include <DbgHelp.h>
#pragma comment(lib, "Dbghelp.lib") 
#else
#include <execinfo.h> // backtrace
#include <unistd.h>
#include <csignal>    // signal
#include <cxxabi.h>   // __cxa_demangle
#endif

// ==========================================
// 辅助函数：Linux 符号名还原 (Demangle)
// ==========================================
#ifndef _WIN32
std::string demangle_linux(const char* symbol) {
    size_t size;
    int status;
    char temp[256];
    char* name_start = nullptr;

    // Linux backtrace 格式通常为: ./binary(function_name+0x123) [0x456]
    // 我们尝试提取括号前的名称或者括号内的名称
    // 这是一个简单的提取逻辑：
    if (1 == sscanf(symbol, "%*[^(](%[^+]+", temp)) {
        name_start = temp;
    }
    else if (1 == sscanf(symbol, "%[^(]+", temp)) {
        // 尝试另一种格式
        name_start = temp;
    }

    if (name_start) {
        char* demangled = abi::__cxa_demangle(name_start, nullptr, &size, &status);
        if (status == 0) {
            std::string result(demangled);
            free(demangled);
            return result;
        }
    }
    return std::string(symbol);
}
#endif

// ==========================================
// 核心：打印堆栈 (跨平台实现)
// ==========================================
void print_stack_trace(int rank) {
    std::cerr << "--------------------------------------------------------\n";
    std::cerr << "[MPI Rank " << rank << "] Call Stack:\n";

#ifdef _WIN32
    // --- Windows 实现 ---
    void* stack[63];
    unsigned short frames;
    HANDLE process = GetCurrentProcess();

    SymInitialize(process, NULL, TRUE);
    frames = CaptureStackBackTrace(0, 63, stack, NULL);

    SYMBOL_INFO* symbol = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
    symbol->MaxNameLen = 255;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

    for (unsigned int i = 0; i < frames; i++) {
        SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);
        // 简单过滤掉一些系统底层函数，使输出更清晰
        std::cerr << "  #" << i << ": "
            << symbol->Name << "() [Addr: 0x" << std::hex << symbol->Address << std::dec << "]\n";
    }
    free(symbol);

#else
    // --- Linux 实现 ---
    const int max_frames = 20;
    void* buffer[max_frames];
    int nptrs = backtrace(buffer, max_frames);
    char** strings = backtrace_symbols(buffer, nptrs);

    if (strings == nullptr) {
        std::cerr << "  (Failed to capture backtrace)\n";
    }
    else {
        for (int i = 0; i < nptrs; i++) {
            std::cerr << "  #" << i << " : " << demangle_linux(strings[i]) << "\n";
        }
        free(strings);
    }
#endif
    std::cerr << "--------------------------------------------------------\n";
}

// ==========================================
// 统一的崩溃处理逻辑
// ==========================================
void handle_crash(int sig_code, const char* sig_name) {
    int rank = -1;
    int initialized = 0;
    MPI_Initialized(&initialized);
    if (initialized) MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::cerr << "\n********************************************************\n";
    std::cerr << "[MPI Rank " << rank << "] CRASH DETECTED!\n";
    std::cerr << "Signal/Error: " << sig_name << " (" << sig_code << ")\n";
    std::cerr << "********************************************************\n";

    print_stack_trace(rank);

    std::cerr << "[MPI Rank " << rank << "] Aborting MPI environment to prevent deadlock...\n";
    if (initialized) {
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    else {
        exit(1);
    }
}

// ==========================================
// Linux 信号处理入口
// ==========================================
#ifndef _WIN32
void linux_signal_handler(int sig) {
    handle_crash(sig, strsignal(sig));
}
#endif

// ==========================================
// Windows 异常过滤器入口
// ==========================================
#ifdef _WIN32
LONG WINAPI WindowsExceptionHandler(EXCEPTION_POINTERS* ExceptionInfo) {
    DWORD code = ExceptionInfo->ExceptionRecord->ExceptionCode;
    const char* err_name = "Unknown Exception";

    switch (code) {
    case EXCEPTION_ACCESS_VIOLATION: err_name = "Access Violation (SegFault)"; break;
    case EXCEPTION_BREAKPOINT:       err_name = "Breakpoint (Debug Assertion)"; break;
    case EXCEPTION_STACK_OVERFLOW:   err_name = "Stack Overflow"; break;
    case EXCEPTION_INT_DIVIDE_BY_ZERO: err_name = "Divide By Zero"; break;
    }

    handle_crash(code, err_name);
    return EXCEPTION_EXECUTE_HANDLER;
}
#endif

// ==========================================
// 初始化函数：在 main 开头调用
// ==========================================
void setup_crash_handlers() {
#ifdef _WIN32
    // 1. 捕获硬件异常 (SEH)
    SetUnhandledExceptionFilter(WindowsExceptionHandler);
    // 2. 捕获 abort() 调用 (常用于 vector 断言失败) - Windows 也支持部分 signal
    // signal(SIGABRT, [](int){ handle_crash(SIGABRT, "SIGABRT (Assertion Failed)"); });
#else
    // Linux 捕获常见崩溃信号
    signal(SIGSEGV, linux_signal_handler); // 段错误 (非法内存)
    signal(SIGABRT, linux_signal_handler); // Abort (assert失败)
    signal(SIGFPE, linux_signal_handler); // 浮点错误
    signal(SIGILL, linux_signal_handler); // 非法指令
#endif
}

// ==========================================
// 崩溃测试用例
// ==========================================
void trigger_crash() {
    std::cerr << "--> Inside trigger_crash function..." << std::endl;

    std::vector<int> v1;
    v1.resize(0);

    // 提示：
    // 在 Linux Release 模式下，v1[0] 属于未定义行为，未必会崩溃。
    // 在 Windows Debug 模式下，v1[0] 会触发 Assert (SIGABRT/Breakpoint)。
    // 为了保证演示效果，如果 v[0] 没挂，我们手动补一刀。

    std::cerr << "--> Accessing vector[0]..." << std::endl;
    int val = v1[0];

    // 如果上面的代码没挂（比如在 Linux Release 下可能读到脏数据），打印下面这行
    std::cerr << "--> Vector access did not crash! Value read: " << val << std::endl;

    // 强制制造一个 SegFault 以测试处理器是否生效
    //std::cerr << "--> Forcing a nullptr crash for demonstration..." << std::endl;
    //volatile int* ptr = nullptr;
    //*ptr = 42;
}

void complex_calculation() {
    trigger_crash();
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    // 【关键】注册所有的崩溃处理器
    setup_crash_handlers();

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        std::cout << "Starting simulation on Rank 0..." << std::endl;
        complex_calculation();
    }

    MPI_Finalize();
    return 0;
}