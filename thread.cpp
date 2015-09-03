#include "thread.h"
#include "app.h"

DWORD WINAPI watek(void *args){
    Thread *thread = (Thread*)args;
    thread->run();
    IO::geti()->log("Pêtla w¹tku zakoñczona.");
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
    Sleep(5); //odczekanie na zakoñczenie pêtli w¹tku
    TerminateThread(thread_handle, 0);
    CloseHandle(thread_handle);
    IO::geti()->log("W¹tek zakoñczony.");
}

FileSearch::FileSearch(){
    IO::geti()->log("W¹tek przeszukiwania plików zainicjowany.");
}

FileSearch::~FileSearch(){
    IO::geti()->log("W¹tek przeszukiwania plików zakoñczony.");
}

void FileSearch::run(){
    if(App::geti()->thread_active) delete this;
    App::geti()->thread_active = true;
    App::geti()->synchroman_init();
    App::geti()->thread_active = false;
    delete this;
}
