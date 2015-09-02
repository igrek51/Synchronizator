#ifndef LISTA_H
#define LISTA_H

#include <iostream>
#include <vector>

#define TASK_BRAK_FOLDERU 1
#define TASK_BRAK_PLIKU 2
#define TASK_INNY_ROZMIAR 3
#define TASK_INNA_WERSJA 4
#define TASK_ZBEDNY_FOLDER 5
#define TASK_ZBEDNY_PLIK 6

using namespace std;

class Task {
    Task(string filename, string dir1, string dir2, int code);
    string filename;
    string dir1;
	string dir2;
    int code;
    string name();
    vector<string>* cmds();
    int cmds_number();
    void invert();
    void execute();
};

#endif
