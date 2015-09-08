#include "app.h"
#include <QEvent>

void App::event_init(){
    //ustawienia
    Config::geti()->load_config();
    //log
    if(Config::geti()->log_enabled){
        IO::geti()->clear_log();
    }
    if(Config::geti()->history_enabled){
        historia->load();
    }
    //kontrolki
    IO::geti()->log("Tworzenie kontrolek...");
    //groupbox
    Controls::geti()->create_static_center("", "info");
    Controls::geti()->create_groupbox("Lista zadań", "groupbox1");
    Controls::geti()->create_button("Szukaj", "szukaj");
    Controls::geti()->create_button("2 foldery", "2foldery");
    Controls::geti()->create_button("Porównaj", "porownaj_pliki");
    Controls::geti()->create_button("Usuń", "usun");
    Controls::geti()->create_button("Odwróć", "odwroc");
    Controls::geti()->create_button("Wykonaj 1", "wykonaj1");
    Controls::geti()->create_button("Wykonaj", "wykonaj");
    Controls::geti()->create_table("listbox");
    Controls::geti()->create_progressbar("progress_bar");
    //czcionki
    controls_fonts_set();
    //subclassing
    Controls::geti()->subclass("szukaj", this);
    Controls::geti()->subclass("2foldery", this);
    Controls::geti()->subclass("porownaj_pliki", this);
    Controls::geti()->subclass("usun", this);
    Controls::geti()->subclass("odwroc", this);
    Controls::geti()->subclass("wykonaj1", this);
    Controls::geti()->subclass("wykonaj", this);
    Controls::geti()->subclass("listbox", this);
    //ustawiona pozycja, rozmiar okna
    if(Config::geti()->save_wnd_pos==1){
        this->move(Config::geti()->wnd_pos_x, Config::geti()->wnd_pos_y);
    }
    if(Config::geti()->save_wnd_size==1){
        this->resize(Config::geti()->window_w, Config::geti()->window_h);
    }else{
        this->resize(Config::geti()->window_w, Config::geti()->window_h);
    }
    event_resize();
    //progress bar
	set_progress(0);
    //tytuł
    this->setWindowTitle((Config::geti()->program_name+" v"+Config::geti()->version).c_str());
    IO::geti()->echo("Naciśnij przycisk \"Szukaj\", aby wyszukać elementy synchronizacji.");
}

void App::event_button(string name){
    if(name.length()==0) return;
    //zmieniony listbox
    if(name=="listbox"){
        listbox_clicked();
        return;
    }
	//przyciski
	if(name=="szukaj"){ //szukaj
        //filesearch_start();
        synchro_search();
	}else if(name=="2foldery"){ //otwórz foldery
		otworz_foldery();
	}else if(name=="usun"){ //usuń
		pb_usun();
	}else if(name=="wykonaj1"){ //wykonaj 1
		wykonaj_1();
	}else if(name=="wykonaj"){ //wykonaj wszystko
		wykonaj_wszystko();
	}else if(name=="porownaj_pliki"){ //otwórz pliki
		otworz_pliki();
	}else if(name=="odwroc"){ //odwróć
		pb_odwroc();
	}else{
        IO::geti()->error("Zdarzenie nie zostało obsłużone: "+name);
    }
}

void App::event_resize(){
    IO::geti()->log("Resize okna - Odświeżanie układu kontrolek...");
    if(!Controls::geti()->exists("info")){
        IO::geti()->log("Brak kontrolki: info - resize anulowany");
        return;
    }
    int w = this->size().width();
    int h = this->size().height();
	Config::geti()->window_w = w;
    Config::geti()->window_h = h;
    Controls::geti()->resize("info",0,0,w,50);
    Controls::geti()->resize("szukaj",20+(w-40)*0/7,h-50,(w-40)/7,30);
    Controls::geti()->resize("2foldery",20+(w-40)*1/7,h-50,(w-40)/7,30);
    Controls::geti()->resize("porownaj_pliki",20+(w-40)*2/7,h-50,(w-40)/7,30);
    Controls::geti()->resize("usun",20+(w-40)*3/7,h-50,(w-40)/7,30);
    Controls::geti()->resize("odwroc",20+(w-40)*4/7,h-50,(w-40)/7,30);
    Controls::geti()->resize("wykonaj1",20+(w-40)*5/7,h-50,(w-40)/7,30);
    Controls::geti()->resize("wykonaj",20+(w-40)*6/7,h-50,(w-40)/7,30);
    Controls::geti()->resize("progress_bar",10,52,w-20,15);
    Controls::geti()->resize("listbox",20,90,w-40,h-150);
    Controls::geti()->resize("groupbox1",10,70,w-20,h-80);
}

void App::event_move(){
    IO::geti()->log("Przemieszczenie okna");
    Config::geti()->wnd_pos_x = this->pos().x();
    Config::geti()->wnd_pos_y = this->pos().y();
}


void App::resizeEvent(QResizeEvent*){
    event_resize();
}

void App::moveEvent(QMoveEvent*){
    event_move();
}

void App::closeEvent(QCloseEvent*){
    delete App::geti();
}

bool App::eventFilter(QObject* object, QEvent* event){
    string name = Controls::geti()->get_object_name(object);
    //buttony
    if(event->type() == QEvent::MouseButtonPress){
        event_button(name);
        return false;
    }
    return false; //przekazanie dalej
}
