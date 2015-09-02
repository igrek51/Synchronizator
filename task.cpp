#include "task.h"
#include "app.h"

Task::Task(string filename, string dir1, string dir2, int code){
    this->filename = filename;
    this->dir1 = dir1;
    this->dir2 = dir2;
    this->code = code;
}

string Task::name(){
    if(code==TASK_BRAK_FOLDERU) return "Brak folderu";
	if(code==TASK_BRAK_PLIKU) return "Brak pliku";
	if(code==TASK_INNY_ROZMIAR) return "Inny rozmiar pliku";
	if(code==TASK_INNA_WERSJA) return "Inna wersja pliku";
	if(code==TASK_ZBEDNY_FOLDER) return "Zbêdny folder";
	if(code==TASK_ZBEDNY_PLIK) return "Zbêdny plik";
	return "";
}

vector<string>* Task::cmds(){
    vector<string>* cmdsy = new vector<string>;
    switch(code){
		case TASK_BRAK_FOLDERU:{
			cmdsy->push_back("md \""+dir1+"\\"+filename+"\"");
			cmdsy->push_back("xcopy \""+dir1+"\\"+filename+"\\*.*\" \""+dir2+"\\"+filename+"\\*.*\" /e /c");
		}break;
		case TASK_BRAK_PLIKU:{
			cmdsy->push_back("copy \""+dir1+"\\"+filename+"\" \""+dir2+"\\"+filename+"\"");
		}break;
		case TASK_INNY_ROZMIAR:{
			cmdsy->push_back("del \""+dir2+"\\"+filename+"\"");
			cmdsy->push_back("copy \""+dir1+"\\"+filename+"\" \""+dir2+"\\"+filename+"\"");
		}break;
		case TASK_INNA_WERSJA:{
			cmdsy->push_back("del \""+dir2+"\\"+filename+"\"");
			cmdsy->push_back("copy \""+dir1+"\\"+filename+"\" \""+dir2+"\\"+filename+"\"");
		}break;
		case TASK_ZBEDNY_FOLDER:{
			cmdsy->push_back("rd \""+dir2+"\\"+filename+"\" /s /q");
		}break;
		case TASK_ZBEDNY_PLIK:{
			cmdsy->push_back("del \""+dir2+"\\"+filename+"\"");
		}break;
	}
    return cmdsy;
}

int Task::cmds_number(){
    vector<string>* cmdsy = cmds();
    int number = cmdsy.size();
    delete cmdsy;
    return number;
}

void Task::invert(){
    string temp = dir1;
	dir1 = dir2;
	dir2 = temp;
	if(code==TASK_BRAK_FOLDERU){
		code=TASK_ZBEDNY_FOLDER;
	}else if(code==TASK_BRAK_PLIKU){
		code=TASK_ZBEDNY_PLIK;
	}else if(code==TASK_ZBEDNY_FOLDER){
		code=TASK_BRAK_FOLDERU;
	}else if(code==TASK_ZBEDNY_PLIK){
		code=TASK_BRAK_PLIKU;
	}
}

void Task::execute(){
    vector<string>* cmdsy = cmds();
	for(unsigned int i=0; i<cmdsy->size(); i++){
		exec_cmd(cmdsy->at(i));
	}
    delete cmdsy;
}


void App::exec_cmd(string l){
	log("Wykonywanie: "+l);
    if(system(l.c_str())==0){
        history_add_time(&historia,l);
    }else{
        IO::geti()->error("Blad polecenia: "+l);
    }
}

void App::tasks_execute(vector<Task*>* zadania){
    for(unsigned int i=0; i<zadania->size(); i++){
        set_progress(double(i)/zadania->size());
		zadania->execute();
	}
	set_progress(1);
}
