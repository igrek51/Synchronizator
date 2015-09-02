#include "app.h"

int App::listbox_count(){
	return SendMessage(Controls::geti()->find("listbox"),LB_GETCOUNT,0,0);
}

void App::listbox_delete(int nr){
	SendMessage(Controls::geti()->find("listbox"),LB_DELETESTRING,nr,0);
}

void App::listbox_clear(){
	while(listbox_count()>0) listbox_delete(0);
}

void App::listbox_add(string s){
	SendMessage(Controls::geti()->find("listbox"),LB_ADDSTRING,NULL,(LPARAM)s.c_str());
}

int App::listbox_current(){
	int ret = SendMessage(Controls::geti()->find("listbox"),LB_GETCURSEL,0,0);
	if(ret==LB_ERR) return -1;
	return ret;
}

void App::listbox_select(int nr){
	if(nr>=listbox_count()) nr=-1;
	SendMessage(Controls::geti()->find("listbox"),LB_SETCURSEL,nr,0);
}

void App::show_lista(){
	listbox_clear();
	HDC dcList = GetDC(Controls::geti()->find("listbox"));
	SIZE textSize;
	HFONT hF = (HFONT)SendMessage(Controls::geti()->find("listbox"),WM_GETFONT,0,0);
	HGDIOBJ hOld = SelectObject(dcList,hF);


	if(listalista==NULL) return;
	lista *pom = listalista;
	int nr = 1;
	int max_w = 0;
	while(pom!=NULL){
		ss_clear(ss);
		ss<<get_task_name(pom)<<": "<<pom->dir2<<"\\"<<pom->filename;
		listbox_add(ss.str());
		//d³ugoœæ tekstu w pixelach
		GetTextExtentPoint32(dcList,ss.str().c_str(),ss.str().length(),&textSize);
		if(textSize.cx>max_w) max_w=textSize.cx;
		nr++;
		pom=pom->next;
	}
	SelectObject(dcList,hOld);
	ReleaseDC(Controls::geti()->find("listbox"),dcList);
	SendMessage(Controls::geti()->find("listbox"),LB_SETHORIZONTALEXTENT,max_w,NULL);
}

void App::listbox_clicked(){
	int n = listbox_current();
	if(n==-1) return;
	if(listalista==NULL) return;
	lista *pom = listalista;
	int nr=0;
	while(pom!=NULL){
		if(nr==n){
			ss_clear(ss);
			int task_num = get_task_num(pom);
			ss<<"Zadania dla elementu "<<pom->dir2<<"\\"<<pom->filename<<" ["<<task_num<<"]:\r\n";
			string *tasks = get_tasks(pom);
			for(int i=0; i<task_num; i++){
				ss<<tasks[i];
				if(i<task_num-1) ss<<"\r\n";
			}
			echo(ss.str());
			break;
		}
		nr++;
		pom=pom->next;
	}
}
