#include "app.h"
#include <QApplication>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    App::geti()->show();
    App::geti()->event_init();
    return a.exec();
}
