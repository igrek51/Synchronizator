#ifndef THREAD_H
#define THREAD_H

#include <windows.h>

class Thread{
public:
    Thread();
    virtual ~Thread();
    virtual void run() = 0;
    volatile bool init;
private:
    HANDLE thread_handle;
};

class FileSearch : public Thread {
public:
    FileSearch();
    ~FileSearch();
    static volatile bool active;
private:
    void run();
};

#endif
