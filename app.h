#ifndef APP_H
#define APP_H

#include <windows.h>
#include <iostream>
#include <sstream>

#include "dir.h"
#include "lista.h"
#include "history.h"

using namespace std;

class App{
public:
	App(int w, int h, string version, int ctrls = 0);
	~App();
	HWND *hctrl;
	void setInstance(HINSTANCE *hInst);
	int window_w, window_h;
	string version;
	//io
	string *argv;
	int argc;
	void get_argv(LPSTR lpCmdLine);
	bool is_arg(string par);
	void clear_file(string filename);
	void log(string l);
	void log(int l);
	void echo(string s);
	void echo(int e);
	void echo();
	void text_vcenter();
	bool file_exists(string name);
	void ss_clear(stringstream &sstream);
	stringstream ss;
	void message(string m);
	//config
	void load_config();
	void save_config();
	int save_wnd_pos, save_wnd_size, wnd_pos_x, wnd_pos_y, wnd_w, wnd_h;
	int synchro_paths_num;
	string *synchro_paths_source, *synchro_paths_dest;
	int *synchro_paths_content;
	int log_enabled, history_enabled;
	//WM
	void wm_create(HWND *window);
	void wm_command(WPARAM wParam, LPARAM lParam);
	void wm_resize();
	void wm_move();
	//czcionki
	void change_font(HWND hwdp);
	int fontsize;
	string fontface;
	//dir
	dir* list_dir(string d);
	//lista
	lista *listalista;
	//historia
	history *historia;
	//synchro
	string drive;
	string select_drive();
	void dirlist_cmp(string head1name, string head2name, lista **listaa, bool content_check, double prog_from, double prog_to);
	//program
	void set_progress(double p);
	void synchroman_init();
	void otworz_foldery();
	void otworz_pliki();
	void compare_files_out(string file1, string file2);
	void pb_usun();
	void pb_odwroc();
	void wykonaj_1();
	void wykonaj_wszystko();
	void lista_exec1(lista *e);
	void lista_exec(lista **head);
	void exec_task(string l);
	//listbox
	void show_lista();
	int listbox_count();
	void listbox_delete(int nr);
	void listbox_clear();
	void listbox_add(string s);
	int listbox_current();
	void listbox_select(int nr);
	void listbox_clicked();
	//subclassing
	WNDPROC windowProc, wndproc_new, *wndproc_old;
	LRESULT CALLBACK subclass_wndproc_new(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	void subclass(int ctrl);
	void un_subclass(int ctrl);
	//w¹tki
	void start_thread();
	void exec_thread();
	volatile bool thread_active;
private:
	int ctrls_num;
	HWND hwnd;
	HINSTANCE* hInst;
};

#endif // APP_H
