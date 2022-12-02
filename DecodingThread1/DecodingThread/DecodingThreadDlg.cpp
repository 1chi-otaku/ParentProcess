#include "DecodingThreadDlg.h"

DecodingThreadDlg* DecodingThreadDlg::ptr = NULL;

DecodingThreadDlg::DecodingThreadDlg(void)
{
	ptr = this;
}

DecodingThreadDlg::~DecodingThreadDlg(void)
{
	
}

void DecodingThreadDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

DWORD WINAPI Decoding_Thread(LPVOID lp) //Read File
{
	DecodingThreadDlg *ptr = (DecodingThreadDlg *)lp;

	HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, false, TEXT("{B8A2C367-10FE-494d-A869-841B2AF972E0}"));
	
	DWORD dwAnswer = WaitForSingleObject(hMutex, INFINITE);
	if(dwAnswer == WAIT_OBJECT_0)
	{
		int buffer;
		ifstream out(TEXT("text.txt"));
		ofstream copy(TEXT("copy.txt"));

		if (!out.is_open() || !copy.is_open()) {
			return 1;
		}

		for (int i = 0; i < 10; i++)
		{
			out >> buffer;
			copy << buffer << " ";
		}

		out.close();
		copy.close();
		ReleaseMutex(hMutex);

		MessageBox(ptr->hDialog, TEXT("Копия была создана"), TEXT("Мьютекс"), MB_OK | MB_ICONINFORMATION);
	}
	return 0;
}

BOOL DecodingThreadDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) 
{
	hDialog = hwnd;
	CreateThread(NULL, 0, Decoding_Thread, this, 0, NULL); 
	return TRUE;
}

BOOL CALLBACK DecodingThreadDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
	}
	return FALSE;
}