#ifndef APP_H
#define APP_H

#include "config.h"
#include "controls.h"
#include "io.h"
#include "history.h"
#include "thread.h"

#include <QMainWindow>
#include <iostream>

using namespace std;

class App : public QMainWindow{
    Q_OBJECT
private:
    static App* instance;
public:
    explicit App(QWidget *parent = 0);
    static App* geti();
	~App();
	//Events
    void event_init();
    void event_button(string name);
	void event_resize();
	void event_move();
    //historia
    History* historia;
    //zadania
    vector<Task*>* zadania;
    void exec_cmd(string l);
    void execute_all_tasks(vector<Task*>* zadania);
	//program
    void controls_fonts_set();
	void set_progress(double p);
	void synchro_search();
	void otworz_foldery();
    void viewer_open(string file);
	void otworz_pliki();
	void pb_usun();
	void pb_odwroc();
	void wykonaj_1();
	void wykonaj_wszystko();
    void buttons_enable(bool enable);
	//listbox
    void listbox_init();
	void show_lista();
	int listbox_count();
	void listbox_delete(int nr);
	void listbox_clear();
    void listbox_add(string s1, string s2);
	int listbox_current();
	void listbox_select(int nr);
	void listbox_clicked();
	//w¹tki
    void filesearch_start();
    //FileSearch* filesearch;

    void resizeEvent(QResizeEvent*);
};

#endif
