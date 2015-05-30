#include "lista.h"
#include "app.h"

void lista_add(lista **head, string filename, string dir1, string dir2, int task_code){
	lista *nowy = new lista;
	nowy->filename = filename;
	nowy->dir1 = dir1;
	nowy->dir2 = dir2;
	nowy->task_code = task_code;
	nowy->next = NULL;
	if(*head==NULL){
		*head = nowy;
	}else{
		lista *last = *head;
		while(last->next!=NULL){
			last=last->next;
		}
		last->next = nowy;
	}
}

void lista_destroy(lista **head){
	lista *pom = *head, *pom2;
	while(pom!=NULL){
		pom2 = pom;
		pom = pom->next;
		delete pom2;
	}
	*head = NULL;
}

int lista_num(lista *head){
	int nr=0;
	lista *pom1 = head;
	while(pom1!=NULL){
		nr++;
		pom1=pom1->next;
	}
	return nr;
}

int get_task_num(lista *e){
	if(e==NULL) return 0;
	if(e->task_code==1) return 2;
	if(e->task_code==2) return 1;
	if(e->task_code==3) return 2;
	if(e->task_code==4) return 2;
	if(e->task_code==5) return 1;
	if(e->task_code==6) return 1;
	return 0;
}

string get_task_name(lista *e){
	if(e==NULL) return 0;
	if(e->task_code==1) return "Brak folderu";
	if(e->task_code==2) return "Brak pliku";
	if(e->task_code==3) return "Inny rozmiar pliku";
	if(e->task_code==4) return "Inna wersja pliku";
	if(e->task_code==5) return "Zb�dny folder";
	if(e->task_code==6) return "Zb�dny plik";
	return "";
}

string* get_tasks(lista *e){
	if(e==NULL) return NULL;
	string *tasks = new string [get_task_num(e)];
	switch(e->task_code){
		case 1:{
			tasks[0]="md \""+e->dir1+"\\"+e->filename+"\"";
			tasks[1]="xcopy \""+e->dir1+"\\"+e->filename+"\\*.*\" \""+e->dir2+"\\"+e->filename+"\\*.*\" /e /c";
		}break;
		case 2:{
			tasks[0]="copy \""+e->dir1+"\\"+e->filename+"\" \""+e->dir2+"\\"+e->filename+"\"";
		}break;
		case 3:{
			tasks[0]="del \""+e->dir2+"\\"+e->filename+"\"";
			tasks[1]="copy \""+e->dir1+"\\"+e->filename+"\" \""+e->dir2+"\\"+e->filename+"\"";
		}break;
		case 4:{
			tasks[0]="del \""+e->dir2+"\\"+e->filename+"\"";
			tasks[1]="copy \""+e->dir1+"\\"+e->filename+"\" \""+e->dir2+"\\"+e->filename+"\"";
		}break;
		case 5:{
			tasks[0]="rd \""+e->dir2+"\\"+e->filename+"\" /s /q";
		}break;
		case 6:{
			tasks[0]="del \""+e->dir2+"\\"+e->filename+"\"";
		}break;
		default:{
			return NULL;
		}
	}
	return tasks;
}

void lista_invert(lista *e){
	string temp = e->dir1;
	e->dir1 = e->dir2;
	e->dir2 = temp;
	if(e->task_code==1){
		e->task_code=5;
	}else if(e->task_code==2){
		e->task_code=6;
	}else if(e->task_code==5){
		e->task_code=1;
	}else if(e->task_code==6){
		e->task_code=2;
	}
}

void App::lista_exec1(lista *e){
	if(e==NULL) return;
	int task_num = get_task_num(e);
	string *tasks = get_tasks(e);
	ss_clear(ss);
	for(int i=0; i<task_num; i++){
		ss<<"Wykonywanie:\r\n"<<tasks[i];
		exec_task(tasks[i]);
	}
}

void App::exec_task(string l){
	ss_clear(ss);
	ss<<"Wykonywanie: "<<l.c_str();
	log(ss.str());
	system(l.c_str());
  history_add_time(&historia,l);
}

void App::lista_exec(lista **head){
	lista *pom = *head;
	int zadania_n=0, zadania_w=0;
	while(pom!=NULL){
		zadania_n+=get_task_num(pom);
		pom=pom->next;
	}
	pom=*head;
	while(pom!=NULL){
		set_progress(double(zadania_w)/zadania_n);
		lista_exec1(pom);
		zadania_w+=get_task_num(pom);
		pom=pom->next;
	}
	set_progress(1);
}
