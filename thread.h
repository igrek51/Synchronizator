#ifndef THREAD_H
#define THREAD_H

#include <windows.h>

class Thread{
public:
    Thread();
    ~Thread();
    virtual void run() = 0;
protected:
    bool running;
private:
    HANDLE thread_handle;
};

class FileSearch : public Thread {
public:
    FileSearch();
    ~FileSearch();
    static bool is_running;
private:
    void run();
};

#endif
