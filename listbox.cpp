#include "app.h"
#include "strings.h"
#include <QTableWidget>
#include <QtWidgets/QHeaderView>

void App::listbox_init(){
    QTableWidget* listbox = dynamic_cast<QTableWidget*>(Controls::geti()->find("listbox"));
    for(int i=0; i<2; i++) listbox->insertColumn(0);
    QHeaderView* header_h = listbox->horizontalHeader();
    QHeaderView* header_v = listbox->verticalHeader();
    header_h->setSectionResizeMode(QHeaderView::ResizeToContents);
    header_h->setVisible(false);
    header_v->setVisible(false);
    header_v->setDefaultSectionSize(23);
    listbox->setSelectionMode(QAbstractItemView::SingleSelection);
    //qheader->resizeSection(0,100);
    //qheader->resizeSection(1,75);
}

int App::listbox_count(){
    QTableWidget* listbox = dynamic_cast<QTableWidget*>(Controls::geti()->find("listbox"));
    return listbox->rowCount();
}

void App::listbox_delete(int nr){
    QTableWidget* listbox = dynamic_cast<QTableWidget*>(Controls::geti()->find("listbox"));
    listbox->removeRow(nr);
}

void App::listbox_clear(){
	while(listbox_count()>0) listbox_delete(0);
}

void App::listbox_add(string s1, string s2){
    QTableWidget* listbox = dynamic_cast<QTableWidget*>(Controls::geti()->find("listbox"));
    listbox->insertRow(listbox->rowCount());
    QTableWidgetItem *item = new QTableWidgetItem(s1.c_str());
    item->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
    listbox->setItem(listbox->rowCount()-1,0,item);
    item = new QTableWidgetItem(s2.c_str());
    item->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    listbox->setItem(listbox->rowCount()-1,1,item);
}

int App::listbox_current(){
    QTableWidget* listbox = dynamic_cast<QTableWidget*>(Controls::geti()->find("listbox"));
    QList <QTableWidgetItem*> zaznaczone = listbox->selectedItems();
    if(zaznaczone.size()==0) return -1;
    return listbox->currentRow();
}

void App::listbox_select(int nr){
    QTableWidget* listbox = dynamic_cast<QTableWidget*>(Controls::geti()->find("listbox"));
    if(nr>=listbox_count()) nr = -1;
    listbox->setCurrentCell(nr, 0);
}

void App::show_lista(){
    listbox_clear();
    for(unsigned int i=0; i<zadania->size(); i++){
        listbox_add(zadania->at(i)->name(), zadania->at(i)->dir2+"/"+zadania->at(i)->filename);
    }
}

void App::listbox_clicked(){
	int n = listbox_current();
	if(n==-1) return;
	if(zadania->size()==0) return;
    stringstream ss;
    Task* task = zadania->at(n);
    vector<string>* cmds = task->cmds();
    ss<<"Zadania dla elementu "<<task->dir2<<"/"<<task->filename<<" ["<<cmds->size()<<"]:\r\n";
    for(unsigned int i=0; i<cmds->size(); i++){
        ss<<cmds->at(i);
        if(i<cmds->size()-1) ss<<"\r\n";
    }
    IO::geti()->echo(ss.str());
}

void App::listbox_click(int, int){
   listbox_clicked();
}
