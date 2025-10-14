#include "basicIO.h"

#define SYS_READ 0
#define SYS_WRITE 1
#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define SYS_OPEN 2
#define SYS_CLOSE 3

basicIO io;

// The full definition of the syscall function, including the assembly code.
// This resolves the "undefined reference" linker error.
extern "C" long syscall3(long number, long arg1, long arg2, long arg3) {
    long ret;
    asm volatile(
        "syscall"
        : "=a"(ret)
        : "a"(number), "D"(arg1), "S"(arg2), "d"(arg3)
        : "rcx", "r11", "memory");
    return ret;
}

static char inputBuffer[256];

void basicIO::activateInput() {
    for (int i = 0; i < 256; ++i) inputBuffer[i] = 0;
}

int basicIO::inputint() {
    char buffer[32] = {0};
    long bytes = syscall3(SYS_READ, STDIN, (long)buffer, 31);
    if (bytes <= 0) return 0;
    buffer[bytes] = '\0';
    int result = 0;
    int i = 0;
    bool neg = false;
    if (buffer[0] == '-') {
        neg = true;
        i++;
    }
    for (; buffer[i] >= '0' && buffer[i] <= '9'; ++i) {
        result = result * 10 + (buffer[i] - '0');
    }
    return neg ? -result : result;
}

const char* basicIO::inputstring() {
    syscall3(SYS_READ, STDIN, (long)inputBuffer, 255);
    return inputBuffer;
}


void basicIO::inputstring(char* buffer, int size) {
    if (!buffer || size <= 0) return;

    char ch;
    int i = 0;

    while (i < size - 1) {
        long bytes = syscall3(SYS_READ, STDIN, (long)&ch, 1);
        if (bytes <= 0 || ch == '\n') break;
        buffer[i++] = ch;
    }
    buffer[i] = '\0';
}

void basicIO::outputint(int number) {
    char buffer[32];
    int i = 0;
    bool isNegative = false;
    if (number == 0) {
        buffer[i++] = '0';
    } else {
        if (number < 0) {
            isNegative = true;
            number = -number;
        }
        char tmp[32];
        int t = 0;
        while (number > 0) {
            tmp[t++] = '0' + (number % 10);
            number /= 10;
        }
        if (isNegative) {
            buffer[i++] = '-';
        }
        for(int j = t - 1; j >= 0; j--){
            buffer[i++] = tmp[j];
        }
    }
    syscall3(SYS_WRITE, STDOUT, (long)buffer, i);
}

void basicIO::outputstring(const char* text) {
    long len = 0;
    while (text[len]) ++len;
    syscall3(SYS_WRITE, STDOUT, (long)text, len);
}

void basicIO::terminate() {
    char newline = '\n';
    syscall3(SYS_WRITE, STDOUT, (long)&newline, 1);
}

void basicIO::errorstring(const char* text) {
    long len = 0;
    while (text[len]) ++len;
    syscall3(SYS_WRITE, STDERR, (long)text, len);
}

void basicIO::errorint(int number) {
    char buffer[32];
    int i = 0;
    bool isNegative = false;
    if (number == 0) {
        buffer[i++] = '0';
    } else {
        if (number < 0) {
            isNegative = true;
            number = -number;
        }
        char tmp[32];
        int t = 0;
        while (number > 0) {
            tmp[t++] = '0' + (number % 10);
            number /= 10;
        }
        if (isNegative) {
            buffer[i++] = '-';
        }
        for(int j = t - 1; j >= 0; j--){
            buffer[i++] = tmp[j];
        }
    }
    syscall3(SYS_WRITE, STDERR, (long)buffer, i);
}

// New file operations
long basicIO::open_file(const char* pathname, int flags, int mode) {
    return syscall3(SYS_OPEN, (long)pathname, flags, mode);
}

long basicIO::read_file(int fd, char* buf, long count) {
    return syscall3(SYS_READ, fd, (long)buf, count);
}

long basicIO::write_file(int fd, const char* buf, long count) {
    return syscall3(SYS_WRITE, fd, (long)buf, count);
}

int basicIO::close_file(int fd) {
    return syscall3(SYS_CLOSE, fd, 0, 0);
}