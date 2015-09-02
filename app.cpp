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
}

App::~App(){
	if(Config::geti()->history_enabled){
        historia->truncate(Config::geti()->history_num);
		historia->save();
	}
	Config::geti()->save_config();
	log("Sprz¹tanie...");
	delete Controls::geti();
	log("Goodbye World...");
	PostQuitMessage(0);
}

void App::setInstance(HINSTANCE *hInst){
	this->hInst = hInst;
}
