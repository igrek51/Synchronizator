#include "app.h"

DWORD WINAPI watek1(void * args){
	App *app = (App*)args;
	app->exec_thread();
	ExitThread(0);
	return 0;
}

void App::exec_thread(){
	if(thread_active) return;
	thread_active=true;
	synchroman_init();
	thread_active=false;
}

void App::start_thread(){
	DWORD watek1_id;
	CreateThread(NULL,0,watek1,(void *)this,0,&watek1_id);
	//HANDLE hThread =( HANDLE ) _beginthread( ThreadProc, 0, NULL );
	//WaitForSingleObject(w1,INFINITE);
	//CloseHandle(w1);
}
