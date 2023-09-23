#include "resource.h"
#include "string"
#include <Windows.h>


#define WINDOW_NAME L"MESSANGER V0.0.1"

BOOL CALLBACK Nmessager(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK ServerSet(HWND, UINT, WPARAM, LPARAM);

TCHAR* passwd, * login, * addr;

void GetServerData(HWND hwnd){
	auto hpass = GetDlgItem(hwnd, PASSWD);
	auto pass_len = GetWindowTextLengthA(hpass);
	auto hlogn = GetDlgItem(hwnd, LOGIN);
	auto login_len = GetWindowTextLengthA(hlogn);
	auto haddr = GetDlgItem(hwnd, IP_ADDR);
	auto addr_len = GetWindowTextLengthA(haddr);

	passwd = new TCHAR[pass_len];
	login = new TCHAR[login_len];
	addr = new TCHAR[addr_len];

	GetWindowText(hpass, passwd, pass_len+1);
	GetWindowText(hlogn, login, login_len+1);
	GetWindowText(haddr, addr, addr_len+1);
}

void close(HWND hwnd){
	EndDialog(hwnd, 0);
}

HINSTANCE hinst;

int CALLBACK WinMain(HINSTANCE hinstance, HINSTANCE hPrevInst, LPSTR cmd, int cmdshow){
	DialogBox(hinstance, MAKEINTRESOURCE(MAIN_WINDOW), 0, (DLGPROC)Nmessager);

	return 0;
}


BOOL CALLBACK ServerSet(HWND hwnd, UINT uint, WPARAM wparam, LPARAM lparam){
	switch(uint){
		case WM_INITDIALOG:{

		} break;

		case WM_COMMAND:
		{
			switch(LOWORD(wparam))
			{
			case SERVER_AUTHOR:
			{
				GetServerData(hwnd);	
				
			}

			case IDCANCEL:
			{
				close(hwnd);
				return TRUE;
			}
			}
		} break;

	}
	return FALSE;
}


BOOL CALLBACK Nmessager(HWND hwnd, UINT uint, WPARAM wparam, LPARAM lparam){
	switch(uint){
	case WM_INITDIALOG:{
		hinst = GetModuleHandle(NULL);

	}
	break;

	case WM_COMMAND:{
		switch(LOWORD(wparam)){
			case SERVER_SET:{
				DialogBoxParam(hinst, MAKEINTRESOURCE(WIN_SERVER_SET), 0, (DLGPROC)ServerSet, 0);
			}
			break;
		}
	}
	break;

	case WM_CLOSE:
	{
		close(hwnd);
		return TRUE;
	}
	}
	return FALSE;
}