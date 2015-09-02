#include "io.h"
#include "config.h"
#include "files.h"
#include "controls.h"
#include "strings.h"
#include "app.h"

#include <fstream>
#include <ctime>
#include <windows.h>

IO* IO::instance = NULL;

IO* IO::geti(){
    if(instance == NULL){
        instance = new IO();
    }
    return instance;
}

IO* IO::i(){
    return geti();
}

IO::IO(){
    instance = this;
    last_echo = "";
    repeated_echo = 0;
    clock_last = clock();
}


void IO::clear_log(){
    if(!Config::geti()->log_enabled) return;
    clear_file(Config::geti()->log_filename);
}

void IO::delete_log(){
    if(file_exists(Config::geti()->log_filename)){
        DeleteFile(Config::geti()->log_filename.c_str());
    }
}

void IO::log(string l){
    if(!Config::geti()->log_enabled) return;
    if(!file_exists(Config::geti()->log_filename)) clear_log();
    fstream plik;
    plik.open(Config::geti()->log_filename.c_str(), fstream::out|fstream::app);
    if(!plik.good()){
        plik.close();
        message_box("B³¹d", "B³¹d zapisu do pliku dziennika: "+Config::geti()->log_filename);
        return;
    }
    plik<<get_time()<<" - "<<l<<endl;
    plik.close();
}

void IO::log(int l){
    stringstream ss;
    ss<<l;
    log(ss.str());
}

void IO::log(string s, int l){
    stringstream ss;
    ss<<s<<": "<<l;
    log(ss.str());
}

void IO::error(string l, bool show_output){
    if(show_output){
        echo("[B£¥D!] - "+l);
    }else{
        log("[B£¥D!] - "+l);
    }
}

void IO::critical_error(string l){
    log("[B£¥D KRYTYCZNY!] - "+l);
    message_box("B³¹d krytyczny", l);
}


void IO::echo(string s){
    Controls::geti()->set_text(Config::geti()->output_control, s.c_str());
	log(s);
    App::geti()->text_vcenter();
}

void IO::echo(int e){
    stringstream ss;
    ss<<e;
    echo(ss.str());
}

void IO::message_box(string title, string message){
    if(message.length()==0){
        message = title;
        title = "Wiadomoœæ";
    }
    MessageBox(NULL, message.c_str(), title.c_str(), MB_OK|MB_ICONINFORMATION);
}

string get_time(){
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    stringstream ss;
    if(timeinfo->tm_hour<10) ss<<"0";
    ss<<timeinfo->tm_hour<<":";
    if(timeinfo->tm_min<10) ss<<"0";
    ss<<timeinfo->tm_min<<":";
    if(timeinfo->tm_sec<10) ss<<"0";
    ss<<timeinfo->tm_sec<<", ";
    if(timeinfo->tm_mday<10) ss<<"0";
    ss<<timeinfo->tm_mday<<".";
    if(timeinfo->tm_mon+1<10) ss<<"0";
    ss<<timeinfo->tm_mon+1<<".";
    ss<<timeinfo->tm_year+1900;
    return ss.str();
}

void ss_clear(stringstream &sstream){
	sstream.str("");
	sstream.clear();
}
