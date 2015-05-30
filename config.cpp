#include "app.h"

void App::load_config(){
	log("Wczytywanie ustawieñ");
	//ustawienia domyœlne
	save_wnd_pos = 0;
	wnd_pos_x = 0;
	wnd_pos_y = 0;
	save_wnd_size = 0;
	synchro_paths_num = 0;
	log_enabled = 1;
	history_enabled = 1;
	if(!file_exists("conf.ini")){
		echo("! Brak pliku konfiguracyjnego - ustawienia domyœlne");
		return;
	}
	char *temp = new char [512];
	//GetPrivateProfileString("dupa","d",".",temp,512,".\\conf.ini");
	//songs_dir = temp;
	RECT wnd_rect;
	//zapisane po³o¿enie okna
	save_wnd_pos=GetPrivateProfileInt("app_window","save_wnd_pos",0,".\\conf.ini");
	if(save_wnd_pos==2){ //reset ustawieñ
		GetWindowRect(hwnd, &wnd_rect);
		wnd_pos_x = wnd_rect.left;
		wnd_pos_y = wnd_rect.top;
		save_wnd_pos=1;
	}else if(save_wnd_pos==1){
		wnd_pos_x=GetPrivateProfileInt("app_window","wnd_pos_x",0,".\\conf.ini");
		wnd_pos_y=GetPrivateProfileInt("app_window","wnd_pos_y",0,".\\conf.ini");
	}else{
		save_wnd_pos=0;
	}
	//zapisany rozmiar okna
	save_wnd_size=GetPrivateProfileInt("app_window","save_wnd_size",0,".\\conf.ini");
	if(save_wnd_size==2){
		GetWindowRect(hwnd, &wnd_rect);
		wnd_w = wnd_rect.right-wnd_rect.left;
		wnd_h = wnd_rect.bottom-wnd_rect.top;
		save_wnd_size=1;
	}else if(save_wnd_size==1){
		wnd_w=GetPrivateProfileInt("app_window","wnd_w",440,".\\conf.ini");
		wnd_h=GetPrivateProfileInt("app_window","wnd_h",400,".\\conf.ini");
	}else{
		save_wnd_size=0;
	}
	//liczba katalogów do synchronizacji
	synchro_paths_num=GetPrivateProfileInt("synchro_paths","synchro_paths_num",0,".\\conf.ini");
	if(synchro_paths_num<=0){
		synchro_paths_num = 0;
		synchro_paths_source = NULL;
		synchro_paths_dest = NULL;
		synchro_paths_content = NULL;
	}else{
		synchro_paths_source = new string [synchro_paths_num];
		synchro_paths_dest = new string [synchro_paths_num];
		synchro_paths_content = new int [synchro_paths_num];
	}
	for(int i=0; i<synchro_paths_num; i++){
		ss_clear(ss);
		ss<<"synchro_path_"<<i+1<<"_source";
		GetPrivateProfileString("synchro_paths",ss.str().c_str(),".",temp,512,".\\conf.ini");
		synchro_paths_source[i] = temp;
		ss_clear(ss);
		ss<<"synchro_path_"<<i+1<<"_dest";
		GetPrivateProfileString("synchro_paths",ss.str().c_str(),".",temp,512,".\\conf.ini");
		synchro_paths_dest[i] = temp;
		ss_clear(ss);
		ss<<"synchro_path_"<<i+1<<"_content";
		synchro_paths_content[i] = GetPrivateProfileInt("synchro_paths",ss.str().c_str(),0,".\\conf.ini");
		if(synchro_paths_content[i]!=1) synchro_paths_content[i]=0;
	}
	//wyjœcie
	log_enabled=GetPrivateProfileInt("verbose","log_enabled",1,".\\conf.ini");
	history_enabled=GetPrivateProfileInt("verbose","history_enabled",1,".\\conf.ini");
	delete[] temp;
}

void App::save_config(){
	log("Zapisywanie ustawieñ");
	if(!file_exists("conf.ini")){
		echo("! Brak pliku konfiguracyjnego - tworzenie nowego");
	}
	char *temp = new char [512];
	WritePrivateProfileString("app_window","save_wnd_pos",itoa(save_wnd_pos,temp,10),".\\conf.ini");
	if(save_wnd_pos==1){
		WritePrivateProfileString("app_window","wnd_pos_x",itoa(wnd_pos_x,temp,10),".\\conf.ini");
		WritePrivateProfileString("app_window","wnd_pos_y",itoa(wnd_pos_y,temp,10),".\\conf.ini");
	}
	WritePrivateProfileString("app_window","save_wnd_size",itoa(save_wnd_size,temp,10),".\\conf.ini");
	if(save_wnd_size==1){
		WritePrivateProfileString("app_window","wnd_w",itoa(wnd_w,temp,10),".\\conf.ini");
		WritePrivateProfileString("app_window","wnd_h",itoa(wnd_h,temp,10),".\\conf.ini");
	}
	delete[] temp;
}
