#include "app.h"
#include <commctrl.h>

void App::wm_create(HWND *window){
	hwnd = *window;
	//ustawienia
	load_config();
	//log
	if(log_enabled==1) clear_file("log.txt");
	log("Hello World...");
	//kontrolki
	hctrl[0]=CreateWindowEx(0,"Edit","",WS_CHILD|WS_VISIBLE|ES_CENTER|ES_MULTILINE|ES_READONLY,0,0,window_w,50,hwnd,0,*hInst,0); //info
	hctrl[1]=CreateWindowEx(0,"Button","Szukaj",WS_CHILD|WS_VISIBLE,20,350,80,30,hwnd,(HMENU)1,*hInst,0);
	hctrl[2]=CreateWindowEx(0,"Button","2 foldery",WS_CHILD|WS_VISIBLE,100,350,80,30,hwnd,(HMENU)2,*hInst,0);
	hctrl[3]=CreateWindowEx(0,"Button","Porównaj pliki",WS_CHILD|WS_VISIBLE,100,350,80,30,hwnd,(HMENU)6,*hInst,0);
	hctrl[4]=CreateWindowEx(0,"Button","Usuñ",WS_CHILD|WS_VISIBLE,180,350,80,30,hwnd,(HMENU)3,*hInst,0);
	hctrl[5]=CreateWindowEx(0,"Button","Odwróæ",WS_CHILD|WS_VISIBLE,180,350,80,30,hwnd,(HMENU)7,*hInst,0);
	hctrl[6]=CreateWindowEx(0,"Button","Wykonaj 1",WS_CHILD|WS_VISIBLE,260,350,80,30,hwnd,(HMENU)4,*hInst,0);
	hctrl[7]=CreateWindowEx(0,"Button","Wykonaj",WS_CHILD|WS_VISIBLE,340,350,80,30,hwnd,(HMENU)5,*hInst,0);
	hctrl[9]=CreateWindowEx(0,PROGRESS_CLASS,NULL,WS_CHILD|WS_VISIBLE,10,52,window_w-20,15,hwnd,(HMENU)200,*hInst,0); //progress bar
	hctrl[10]=CreateWindowEx(0,"Button","Lista zadañ",WS_CHILD|WS_VISIBLE|BS_GROUPBOX,10,70,window_w-20,window_h-80,hwnd,(HMENU)0,*hInst,0); //groupbox
	hctrl[8]=CreateWindowEx(WS_EX_CLIENTEDGE,"LISTBOX","",WS_CHILD|WS_VISIBLE|LBS_NOINTEGRALHEIGHT|WS_HSCROLL|WS_VSCROLL|LBS_NOTIFY,20,90,window_w-40,window_h-150,hwnd,0,*hInst,0); //lista
	//ustawienia
	load_config();
	listalista=NULL;
	history_load(&historia);
	thread_active=false;
	//ustawiona pozycja, rozmiar okna
	if(save_wnd_pos==1&&save_wnd_size==1) SetWindowPos(hwnd,HWND_TOP,wnd_pos_x,wnd_pos_y,wnd_w,wnd_h,0);
	else if(save_wnd_pos==1) SetWindowPos(hwnd,HWND_TOP,wnd_pos_x,wnd_pos_y,0,0,SWP_NOSIZE);
	else if(save_wnd_size==1) SetWindowPos(hwnd,HWND_TOP,0,0,wnd_w,wnd_h,SWP_NOMOVE);
	//czcionki
	fontsize=13;
	//fontface="Calibri";
	fontface="MS Shell Dlg 2";
	for(int i=0; i<ctrls_num; i++){
		change_font(hctrl[i]);
	}
	//progress bar
	SendMessage(hctrl[9],PBM_SETRANGE,0,(LPARAM)MAKELONG(0,10000));
	set_progress(0);
	//tytu³
	ss_clear(ss);
	ss<<"Igrek Synchronizator v"<<version;
	SetWindowText(hwnd,ss.str().c_str());
	log("wersja "+version);
	echo("Naciœnij przycisk \"Szukaj\", aby wyszukaæ elementy synchronizacji.");
}

void App::wm_command(WPARAM wParam, LPARAM lParam){
	//zmieniony listbox
	if(lParam==(LPARAM)hctrl[8]&&HIWORD(wParam)==LBN_SELCHANGE){
		listbox_clicked();
	}
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
}

void App::wm_resize(){
	RECT wnd_rect;
	GetClientRect(hwnd, &wnd_rect);
	window_w = wnd_rect.right-wnd_rect.left;
	window_h = wnd_rect.bottom-wnd_rect.top;
	GetWindowRect(hwnd, &wnd_rect);
	wnd_w = wnd_rect.right-wnd_rect.left;
	wnd_h = wnd_rect.bottom-wnd_rect.top;
	text_vcenter();
	SetWindowPos(hctrl[9],HWND_TOP,0,0,window_w-20,15,SWP_NOMOVE);
	SetWindowPos(hctrl[10],HWND_TOP,0,0,window_w-20,window_h-80,SWP_NOMOVE);
	SetWindowPos(hctrl[8],HWND_TOP,0,0,window_w-40,window_h-150,SWP_NOMOVE);
	for(int i=0; i<7; i++){
		SetWindowPos(hctrl[i+1],HWND_TOP,20+(window_w-40)*i/7,window_h-50,(window_w-40)/7,30,0);
	}
}

void App::wm_move(){
	RECT wnd_rect;
	GetWindowRect(hwnd, &wnd_rect);
	wnd_pos_x = wnd_rect.left;
	wnd_pos_y = wnd_rect.top;
	wnd_w = wnd_rect.right-wnd_rect.left;
	wnd_h = wnd_rect.bottom-wnd_rect.top;
}

void App::change_font(HWND hwdp){
	if(hwdp==NULL) return;
	HFONT hFont=CreateFont(fontsize,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,0,0,0,0,fontface.c_str());
	SendMessage(hwdp,WM_SETFONT,(WPARAM)hFont,true);
}
