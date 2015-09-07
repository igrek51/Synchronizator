#ifndef CONTROLS_H
#define CONTROLS_H

#include <iostream>
#include <vector>
#include <QWidget>
#include <QVBoxLayout>

using namespace std;

class Control {
public:
    Control(QWidget* handle, string name);
    ~Control();
    QWidget* handle;
    string name;
};

class Controls {
private:
	static Controls* instance;
    Controls();
public:
    static Controls* geti();
    ~Controls();
    vector<Control*> controls;
    QVBoxLayout* layout;
    QWidget* central_widget;
    //  Wyszukiwanie kontrolek
    Control* find_control(string name);
    QWidget* find(string name);
    bool exists(string name);
    string get_button_name(int button_nr);
    //  Tworzenie nowych kontrolek
    void create_button(string text, string name = "");
    void create_button_multiline(string text, string name = "");
    void create_edit(string text, string name = "");
    void create_edit_center(string text, string name = "");
    void create_static(string text, string name = "");
    void create_static_center(string text, string name = "");
    void create_groupbox(string text, string name = "");
    void create_progressbar(string name);
    void create_table(string name);
    //  Zawartoœæ
    void set_text(string control_name, string text = "");
    void set_text(string control_name, int number);
    string get_text(string control_name);
    int get_int(string control_name);
    void set_focus(string control_name);
    //  Zmiana rozmiaru
    void resize(string control_name, int x, int y, int w=-1, int h=-1);
    //  Zmiana czcionki
    void set_font(QWidget* handle, string fontface, int fontsize);
    void set_font(string name, string fontface, int fontsize);
};

#endif
