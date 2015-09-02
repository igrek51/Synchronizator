#include "app.h"
#include <commctrl.h>

void App::event_init(HWND *window){
    main_window = *window;
    //ustawienia
    Config::geti()->load_config();
    //log
    if(Config::geti()->log_enabled){
        IO::geti()->clear_log();
        IO::geti()->log("Hello World...");
    }
    historia->load();
    //kontrolki
    IO::geti()->log("Tworzenie kontrolek...");
    //groupbox
    HWND handle = CreateWindowEx(0,WC_BUTTON,"Lista zadañ",WS_CHILD|WS_VISIBLE|BS_GROUPBOX,0,0,0,0,main_window,(HMENU)0,*hInst,0);
    Controls::geti()->controls.push_back(new Control(handle, "groupbox1"));
    Controls::geti()->create_button("Szukaj", 0, 0, 0, 0, "szukaj");
    Controls::geti()->create_button("2 foldery", 0, 0, 0, 0, "2foldery");
    Controls::geti()->create_button("Porównaj", 0, 0, 0, 0, "porownaj_pliki");
    Controls::geti()->create_button("Usuñ", 0, 0, 0, 0, "usun");
    Controls::geti()->create_button("Odwróæ", 0, 0, 0, 0, "odwroc");
    Controls::geti()->create_button("Wykonaj 1", 0, 0, 0, 0, "wykonaj1");
    Controls::geti()->create_button("Wykonaj", 0, 0, 0, 0, "wykonaj");
    //multiline centered static
    handle = CreateWindowEx(0,WC_EDIT,"",WS_CHILD|WS_VISIBLE|ES_CENTER|ES_MULTILINE|ES_READONLY,0,0,0,0,main_window,0,*hInst,0);
    Controls::geti()->controls.push_back(new Control(handle, "info"));
    //listbox
    handle = CreateWindowEx(WS_EX_CLIENTEDGE,WC_LISTBOX,"",WS_CHILD|WS_VISIBLE|LBS_NOINTEGRALHEIGHT|WS_HSCROLL|WS_VSCROLL|LBS_NOTIFY,0,0,0,0,main_window,0,*hInst,0);
    Controls::geti()->controls.push_back(new Control(handle, "listbox"));
    //progress bar
    handle = CreateWindowEx(0,PROGRESS_CLASS,NULL,WS_CHILD|WS_VISIBLE,0,0,0,0,main_window,(HMENU)200,*hInst,0);
    Controls::geti()->controls.push_back(new Control(handle, "progress_bar"));
    //czcionki
    controls_fonts_set();
    //subclassing
    IO::geti()->log("Subclassing...");
    for(unsigned int i=0; i<Controls::geti()->controls.size(); i++){
        subclass(Controls::geti()->controls.at(i));
    }
    event_resize();
    //progress bar
	SendMessage(Controls::geti()->find("progress_bar"),PBM_SETRANGE,0,(LPARAM)MAKELONG(0,10000));
	set_progress(0);
    //tytu³
    ss_clear(ss);
	ss<<"Igrek Synchronizator v"<<version;
	SetWindowText(main_window, ss.str().c_str());
    IO::geti()->echo("Naciœnij przycisk \"Szukaj\", aby wyszukaæ elementy synchronizacji.");
}

void App::event_button(WPARAM wParam, LPARAM lParam){
    //zmieniony listbox
	if(lParam==(LPARAM)Controls::geti()->find("listbox")&&HIWORD(wParam)==LBN_SELCHANGE){
		//listbox_clicked();
        return;
	}
    string name = Controls::geti()->get_button_name(wParam);
    if(name.length()==0) return;
    /*
	//przyciski
	if(wParam==1){ //szukaj
		start_thread();
	}
	if(wParam==2){ //otwórz foldery
		otworz_foldery();
	}
	if(wParam==3){ //usuñ
		pb_usun();
	}
	if(wParam==4){ //wykonaj 1
		wykonaj_1();
	}
	if(wParam==5){ //wykonaj wszystko
		wykonaj_wszystko();
	}
	if(wParam==6){ //otwórz pliki
		otworz_pliki();
	}
	if(wParam==7){ //odwróæ
		pb_odwroc();
	}
    */
}

void App::event_resize(){
    IO::geti()->log("Resize okna - Odœwie¿anie uk³adu kontrolek...");
    if(!Controls::geti()->exists("info")) return;
    RECT wnd_rect;
	GetClientRect(main_window, &wnd_rect);
    int w = wnd_rect.right-wnd_rect.left;
	int h = wnd_rect.bottom-wnd_rect.top;
	Config::geti()->window_w = w;
    Config::geti()->window_h = h;
	text_vcenter();
	SetWindowPos(hctrl[9],HWND_TOP,0,0,window_w-20,15,SWP_NOMOVE);
	SetWindowPos(hctrl[10],HWND_TOP,0,0,window_w-20,window_h-80,SWP_NOMOVE);
	SetWindowPos(hctrl[8],HWND_TOP,0,0,window_w-40,window_h-150,SWP_NOMOVE);
	for(int i=0; i<7; i++){
		SetWindowPos(hctrl[i+1],HWND_TOP,20+(window_w-40)*i/7,window_h-50,(window_w-40)/7,30,0);
	}
    /*
    IO::geti()->log("Resize okna - Odœwie¿anie uk³adu kontrolek...");
    if(!Controls::geti()->exists("cmd_output1")) return;
    if(!Controls::geti()->exists("editor")) return;
    int ch = Config::geti()->control_height;
	RECT wnd_rect;
	GetClientRect(main_window, &wnd_rect);
	int w = wnd_rect.right-wnd_rect.left;
	int h = wnd_rect.bottom-wnd_rect.top;
    Config::geti()->window_w = w;
    Config::geti()->window_h = h;
    //rozmieszczenie kontrolek
    int editor_h = h-ch;
    int editor_y = 0;
    if(Config::geti()->cmd_show){
        editor_h -= ch*Config::geti()->cmd_outputs_num;
    }

	if(Config::geti()->toolbar_show){
        editor_h -= ch*3;
        editor_y += ch*3;
    }
    Controls::i()->resize("editor", 0,editor_y,w,editor_h);

    Controls::i()->resize("filename", 0,0,Config::geti()->static_filename_width,ch);
    Controls::i()->resize("filename_edit", Config::geti()->static_filename_width,0,w-Config::geti()->static_filename_width,ch);
    //1. rz¹d
    Controls::i()->resize("new", w*0/7,ch*1,w/7,ch);
    Controls::i()->resize("load", w*1/7,ch*1,w/7,ch);
    Controls::i()->resize("save", w*2/7,ch*1,w/7,ch);
    Controls::i()->resize("base", w*3/7,ch*1,w/7,ch);
    Controls::i()->resize("autoscroll_interval", w*4/7,ch*1,w/7,ch);
    Controls::i()->resize("autoscroll_wait", w*5/7,ch*1,w/7,ch);
    Controls::i()->resize("autoscroll", w*6/7,ch*1,w/7,ch);
	//2. rz¹d
    Controls::i()->resize("find_edit", w*0/7,ch*2,w*2/7,ch);
    Controls::i()->resize("replace_edit", w*2/7,ch*2,w*2/7,ch);
    Controls::i()->resize("find", w*4/7,ch*2,w/7,ch);
    Controls::i()->resize("replace", w*5/7,ch*2,w/7,ch);
    Controls::i()->resize("analyze", w*6/7,ch*2,w/7,ch);
    //konsola
    if(Config::geti()->cmd_show){
        for(int i=0; i<Config::geti()->cmd_outputs_num; i++){
            stringstream ss;
            ss<<"cmd_output"<<i+1;
            Controls::i()->resize(ss.str(), Config::geti()->cmd_outputs_space,h-ch*(i+2),w-Config::geti()->cmd_outputs_space,ch);
        }
        Controls::i()->resize("cmd", 0,h-ch,w,ch);
    }else{
        Controls::i()->resize("cmd_output1", 0,h-ch,w,ch);
    }
    */
}

void App::event_move(){
	RECT wnd_rect;
	GetWindowRect(hwnd, &wnd_rect);
	wnd_pos_x = wnd_rect.left;
	wnd_pos_y = wnd_rect.top;
	wnd_w = wnd_rect.right-wnd_rect.left;
	wnd_h = wnd_rect.bottom-wnd_rect.top;
}

