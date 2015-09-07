#include "app.h"

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
    Controls::geti()->create_groupbox("Lista zadañ", "groupbox1");
    Controls::geti()->create_button("Szukaj", "szukaj");
    Controls::geti()->create_button("2 foldery", "2foldery");
    Controls::geti()->create_button("Porównaj", "porownaj_pliki");
    Controls::geti()->create_button("Usuñ", "usun");
    Controls::geti()->create_button("Odwróæ", "odwroc");
    Controls::geti()->create_button("Wykonaj 1", "wykonaj1");
    Controls::geti()->create_button("Wykonaj", "wykonaj");
    Controls::geti()->create_static_center("", "info");
    Controls::geti()->create_table("listbox");
    Controls::geti()->create_progressbar("progress_bar");
    //czcionki
    controls_fonts_set();
    //subclassing
    /*
    ui->edit_wait->installEventFilter(this);
    ui->edit_interval->installEventFilter(this);
    ui->edit_search->installEventFilter(this);
    ui->edit_replace->installEventFilter(this);
    ui->edit_cmd->installEventFilter(this);
    ui->textedit->installEventFilter(this);
    this->installEventFilter(this);
    */
    //ustawiona pozycja, rozmiar okna
    if(Config::geti()->save_wnd_pos==1){
        this->move(Config::geti()->wnd_pos_x, Config::geti()->wnd_pos_y);
    }
    if(Config::geti()->save_wnd_size==1){
        this->resize(Config::geti()->window_w, Config::geti()->window_h);
    }
    event_resize();
    //progress bar
	set_progress(0);
    //tytu³
    this->setWindowTitle((Config::geti()->program_name+" v"+Config::geti()->version).c_str());
    IO::geti()->echo("Naciœnij przycisk \"Szukaj\", aby wyszukaæ elementy synchronizacji.");
}

void App::event_button(string name){
    /*
    //zmieniony listbox
	if(lParam==(LPARAM)Controls::geti()->find("listbox")&&HIWORD(wParam)==LBN_SELCHANGE){
		//listbox_clicked();
        return;
	}
    string name = "";
    if(wParam>=1 && wParam<=Controls::geti()->controls.size()){
        name = Controls::geti()->get_button_name(wParam);
    }
    if(name.length()==0) return;
	//przyciski
	if(name=="szukaj"){ //szukaj
        //filesearch_start();
        synchro_search();
	}else if(name=="2foldery"){ //otwórz foldery
		otworz_foldery();
	}else if(name=="usun"){ //usuñ
		pb_usun();
	}else if(name=="wykonaj1"){ //wykonaj 1
		wykonaj_1();
	}else if(name=="wykonaj"){ //wykonaj wszystko
		wykonaj_wszystko();
	}else if(name=="porownaj_pliki"){ //otwórz pliki
		otworz_pliki();
	}else if(name=="odwroc"){ //odwróæ
		pb_odwroc();
	}else{
        IO::geti()->error("Zdarzenie nie zosta³o obs³u¿one: "+name);
    }
    */
}

void App::event_resize(){
    /*
    int w = this->size().width(); //560
    int h = this->size().height(); //400
    ui->info_txt->resize(w,60);
    ui->progress1->resize(w-20,15);
    ui->groupBox1->resize(w-20,h-90);
    ui->list1->resize(w-40,h-175);

    ui->pb_szukaj->resize((w-40)/4,60);
    ui->pb_porownaj->resize((w-40)/4,30);
    ui->pb_odwroc->resize((w-40)/4,30);
    ui->pb_wykonaj1->resize((w-40)/4,30);
    ui->pb_otworz->resize((w-40)/4,30);
    ui->pb_usun->resize((w-40)/4,30);
    ui->pb_wykonaj->resize((w-40)/4,30);

    ui->pb_szukaj->move(10,h-150);
    ui->pb_porownaj->move(10+(w-40)/4,h-150);
    ui->pb_odwroc->move(10+(w-40)*2/4,h-150);
    ui->pb_wykonaj1->move(10+(w-40)*3/4,h-150);
    ui->pb_otworz->move(10+(w-40)/4,h-120);
    ui->pb_usun->move(10+(w-40)*2/4,h-120);
    ui->pb_wykonaj->move(10+(w-40)*3/4,h-120);
    */
    IO::geti()->log("Resize okna - Odœwie¿anie uk³adu kontrolek...");
    if(!Controls::geti()->exists("info")) return;
    /*
    RECT wnd_rect;
	GetClientRect(main_window, &wnd_rect);
    int w = wnd_rect.right-wnd_rect.left;
	int h = wnd_rect.bottom-wnd_rect.top;
	Config::geti()->window_w = w;
    Config::geti()->window_h = h;
	text_vcenter();
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
    */
}

void App::event_move(){
    /*
	RECT wnd_rect;
	GetWindowRect(main_window, &wnd_rect);
	Config::geti()->wnd_pos_x = wnd_rect.left;
	Config::geti()->wnd_pos_y = wnd_rect.top;
	Config::geti()->window_w = wnd_rect.right-wnd_rect.left;
	Config::geti()->window_h = wnd_rect.bottom-wnd_rect.top;
    */
}

