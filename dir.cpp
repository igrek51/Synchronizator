#include "dir.h"
#include "app.h"

bool dir_exists(const string& dirName_in){
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if(ftyp==INVALID_FILE_ATTRIBUTES) return false;
	if(ftyp&FILE_ATTRIBUTE_DIRECTORY) return true;
	return false;
}

void dirlist_add(dir **head, string name, int size){
	dir *nowy = new dir;
	nowy->name = name;
	nowy->size = size;
	nowy->next = NULL;
	if(*head==NULL){
		*head = nowy;
	}else{
		dir *last = *head;
		while(last->next!=NULL){
			last=last->next;
		}
		last->next = nowy;
	}
}

void dirlist_destroy(dir **head){
	dir *pom = *head, *pom2;
	while(pom!=NULL){
		pom2 = pom;
		pom = pom->next;
		delete pom2;
	}
	*head = NULL;
}

int dirlist_num(dir *head){
	int nr=0;
	dir *pom1 = head;
	while(pom1!=NULL){
		nr++;
		pom1=pom1->next;
	}
	return nr;
}

char to_lower(char c){
	if(c>='A'&&c<='Z') return c+32;
	return c;
}

bool str_cmp_lc(string s1, string s2){
	if(s1.length()!=s1.length()) return false;
	for(unsigned int i=0; i<s1.length(); i++){
		if(to_lower(s1[i])!=to_lower(s2[i])) return false;
	}
	return true;
}

dir* dirlist_search(dir *head, string name){
	dir *pom1 = head;
	while(pom1!=NULL){
		if(str_cmp_lc(pom1->name,name)) return pom1;
		pom1=pom1->next;
	}
	return NULL;
}

dir* App::list_dir(string d){
	if(!dir_exists(d)){
		ss_clear(ss);
		ss<<"B³¹d: Folder \""<<d<<"\" nie istnieje";
		echo(ss.str());
		return NULL;
	}
	WIN32_FIND_DATAA ffd;
	HANDLE hFind = FindFirstFileA((d+"\\*").c_str(),&ffd);
	if(hFind==INVALID_HANDLE_VALUE){
		echo("B³¹d: 201");
		return NULL;
	}
	dir *head = NULL;
	do{
		const char *stemp = string(ffd.cFileName).c_str();
		if(strcmp(stemp,".")==0) continue;
		if(strcmp(stemp,"..")==0) continue;
		if(strcmp(stemp,"desktop.ini")==0) continue;
		if(ffd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY){
			dirlist_add(&head,ffd.cFileName,-1); //katalog
		}else{
			dirlist_add(&head,ffd.cFileName,ffd.nFileSizeLow); //plik
		}
	}while(FindNextFileA(hFind,&ffd)!=0);
	FindClose(hFind);
	return head;
}
