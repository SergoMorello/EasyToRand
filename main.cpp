#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <string>
#include <ctime>
#include <Wincrypt.h>
#include <openssl/md5.h>

using namespace std;

bool bfrWrite(string source)
{	
	if(OpenClipboard(NULL))
	{
	   HGLOBAL hgBuffer;
	   char* chBuffer;
	   EmptyClipboard();
	   hgBuffer= GlobalAlloc(GMEM_DDESHARE, source.size()+1);
	   chBuffer= (char*)GlobalLock(hgBuffer);
	   strcpy(chBuffer, LPCSTR(source.c_str()));
	   GlobalUnlock(hgBuffer);
	   SetClipboardData(CF_TEXT, hgBuffer);
	   CloseClipboard();
	   return true;
	}
	return false;
}

string randP(int size, int type)
{
	srand(time(0));
	string buffer;
	for(int i=0;i<size;i++)
	{	
		int getRand;
		int bigChars = 65 + rand() % (90-65);
		int littleChars = 97 + rand() % (122-97);
		int specChars = 33 + rand() % (126-33);
		int nums = 48 + rand() % (57-48);
		int num_SpecChars = 0,num_bigChars = 0,num_littleChars = 0,num_nums = 0;
		
		while(true)
		{
			int  typeRand = rand() % 5;
			
			if (typeRand==1 && type>=1)
			{
				getRand = littleChars;
				++num_littleChars;
			}
			else
			if (typeRand==2 && type>=1)
			{
				getRand = bigChars;
				++num_bigChars;
			}
			else
			if (typeRand==3 && type==2)
			{
				
				getRand = specChars;
				++num_SpecChars;
			}
			else
			if (typeRand==4 && type>=1)
			{
				getRand = nums;
				++num_nums;
			}
			else
			{
				getRand = littleChars;
				++num_littleChars;
			}
			if (type==2 && num_SpecChars<2) {continue;}
			break;
		}
			

		char ch(getRand);
		buffer += ch;
	}
	return buffer;
}

string encode_decode_xor(string data, string key, int type) 
{
	string result;
	if (type==1)
	{
		int keyCode = 0;
		for (int x=0; x<key.size(); x++) {
			keyCode += key[x];
		}
		for (int i=0; i<data.size(); i++) {
			result += data[i] ^ (keyCode + 1) % 20;
		}
	}
	if (type==2)
	{
		for (int i = 0; i < data.size(); i++)
		{
			data[i] ^= key[i % key.size()];
		}
		int keyCode = 0;
		for (int x=0; x<data.size(); x++) {
			result += to_string((int)data[x]);
		}
	}
	return result;
}

string md5(string data)
{
	unsigned char digest[MD5_DIGEST_LENGTH];  
    MD5((unsigned char*)data.c_str(), data.size(), (unsigned char*)&digest);    
 
    char mdString[33];
 
    for(int i = 0; i < 16; i++)
         sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
	
	return string(mdString);
}



void SetStyle(HWND obj,string font,int size, int type)
{
	int b, i, u, s;
	type==1 ? b=700 : b=0;
	type==2 ? i=1 : i=0;
	type==3 ? u=1 : u=0;
	type==4 ? s=1 : s=0;
	HFONT hFont = CreateFont( size, 0, 0, 0, b, i, u, s, DEFAULT_CHARSET, 0, 0, 0, 0, font.c_str() );
	SendMessage(obj,WM_SETFONT,(LPARAM)hFont,TRUE);
}

void SetText(string text,int type, int x, int y, int width, int height, HWND hmwnd, string font,int size)
{
	int b, i, u, s;
	type==1 ? b=700 : b=0;
	type==2 ? i=1 : i=0;
	type==3 ? u=1 : u=0;
	type==4 ? s=1 : s=0;

	HFONT hFont = CreateFont( size, 0, 0, 0, b, i, u, s, DEFAULT_CHARSET, 0, 0, 0, 0, font.c_str() );
	SendMessage(CreateWindow("Static", text.c_str(), WS_CHILD|WS_VISIBLE, x, y, width, height, hmwnd, 0, 0, NULL),WM_SETFONT,(LPARAM)hFont,TRUE);
}

void openLink(string link)
{
	ShellExecute(NULL, "open", link.c_str(), NULL, NULL, SW_SHOW);
}
 
#define ID_BUTTON1 1
#define ID_EDIT1 2
#define ID_CHECKBOX1 3
#define ID_BTN_CRYPT 4
#define ID_EDIT_CRPT1 5
#define ID_EDIT_CRPT2 6
#define ID_TEXTBOX 7
#define ID_CHBX_AUTOC 8
#define ID_BTN_MD5 9
#define ID_EDIT_MD5 10
#define ID_MY_LINK 11
#define ID_CHECKBOX2 12

#define PROGRAM_NAME "Easy to rand()"
#define GLOBAL_FONT_SIZE 15
HINSTANCE hInstance;
HWND hGroupBox;
HWND hGroupBox2;
HWND hGroupBox3;

LRESULT CALLBACK GroupBoxProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
WNDPROC OldGroupBoxProc;

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {

    switch(Message) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_CREATE:			
            break;
    
	case WM_NOTIFY:
	{
	  NMHDR* pLinkCtr = (NMHDR*)lParam;
	  if (pLinkCtr)
	  {
		 switch(pLinkCtr->code)
		 {
		   case NM_CLICK:
		   {
			  if (LOWORD(wParam) == ID_MY_LINK)
			  {
				NMLINK* pNMLink = (NMLINK*)lParam;
				LITEM iItem = pNMLink->item;

				wstring ws(iItem.szUrl); 
				openLink(string(ws.begin(), ws.end()));
				return (INT_PTR)TRUE;
			  }
			  return 0;
			}
		  }
	  }
	}
			
    
    case WM_COMMAND:
        {
            switch (LOWORD(wParam))
			{
				
                case ID_BUTTON1:
					{
						int typePass = 1;
						if (IsDlgButtonChecked( hGroupBox, ID_CHECKBOX1)==1)
						{
							typePass = 2;
						}
						char editBuf[4];
						GetDlgItemText (hGroupBox, ID_EDIT1, editBuf, 4);
						int editBufINT = atoi(editBuf);
						if (editBufINT<6)
						{
							MessageBox(hwnd, "Min value = 6", PROGRAM_NAME, MB_ICONINFORMATION);
							return 0;
						}
						string newRandPass = randP(editBufINT,typePass);
						SetDlgItemText(hwnd, ID_TEXTBOX, newRandPass.c_str());
						if (IsDlgButtonChecked( hwnd, ID_CHBX_AUTOC)==1)
						{
							bfrWrite(newRandPass);
						}
                        return 0;
					}
					case ID_BTN_CRYPT:
					{
						int typeCrypt = 1;
						if (IsDlgButtonChecked( hGroupBox2, ID_CHECKBOX2)==1)
						{
							typeCrypt = 2;
						}
						char editBufKey[1024];
						char editBufData[1024];
						GetDlgItemText (hGroupBox2, ID_EDIT_CRPT1, editBufData, 1024);	
						GetDlgItemText (hGroupBox2, ID_EDIT_CRPT2, editBufKey, 1024);
						string editBufKeySTR = string(editBufKey);
						string editBufDataSTR = string(editBufData);
						if (editBufKeySTR=="" || editBufDataSTR=="")
						{
							MessageBox(hwnd, "Data or key empty", PROGRAM_NAME, MB_ICONINFORMATION);
							return 0;
						}
						
						string cryptRes = encode_decode_xor(editBufDataSTR, editBufKeySTR, typeCrypt);
						SetDlgItemText(hwnd, ID_TEXTBOX, cryptRes.c_str());
						if (IsDlgButtonChecked( hwnd, ID_CHBX_AUTOC)==1)
						{
							bfrWrite(cryptRes);
						}
                        return 0;
					}
					case ID_BTN_MD5:
					{
						char editBufData[1024];
						GetDlgItemText (hGroupBox3, ID_EDIT_MD5, editBufData, 1024);
						string editBufDataSTR = string(editBufData);
						if (editBufDataSTR=="")
						{
							MessageBox(hwnd, "Data empty", PROGRAM_NAME, MB_ICONINFORMATION);
							return 0;
						}
						string md5Res = md5(editBufDataSTR);
						SetDlgItemText(hwnd, ID_TEXTBOX, md5Res.c_str());
						if (IsDlgButtonChecked( hwnd, ID_CHBX_AUTOC)==1)
						{
							bfrWrite(md5Res);
						}
						return 0;
					}
					
			}
        }
 
        default:
            return DefWindowProc(hwnd, Message, wParam, lParam);
    }
    return 0;
}
 

LRESULT CALLBACK GroupBoxProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch(Message)
    {
        case WM_COMMAND:
            CallWindowProc(WndProc, GetParent(hwnd), Message, wParam, lParam);
            break;
    }
 
    return CallWindowProc(OldGroupBoxProc, hwnd, Message, wParam, lParam);
}
 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	
    const char* szClassName = "WndClass";
    WNDCLASSEX wc;  
    wc.cbSize        = sizeof(wc);      
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, MAKEINTRESOURCE(NULL));
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Cannot register class", PROGRAM_NAME, MB_OK);
        return 0;
    }
	RECT rcWork;
	SystemParametersInfo( SPI_GETWORKAREA, 0, &rcWork, 0 );
	
	
	int iWidth = 600;
	int iHeight = 360;
    HWND hMainWnd = CreateWindowEx(WS_EX_APPWINDOW,szClassName,PROGRAM_NAME,WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZE,
        ( rcWork.left + rcWork.right - iWidth ) / 2,
        ( rcWork.top + rcWork.bottom - iHeight ) / 2,
        iWidth,iHeight,
        NULL,NULL,hInstance,NULL);
 
    if (!hMainWnd) {
        MessageBox(NULL, "Cannot create main window", PROGRAM_NAME, MB_OK);
        return 0;
    }
	HICON hIcon1 = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(1));
	SendMessage(hMainWnd, WM_SETICON, 1, (LPARAM)hIcon1);
	

	//Home
	SetStyle(CreateWindow("Button", "Autocopy result to buffer", BS_AUTOCHECKBOX|WS_VISIBLE|WS_CHILD|WS_TABSTOP, 
                350,10,180,20, hMainWnd, (HMENU)ID_CHBX_AUTOC, hInstance, NULL),"Arial",15,0);
	SetStyle(CreateWindow("Edit", "", WS_BORDER|WS_VISIBLE|WS_CHILDWINDOW|ES_NOHIDESEL|EN_CHANGE|ES_MULTILINE,
                350,40,230,100, hMainWnd, (HMENU)ID_TEXTBOX, hInstance, NULL),"Arial",15,0);
	SetText("For easy create pass, hash and crypt data",0,355, 290, 290, 20, hMainWnd, "Arial",GLOBAL_FONT_SIZE);
	SetText("By",0,490, 310, 290, 20, hMainWnd, "Arial",GLOBAL_FONT_SIZE);
	
	SetStyle(CreateWindow("Syslink", "<A HREF=\"https://vk.com/serov.sergey\">SergoMorello</A> ", BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE, 507, 310, 290, 20, hMainWnd, (HMENU)ID_MY_LINK, hInstance, NULL),"Arial",15,1);
	
	//
	
	//Rand pass
	
	hGroupBox = CreateWindow("Button","Random password",WS_CHILD | WS_VISIBLE | BS_GROUPBOX,10,10,300,90,hMainWnd,(HMENU) 100, hInstance,NULL);
	SetStyle(hGroupBox,"Arial",15,1);
	SetText("Password size:",0, 10, 32, 90, 24, hGroupBox, "Arial",GLOBAL_FONT_SIZE);

	SetStyle(CreateWindow("Edit", "8", WS_BORDER|WS_VISIBLE|WS_CHILDWINDOW|ES_AUTOHSCROLL|ES_NOHIDESEL|EN_CHANGE|ES_NUMBER,
                100,30,20,20, hGroupBox, (HMENU)ID_EDIT1, hInstance, NULL),"Arial",15,0);
				
	SetStyle(CreateWindow( "Button", "Special symbols", BS_AUTOCHECKBOX|WS_VISIBLE|WS_CHILD|WS_TABSTOP, 
                10,60,150,20, hGroupBox, (HMENU)ID_CHECKBOX1, hInstance, NULL),"Arial",15,0);

	SetStyle(CreateWindow("Button", "Rand", BS_PUSHBUTTON|WS_VISIBLE|WS_CHILD|WS_TABSTOP, 
                230,60,60,20, hGroupBox, (HMENU)ID_BUTTON1, hInstance, NULL),"Arial",15,0);
	
	//

	//XOR
	hGroupBox2 = CreateWindow("Button","XOR en/decrypt",WS_CHILD |WS_VISIBLE | BS_GROUPBOX,10,110,300,120,hMainWnd,(HMENU) 100, hInstance,NULL);
	SetStyle(hGroupBox2,"Arial",15,1);
	SetText("Data:",0, 10, 32, 90, 24, hGroupBox2, "Arial",GLOBAL_FONT_SIZE);
	SetStyle(CreateWindow("Edit", "", WS_BORDER|WS_VISIBLE|WS_CHILDWINDOW|ES_AUTOHSCROLL|ES_NOHIDESEL|EN_CHANGE,
                45,30,245,20, hGroupBox2, (HMENU)ID_EDIT_CRPT1, hInstance, NULL),"Arial",15,0);
	SetText("Key:",0, 10, 62, 90, 24, hGroupBox2, "Arial",GLOBAL_FONT_SIZE);
	SetStyle(CreateWindow("Edit", "", WS_BORDER|WS_VISIBLE|WS_CHILDWINDOW|ES_AUTOHSCROLL|ES_NOHIDESEL|EN_CHANGE,
                45,60,245,20, hGroupBox2, (HMENU)ID_EDIT_CRPT2, hInstance, NULL),"Arial",15,0);
	SetStyle(CreateWindow( "Button", "Full XOR (binary)", BS_AUTOCHECKBOX|WS_VISIBLE|WS_CHILD|WS_TABSTOP, 
                10,90,150,20, hGroupBox2, (HMENU)ID_CHECKBOX2, hInstance, NULL),"Arial",15,0);
	SetStyle(CreateWindow("Button", "Crypt", BS_PUSHBUTTON|WS_VISIBLE|WS_CHILD|WS_TABSTOP, 
                230,90,60,20, hGroupBox2, (HMENU)ID_BTN_CRYPT, hInstance, NULL),"Arial",15,0);
	
	//
	
	//MD5
	hGroupBox3 = CreateWindow("Button","MD5 hash",WS_CHILD |WS_VISIBLE | BS_GROUPBOX,10,235,300,90,hMainWnd,(HMENU) 100, hInstance,NULL);
	SetStyle(hGroupBox3,"Arial",15,1);
	SetText("Data:",0, 10, 32, 90, 24, hGroupBox3, "Arial",GLOBAL_FONT_SIZE);
	SetStyle(CreateWindow("Edit", "", WS_BORDER|WS_VISIBLE|WS_CHILDWINDOW|ES_AUTOHSCROLL|ES_NOHIDESEL|EN_CHANGE,
                45,30,245,20, hGroupBox3, (HMENU)ID_EDIT_MD5, hInstance, NULL),"Arial",15,0);
	SetStyle(CreateWindow("Button", "Hash", BS_PUSHBUTTON|WS_VISIBLE|WS_CHILD|WS_TABSTOP, 
                230,60,60,20, hGroupBox3, (HMENU)ID_BTN_MD5, hInstance, NULL),"Arial",15,0);
	
	//

	OldGroupBoxProc = (WNDPROC)::GetWindowLongPtr(hGroupBox2, GWLP_WNDPROC);
	SetWindowLongPtr(hGroupBox, GWLP_WNDPROC, (LRESULT)GroupBoxProc);
	SetWindowLongPtr(hGroupBox2, GWLP_WNDPROC, (LRESULT)GroupBoxProc);
	SetWindowLongPtr(hGroupBox3, GWLP_WNDPROC, (LRESULT)GroupBoxProc);
    ShowWindow(hMainWnd, nCmdShow);   
    UpdateWindow(hMainWnd);
    
	
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))  {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}