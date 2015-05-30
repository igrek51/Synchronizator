#include "app.h"
#include <commctrl.h>

void App::set_progress(double p){
	if(p>1) p=1;
	if(p<0) p=0;
	SendMessage(hctrl[9],PBM_SETPOS,(WPARAM)(p*10000),0);
}

void App::synchroman_init(){
	set_progress(0);
	echo("Wyszukiwanie dysku...");
	drive = select_drive();
	if(drive.length()==0) return;
	if(listalista!=NULL) lista_destroy(&listalista);
	//dla ka¿dego synchronizowanego katalogu
	for(int i=0; i<synchro_paths_num; i++){
		ss_clear(ss);
		ss<<"Przeszukiwanie folderu \""<<drive<<synchro_paths_dest[i]<<"\"...";
		echo(ss.str());
		dirlist_cmp(synchro_paths_source[i],drive+synchro_paths_dest[i],&listalista,(synchro_paths_content[i]==1)?true:false,double(i)/synchro_paths_num,double(i+1)/synchro_paths_num);
	}
	set_progress(1);
	ss_clear(ss);
	ss<<"Zakoñczono przeszukiwanie.\r\n";
	if(listalista==NULL){
		ss<<"Brak elementów do synchronizacji.";
	}else{
		ss<<"Znalezione elementy synchronizacji: "<<lista_num(listalista);
	}
	echo(ss.str());
	show_lista();
}

void App::otworz_foldery(){
	int n = listbox_current();
	if(n==-1){
		echo("B³¹d: Nie wybrano elementu.");
		return;
	}
	if(listalista==NULL) return;
	lista *pom = listalista;
	int nr=0;
	while(pom!=NULL){
		if(nr==n){
			ShellExecuteA(NULL,NULL,"explorer.exe",pom->dir1.c_str(),NULL,SW_SHOW);
			ShellExecuteA(NULL,NULL,"explorer.exe",pom->dir2.c_str(),NULL,SW_SHOW);
			return;
		}
		nr++;
		pom=pom->next;
	}
}

void App::otworz_pliki(){
	int n = listbox_current();
	if(n==-1){
		echo("B³¹d: Nie wybrano elementu.");
		return;
	}
	if(listalista==NULL) return;
	lista *pom = listalista;
	int nr=0;
	while(pom!=NULL){
		if(nr==n){
			if(pom->task_code!=3&&pom->task_code!=4){
				echo("Funkcja niedostêpna dla tych elementów.");
				return;
			}
			ShellExecuteA(NULL,"open",(pom->dir1+"\\"+pom->filename).c_str(),NULL,NULL,SW_SHOW);
			ShellExecuteA(NULL,"open",(pom->dir2+"\\"+pom->filename).c_str(),NULL,NULL,SW_SHOW);
			compare_files_out(pom->dir1+"\\"+pom->filename,pom->dir2+"\\"+pom->filename);
			Sleep(300);
			SetForegroundWindow(hwnd);
			return;
		}
		nr++;
		pom=pom->next;
	}
}

void App::pb_usun(){
	if(thread_active){
		echo("B³¹d: Trwa przeszukiwanie.");
		return;
	}
	int cr=listbox_current();
	if(cr==-1){
		echo("B³¹d: Nie wybrano elementu.");
		return;
	}
	if(listalista==NULL) return;
	lista *pom = listalista;
	if(cr==0){
		listalista=listalista->next;
		delete pom;
	}else{
		int nr=1;
		while(pom->next!=NULL){
			if(nr==cr){
				lista *pom2 = pom->next;
				pom->next=pom->next->next;
				delete pom2;
				break;
			}
			nr++;
			pom=pom->next;
		}
	}
	echo("Usuniêto element synchronizacji z listy.");
	show_lista();
}

void App::pb_odwroc(){
	int n = listbox_current();
	if(n==-1){
		echo("B³¹d: Nie wybrano elementu.");
		return;
	}
	if(listalista==NULL) return;
	lista *pom = listalista;
	int nr=0;
	while(pom!=NULL){
		if(nr==n){
			lista_invert(pom);
			echo("Odwrócono kierunek synchronizacji dla wybranego elementu.");
			show_lista();
			listbox_select(n);
			return;
		}
		nr++;
		pom=pom->next;
	}
}

void App::wykonaj_1(){
	if(thread_active){
		echo("B³¹d: Trwa przeszukiwanie.");
		return;
	}
	int cr=listbox_current();
	if(cr==-1){
		echo("B³¹d: Nie wybrano elementu.");
		return;
	}
	if(listalista==NULL) return;
	lista *pom = listalista;
	if(cr==0){
		lista_exec1(pom);
		listalista=listalista->next;
		delete pom;
	}else{
		int nr=1;
		while(pom->next!=NULL){
			if(nr==cr){
				lista_exec1(pom->next);
				lista *pom2 = pom->next;
				pom->next=pom->next->next;
				delete pom2;
				break;
			}
			nr++;
			pom=pom->next;
		}
	}
	echo("Wykonano zadania 1 elementu synchronizacji.");
	show_lista();
	listbox_select(cr);
}

void App::wykonaj_wszystko(){
	if(thread_active){
		echo("B³¹d: Trwa przeszukiwanie.");
		return;
	}
	echo("Wykonywanie operacji...");
	if(listalista==NULL){
		echo("Lista zadañ jest pusta.");
		return;
	}
	lista_exec(&listalista);
	lista_destroy(&listalista);
	show_lista();
	echo("Zakoñczono synchronizacjê.");
}
