#include "synchro.h"
#include "dir.h"
#include "task.h"
#include "app.h"
#include "files.h"

#include <fstream>

SynchroPath::SynchroPath(string source, string dest, bool content_check){
    this->source = source;
    this->dest = dest;
    this->content_check = content_check;
}

void App::compare_files_out(string file1, string file2){
	fstream plik;
	plik.open(file1.c_str(),fstream::in|fstream::binary);
	if(!plik.good()){
		plik.close();
		IO::geti()->echo("B³¹d: Pierwszy plik nie istnieje");
		return;
	}
	plik.seekg(0,plik.end);
	unsigned int fsize1 = plik.tellg();
	char *plik1 = new char [fsize1];
	plik.seekg(0,plik.beg);
	plik.read(plik1,fsize1);
	plik.close();
	plik.open(file2.c_str(),fstream::in|fstream::binary);
	if(!plik.good()){
		plik.close();
		IO::geti()->echo("B³¹d: Drugi plik nie istnieje");
		return;
	}
	plik.seekg(0,plik.end);
	unsigned int fsize2 = plik.tellg();
	char *plik2 = new char [fsize2];
	plik.seekg(0,plik.beg);
	plik.read(plik2,fsize2);
	plik.close();
	ss_clear(ss);
	if(fsize1!=fsize2){
		ss<<"Pliki o ró¿nych rozmiarach,\r\n";
	}else{
		ss<<"Pliki o równych rozmiarach,\r\n";
	}
	int minsize=(fsize1>fsize2)?fsize2:fsize1;
	bool rowne = true;
	int wiersz = 1;
	int znak_wiersza = 0;
	for(int i=0; i<minsize; i++){
		if(plik1[i]=='\n'){
			wiersz++;
			znak_wiersza = 0;
		}else{
			znak_wiersza++;
		}
		if(plik1[i]!=plik2[i]){
			rowne=false;
			ss<<"Ró¿nica - bajt "<<i<<" (wiersz: "<<wiersz<<", znak: "<<znak_wiersza-1<<")";
			break;
		}
	}
	if(rowne){
		if(fsize1==fsize2){
			ss<<"Brak ró¿nicy zawartoœci plików";
		}else{
			ss<<"Ró¿nica - bajt "<<minsize<<" (ostatni wiersz: "<<wiersz<<")";
		}
	}
	IO::geti()->echo(ss.str());
	delete[] plik1;
	delete[] plik2;
}

string App::select_drive(){
    vector<string>* drives = get_drives();
	int selected = -1;
	if(Config::geti()->synchropaths.size()==0){
		IO::geti()->error("Nie wybrano synchronizowanych folderów.");
		return "";
	}
    stringstream ss;
	//sprawdzenie istnienia folderów na dysku Ÿród³owym
	for(unsigned int i=0; i<Config::geti()->synchropaths.size(); i++){
		if(!dir_exists(Config::geti()->synchropaths.at(i)->source)){
			ss_clear(ss);
			ss<<"Brak katalogu \""<<Config::geti()->synchropaths.at(i)->source<<"\" na dysku Ÿród³owym";
			IO::geti()->error(ss.str());
			return "";
		}
	}
	//szukanie istnienia folderów na dysku docelowym
	for(unsigned int i=0; i<drives->size(); i++){
		bool valid = true;
		for(int j=0; j<Config::geti()->synchropaths.size(); j++){
			if(!dir_exists(drives->at(i) + Config::geti()->synchropaths.at(i)->dest)){
				valid = false;
				break;
			}
		}
		if(valid){
			selected = i;
			break;
		}
	}
	if(selected==-1){
		ss_clear(ss);
		ss<<"Nie znaleziono odpowiedniego dysku.\r\n";
		ss<<"Dostêpne dyski: ";
		ss<<list_drives();
		IO::geti()->error(ss.str());
		return "";
	}else{
		ss_clear(ss);
		ss<<"Wybrany dysk: "<<drives->at(selected);
		IO::geti()->log(ss.str());
		return drives->at(selected);
	}
}

void App::dirlist_cmp(string head1name, string head2name, vector<Task*>* tasks, bool content_check, double prog_from, double prog_to){
	set_progress(prog_from);
    vector<File*>* dir1 = list_dir(head1name);
    vector<File*>* dir2 = list_dir(head2name);
	if(dir1==NULL || dir2==NULL) return;
    for(unsigned int i=0; i<dir1->size(); i++){ //sprawdzanie listy pierwszej - wzorca
        File* pom1 = dir1->at(i);
        File* szuk = file_search(dir2, pom1->name);
		if(pom1->size==-1){ //katalog
			if(szuk==NULL){
                tasks->push_back(new Task(pom1->name,head1name,head2name,TASK_BRAK_FOLDERU));
				show_lista();
			}else{
				if(szuk->size!=-1){ //znalaz³o, lecz jest plikiem
					tasks->push_back(new Task(pom1->name,head1name,head2name,TASK_BRAK_FOLDERU));
					show_lista();
				}else{ //znalaz³o, jest te¿ katalogiem
                    double progress1 = (prog_to-prog_from)*i/dir1->size()+prog_from;
                    double progress2 = (prog_to-prog_from)*(i+1)/dir1->size()+prog_from;
					dirlist_cmp(head1name+"\\"+pom1->name,head2name+"\\"+pom1->name,tasks,content_check,progress1, progress2);
				}
			}
		}else{ //plik
			if(szuk==NULL){
				lista_add(listaa,pom1->name,head1name,head2name,2);
				show_lista();
			}else{
				if(szuk->size!=pom1->size){
					lista_add(listaa,pom1->name,head1name,head2name,3);
					show_lista();
				}else{
					if(content_check){
						if(!files_cmp(head1name+"\\"+pom1->name,head2name+"\\"+pom1->name)){
							lista_add(listaa,pom1->name,head1name,head2name,4);
							show_lista();
						}
					}
				}
			}
		}
		numi++;
		set_progress((prog_to-prog_from)*i/dir1->size()+prog_from);
    }


	dir *pom1 = head1, *szuk;
	while(pom1!=NULL){

	}
	pom1 = head2;
	while(pom1!=NULL){ //sprawdzanie zbednych plikow z drugiej listy
		if(dirlist_search(head1,pom1->name)==NULL){ //jeœli nie znajdzie
			if(pom1->size==-1){
				lista_add(listaa,pom1->name,head1name,head2name,5);
				show_lista();
			}else{
				lista_add(listaa,pom1->name,head1name,head2name,6);
				show_lista();
			}
		}
		pom1=pom1->next;
	}
	dirlist_destroy(&head1);
	dirlist_destroy(&head2);
}
