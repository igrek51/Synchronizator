#include "app.h"

LRESULT CALLBACK App::subclass_wndproc_new(HWND hwdp, UINT message, WPARAM wParam, LPARAM lParam){
	//rozpoznanie numeru kontrolki
	int ctrl=-1;
	for(int i=0; i<ctrls_num; i++){
		if(hwdp==hctrl[i]){
			ctrl=i;
			break;
		}
	}
	if(ctrl==-1) return 0;
	//nowe procedury kontrolek
	/*if(ctrl==8){ //listbox
		switch(message){
			case WM_COMMAND:{
			}break;
		}
	}*/
	return CallWindowProc(wndproc_old[ctrl], hwdp, message, wParam, lParam);
}

void App::subclass(int ctrl){
	wndproc_old[ctrl] = (WNDPROC) SetWindowLong(hctrl[ctrl],GWL_WNDPROC,(LONG)wndproc_new);
}

void App::un_subclass(int ctrl){
	if(wndproc_old[ctrl]==NULL) return;
	SetWindowLong(hctrl[ctrl],GWL_WNDPROC,(LONG)wndproc_old[ctrl]);
}

