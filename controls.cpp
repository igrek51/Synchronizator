#include "controls.h"
#include "io.h"
#include "app.h"
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QProgressBar>
#include <QTableWidget>

Controls* Controls::instance = NULL;

Controls* Controls::geti(){
    if(instance == NULL){
        instance = new Controls();
    }
    return instance;
}

Controls::Controls(){
    instance = this;
    central_widget = new QWidget(App::geti());
    layout = new QGridLayout(central_widget);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    central_widget->setLayout(layout);
    layout->setEnabled(false);
    App::geti()->setCentralWidget(central_widget);
}

Controls::~Controls(){
    for(unsigned int i=0; i<controls.size(); i++){
        delete controls.at(i);
    }
    controls.clear();
    delete layout;
}


Control::Control(QWidget* handle, string name){
    this->handle = handle;
    this->name = name;
}

Control::~Control(){
    if(this->handle!=NULL){
        Controls::geti()->layout->removeWidget(this->handle);
    }
    this->handle = NULL;
}


Control* Controls::find_control(string name){
    if(name.length()==0){
        IO::geti()->error("Wyszukiwanie kontrolki o pustej nazwie");
        return NULL;
    }
    for(unsigned int i=0; i<controls.size(); i++){
        if(controls.at(i)->name == name){
            return controls.at(i);
        }
    }
    IO::geti()->error("Nie odnaleziono kontrolki o nazwie: "+name);
    return NULL;
}

QWidget* Controls::find(string name){
    Control* kontrolka = find_control(name);
    if(kontrolka==NULL) return NULL;
    return kontrolka->handle;
}

bool Controls::exists(string name){
    if(name.length()==0) return false;
    for(unsigned int i=0; i<controls.size(); i++){
        if(controls.at(i)->name == name) return true;
    }
    return false;
}

string Controls::get_button_name(int button_nr){
    //jeśli numer jest poprawny
    if(button_nr>=1 && button_nr<=(int)controls.size()){
        return controls.at(button_nr-1)->name;
    }
    stringstream ss;
    ss<<"Nie odnaleziono kontrolki o numerze: "<<button_nr;
    IO::geti()->error(ss.str());
    return "";
}

string Controls::get_object_name(QObject* o){
    for(unsigned int i=0; i<controls.size(); i++){
        if(controls.at(i)->handle == o){
            return controls.at(i)->name;
        }
    }
    IO::geti()->error("Nie odnaleziono obiektu");
    return "";
}


void Controls::add_control(QWidget* handle, string name){
    handle->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    handle->setMinimumHeight(10);
    handle->setMinimumWidth(10);
    layout->addWidget(handle);
    controls.push_back(new Control(handle, name));
}

void Controls::create_button(string text, string name){
    QPushButton* nowy_widget = new QPushButton(text.c_str());
    add_control(nowy_widget, name);
}

void Controls::create_edit(string text, string name){
    QLineEdit* nowy_widget = new QLineEdit(text.c_str());
    add_control(nowy_widget, name);
}

void Controls::create_edit_center(string text, string name){
    QLineEdit* nowy_widget = new QLineEdit(text.c_str());
    add_control(nowy_widget, name);
}

void Controls::create_static(string text, string name){
    QLabel* nowy_widget = new QLabel(text.c_str());
    add_control(nowy_widget, name);
}

void Controls::create_static_center(string text, string name){
    QLabel* nowy_widget = new QLabel(text.c_str());
    add_control(nowy_widget, name);
}

void Controls::create_groupbox(string text, string name){
    QGroupBox* nowy_widget = new QGroupBox(text.c_str());
    add_control(nowy_widget, name);
}

void Controls::create_progressbar(string name){
    QProgressBar* nowy_widget = new QProgressBar();
    add_control(nowy_widget, name);
}

void Controls::create_table(string name){
    QTableWidget* nowy_widget = new QTableWidget();
    add_control(nowy_widget, name);
}


void Controls::set_text(string control_name, string text){
    QWidget* uchwyt = find(control_name);
    if(uchwyt==NULL) return;
    QLabel* label = dynamic_cast<QLabel*>(uchwyt);
    if(label!=NULL){
        label->setText(text.c_str());
        return;
    }
    QLineEdit* lineedit = dynamic_cast<QLineEdit*>(uchwyt);
    if(lineedit!=NULL){
        lineedit->setText(text.c_str());
        return;
    }
    QPushButton* pb = dynamic_cast<QPushButton*>(uchwyt);
    if(pb!=NULL){
        pb->setText(text.c_str());
        return;
    }
    QGroupBox* gb = dynamic_cast<QGroupBox*>(uchwyt);
    if(gb!=NULL){
        gb->setTitle(text.c_str());
        return;
    }
}

void Controls::set_text(string control_name, int number){
    stringstream ss;
    ss<<number;
    set_text(control_name, ss.str());
}

string Controls::get_text(string control_name){
    Control* kontrolka = find_control(control_name);
    if(kontrolka==NULL) return "";
    if(kontrolka->handle==NULL) return "";
    QLabel* label = dynamic_cast<QLabel*>(kontrolka->handle);
    if(label!=NULL){
        return label->text().toStdString();
    }
    QLineEdit* lineedit = dynamic_cast<QLineEdit*>(kontrolka->handle);
    if(lineedit!=NULL){
        return lineedit->text().toStdString();
    }
    QPushButton* pb = dynamic_cast<QPushButton*>(kontrolka->handle);
    if(pb!=NULL){
        return pb->text().toStdString();
    }
    QGroupBox* gb = dynamic_cast<QGroupBox*>(kontrolka->handle);
    if(gb!=NULL){
        return gb->title().toStdString();
    }
    return "";
}

int Controls::get_int(string control_name){
    string content = get_text(control_name);
    if(content.length()==0) return 0;
    return atoi(content.c_str());
}

void Controls::set_focus(string control_name){
    Control* kontrolka = find_control(control_name);
    if(kontrolka==NULL) return;
    if(kontrolka->handle==NULL) return;
    kontrolka->handle->setFocus();
}


void Controls::resize(string control_name, int x, int y, int w, int h){
    Control* kontrolka = find_control(control_name);
    if(kontrolka==NULL) return;
    if(kontrolka->handle==NULL) return;
    if(x!=-1 && y!=-1){
        kontrolka->handle->move(x, y);
    }
    if(w>0 && h>0){
        kontrolka->handle->resize(w, h);
    }
}


void Controls::set_font(QWidget* handle, string fontface, int fontsize){
    if(handle==NULL) return;
    QFont font = handle->font();
    font.setPixelSize(fontsize);
    font.setFamily(fontface.c_str());
    handle->setFont(font);
}

void Controls::set_font(string name, string fontface, int fontsize){
    set_font(find(name), fontface, fontsize);
}


void Controls::subclass(string name, QObject* event_listener){
    Control* kontrolka = find_control(name);
    if(kontrolka==NULL) return;
    if(kontrolka->handle==NULL) return;
    kontrolka->handle->installEventFilter(event_listener);
}
