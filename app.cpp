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
}

App::~App(){
	if(Config::geti()->history_enabled){
        historia->truncate(Config::geti()->history_num);
		historia->save();
	}
    Config::geti()->save_config();
	IO::geti()->log("Sprz¹tanie...");
	delete Controls::geti();
	IO::geti()->log("Goodbye World...");
	PostQuitMessage(0);
}
