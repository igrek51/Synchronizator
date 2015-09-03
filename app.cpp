#include "app.h"

App* App::instance = NULL;

App* App::geti(){
    if(instance == NULL){
        instance = new App();
    }
    return instance;
}

App::App(){
    instance = this;
    historia = NULL;
    thread_active = false;
    historia = new History();
    zadania = new vector<Task*>;
    filesearch = NULL;
}

App::~App(){
	if(Config::geti()->history_enabled){
        historia->truncate(Config::geti()->history_num);
		historia->save();
	}
    Config::geti()->save_config();
    IO::geti()->log("Sprz¹tanie...");
    if(filesearch!=NULL) delete filesearch;
	delete Controls::geti();
    tasks_clear(zadania);
	IO::geti()->log("Goodbye World...");
	PostQuitMessage(0);
}
