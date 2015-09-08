#include "app.h"
#include "strings.h"
#include "files.h"

#include <QLabel>
#include <QProgressBar>

void App::buttons_enable(bool enable){
    Controls::geti()->find("porownaj_pliki")->setEnabled(enable);
    Controls::geti()->find("odwroc")->setEnabled(enable);
    Controls::geti()->find("wykonaj1")->setEnabled(enable);
    Controls::geti()->find("2foldery")->setEnabled(enable);
    Controls::geti()->find("usun")->setEnabled(enable);
    Controls::geti()->find("wykonaj")->setEnabled(enable);
}

void App::controls_fonts_set(){
    for(unsigned int i=0; i<Controls::geti()->controls.size(); i++){
        string fontface = Config::geti()->buttons_fontface;
        int fontsize = Config::geti()->buttons_fontsize;
        Controls::geti()->set_font(Controls::geti()->controls.at(i)->handle, fontface, fontsize);
    }
}

void App::set_progress(double p){
    if(p>1) p=1;
    if(p<0) p=0;
    QProgressBar* handle = dynamic_cast<QProgressBar*>(Controls::geti()->find("progress_bar"));
    if(handle==NULL) return;
    handle->setValue(p*100);
    this->repaint();
}

void App::synchro_search(){
	set_progress(0);
	IO::geti()->echo("Wyszukiwanie dysku...");
	string drive = select_drive();
	if(drive.length()==0) return;
	if(zadania->size()>0) tasks_clear(zadania);
	//dla każdego synchronizowanego katalogu
    stringstream ss;
	for(unsigned int i=0; i<Config::geti()->synchropaths.size(); i++){
        string source = Config::geti()->synchropaths.at(i)->source;
        string dest = drive+Config::geti()->synchropaths.at(i)->dest;
        double prog_from = double(i)/Config::geti()->synchropaths.size();
        double prog_to = double(i+1)/Config::geti()->synchropaths.size();
        ss_clear(ss);
		ss<<"Przeszukiwanie folderu \""<<drive<<dest<<"\"...";
		IO::geti()->echo(ss.str());
		dirlist_cmp(source, dest, Config::geti()->synchropaths.at(i)->content_check?true:false, prog_from, prog_to);
	}
	set_progress(1);
	ss_clear(ss);
	ss<<"Zakończono przeszukiwanie.\r\n";
	if(zadania->size()==0){
		ss<<"Brak elementów do synchronizacji.";
	}else{
		ss<<"Znalezione elementy synchronizacji: "<<zadania->size();
	}
	IO::geti()->echo(ss.str());
	show_lista();
}

void App::otworz_foldery(){
    int n = listbox_current();
    if(n==-1){
        IO::geti()->error("Nie wybrano elementu.");
        return;
    }
    if(zadania->size()==0) return;
    Task* task = zadania->at(n);
    stringstream ss;
    ss<<"thunar \""<<task->dir1.c_str()<<"\" &";
    exec_cmd(ss.str());
    ss_clear(ss);
    ss<<"thunar \""<<task->dir2.c_str()<<"\" &";
    exec_cmd(ss.str());
}

void App::viewer_open(string file){
    stringstream ss;
    if(Config::geti()->external_viewer.length()==0){
        ss<<"gedit \""<<file<<"\" &";
        exec_cmd(ss.str());
    }else{
        ss<<"\""<<Config::geti()->external_viewer<<"\" \""<<file<<"\" &";
        exec_cmd(ss.str());
    }
}

void App::otworz_pliki(){
	int n = listbox_current();
	if(n==-1){
		IO::geti()->error("Nie wybrano elementu.");
		return;
	}
    if(zadania->size()==0) return;
    Task* task = zadania->at(n);
    if(task->code!=TASK_INNY_ROZMIAR && task->code!=TASK_INNA_WERSJA){
        IO::geti()->error("Funkcja niedostępna dla tych elementów.");
        return;
    }
    viewer_open(task->dir1+"/"+task->filename);
    viewer_open(task->dir2+"/"+task->filename);
    files_cmp(task->dir1+"/"+task->filename, task->dir2+"/"+task->filename, true);
}

void App::pb_usun(){
    /*
    if(FileSearch::active){
		IO::geti()->error("Trwa przeszukiwanie.");
		return;
	}
    */
	int cr=listbox_current();
	if(cr==-1){
		IO::geti()->error("Nie wybrano elementu.");
		return;
	}
    if(zadania->size()==0) return;
    zadania->erase(zadania->begin() + cr);
	show_lista();
    IO::geti()->echo("Usunięto element synchronizacji z listy.");
}

void App::pb_odwroc(){
	int n = listbox_current();
	if(n==-1){
		IO::geti()->error("Nie wybrano elementu.");
		return;
	}
	if(zadania->size()==0) return;
    Task* task = zadania->at(n);
    task->invert();
    show_lista();
    listbox_select(n);
    IO::geti()->echo("Odwrócono kierunek synchronizacji dla wybranego elementu.");
}

void App::wykonaj_1(){
    /*
	if(FileSearch::active){
		IO::geti()->error("Trwa przeszukiwanie.");
		return;
	}
    */
	int cr=listbox_current();
	if(cr==-1){
		IO::geti()->error("Nie wybrano elementu.");
		return;
	}
    if(zadania->size()==0) return;
    zadania->at(cr)->execute();
    zadania->erase(zadania->begin() + cr);
	show_lista();
	listbox_select(cr);
    IO::geti()->echo("Wykonano zadania 1 elementu synchronizacji.");
}

void App::wykonaj_wszystko(){
    /*
	if(FileSearch::active){
		IO::geti()->error("Trwa przeszukiwanie.");
		return;
	}
    */
	IO::geti()->echo("Wykonywanie zadań...");
	if(zadania->size()==0){
		IO::geti()->error("Lista zadań jest pusta.");
		return;
	}
    execute_all_tasks(zadania);
    tasks_clear(zadania);
	show_lista();
	IO::geti()->echo("Zakończono synchronizację.");
}
