#include "thread.h"
#include "app.h"

DWORD WINAPI watek(void *args){
    Thread *thread = (Thread*)args;
    while(!thread->init) Sleep(1);
    thread->run();
    IO::geti()->log("Pêtla w¹tku zakoñczona.");
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
    IO::geti()->log("W¹tek zakoñczony.");
}


volatile bool FileSearch::active = false;

FileSearch::FileSearch(){
    IO::geti()->log("W¹tek przeszukiwania plików zainicjowany.");
    init = true;
}

FileSearch::~FileSearch(){
    FileSearch::active = false;
    IO::geti()->log("W¹tek przeszukiwania plików zakoñczony.");
}

void FileSearch::run(){
    App::geti()->synchro_search();
}

void App::filesearch_start(){
    if(FileSearch::active) return;
    FileSearch::active = true;
    filesearch = new FileSearch();
}
