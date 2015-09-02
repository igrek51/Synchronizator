#include "thread.h"
#include "app.h"

DWORD WINAPI watek(void *args){
    Thread *thread = (Thread*)args;
    thread->run();
    IO::geti()->log("P�tla w�tku zako�czona.");
    ExitThread(0);
    return 0;
}

Thread::Thread(){
    DWORD watek_id;
    running = true;
    thread_handle = CreateThread(NULL,0,watek,(void *)this,0,&watek_id);
}

Thread::~Thread(){
    running = false;
    Sleep(5); //odczekanie na zako�czenie p�tli w�tku
    TerminateThread(thread_handle, 0);
    CloseHandle(thread_handle);
    IO::geti()->log("W�tek zako�czony.");
}

bool FileSeach::is_running = false;

FileSearch::FileSearch(){
    IO::geti()->log("W�tek przeszukiwania plik�w zainicjowany.");
}

FileSearch::~FileSearch(){
    IO::geti()->log("W�tek przeszukiwania plik�w zako�czony.");
}

void FileSearch::run(){
    if(is_running) delete this;
    is_running = true;
    App::geti()->synchroman_init();
    is_running = false;
    delete this;
}
