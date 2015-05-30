#include "app.h"
#include <fstream>
#include <ctime>

void App::clear_file(string filename){
	fstream plik;
	plik.open(filename.c_str(),fstream::out|fstream::trunc);
	plik.close();
}

void App::log(string l){
	if(log_enabled!=1) return;
	fstream plik;
	plik.open("log.txt",fstream::out|fstream::app);
	plik<<time(NULL)<<" - "<<l<<endl;
	plik.close();
}

void App::log(int l){
	ss_clear(ss);
	ss<<l;
	log(ss.str());
}

void App::echo(string s){
	SetWindowText(hctrl[0],s.c_str());
	text_vcenter();
	UpdateWindow(hwnd);
	log(s);
}

void App::text_vcenter(){
	int lines = SendMessage(hctrl[0],EM_GETLINECOUNT,0,0);
	int height = fontsize*lines;
	int ypos = (50-height)/2;
	if(ypos<0) ypos=0;
	if(height>52) height=52;
	SetWindowPos(hctrl[0],HWND_BOTTOM,0,ypos,window_w,height,0);
}

void App::echo(){
	echo(ss.str());
	ss_clear(ss);
}

void App::echo(int e){
	ss_clear(ss);
	ss<<e;
	echo(ss.str());
}

bool App::file_exists(string name){
	ifstream plik(name.c_str());
	if(plik.good()){
		plik.close();
		return true;
	}else{
		plik.close();
		return false;
	}
}

void App::ss_clear(stringstream &sstream){
	sstream.str("");
	sstream.clear();
}

void App::get_argv(LPSTR lpCmdLine){
	string arg=(char*)lpCmdLine;
	if(arg.length()==0){
		argc=0;
		argv=NULL;
	}else{
		bool cudzyslow=0;
		argc=1;
		for(unsigned int i=0; i<arg.length(); i++){
			if(arg[i]=='\"') cudzyslow=!cudzyslow;
			if(arg[i]==' '&&!cudzyslow) argc++;
		}
		argv = new string [argc];
		if(argc==1){
			argv[0]=arg;
		}else{
			int spaces=0;
			cudzyslow=0;
			for(int i=0; i<argc; i++) argv[i]="";
			for(unsigned int i=0; i<arg.length(); i++){
				if(arg[i]=='\"') cudzyslow=!cudzyslow;
				if(arg[i]==' '&&!cudzyslow){
					spaces++;
				}else{
					argv[spaces]+=arg[i];
				}
			}
		}
	}
	ss_clear(ss);
	ss<<"Parametry ["<<argc<<"]: ";
	for(int i=0; i<argc; i++) ss<<argv[i]<<" ";
	log(ss.str());
}

bool App::is_arg(string par){
	for(int i=0; i<argc; i++){
		if(argv[i]==par) return true;
	}
	return false;
}

void App::message(string m){
	MessageBox(NULL,m.c_str(),"Info",MB_OK|MB_ICONINFORMATION);
}
