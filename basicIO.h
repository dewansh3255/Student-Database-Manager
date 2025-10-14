#pragma once

#ifndef BASIC_IO_H
#define BASIC_IO_H

class basicIO {
public:
    void activateInput();
    int inputint();
    const char* inputstring();
    void inputstring(char* buffer, int size);
    void outputint(int value);
    void outputstring(const char* text);
    void terminate();
    void errorstring(const char* text);
    void errorint(int number);

    // New file operations
    long open_file(const char* pathname, int flags, int mode);
    long read_file(int fd, char* buf, long count);
    long write_file(int fd, const char* buf, long count);
    int close_file(int fd);
};

extern basicIO io;

#endif