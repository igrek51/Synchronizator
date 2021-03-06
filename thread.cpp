#include "thread.h"
#include "app.h"

DWORD WINAPI watek(void *args){
    Thread *thread = (Thread*)args;
    while(!thread->init) Sleep(1);
    thread->run();
    IO::geti()->log("P�tla w�tku zako�czona.");
    //ExitThread(0);
    delete thread;
    return 0;
}

Thread::Thread(){
    init = false;
    DWORD watek_id;
    thread_handle = CreateThread(NULL,0,watek,(void *)this,0,&watek_id);
}

Thread::~Thread(){
    //TerminateThread(thread_handle, 0);
    CloseHandle(thread_handle);
    IO::geti()->log("W�tek zako�czony.");
}


volatile bool FileSearch::active = false;

FileSearch::FileSearch(){
    IO::geti()->log("W�tek przeszukiwania plik�w zainicjowany.");
    init = true;
}

FileSearch::~FileSearch(){
    FileSearch::active = false;
    IO::geti()->log("W�tek przeszukiwania plik�w zako�czony.");
}

void FileSearch::run(){
    App::geti()->synchro_search();
}

void App::filesearch_start(){
    if(FileSearch::active) return;
    FileSearch::active = true;
    filesearch = new FileSearch();
}
