#define _WIN32_WINNT 0x0400
#pragma comment( lib, "user32.lib" )

#include <windows.h>
#include <stdio.h>

HHOOK hMouseHook;

/*
LowLevelHooksTimeout value in the following registry key:
HKEY_CURRENT_USER\Control Panel\Desktop
The value is in milliseconds
*/





LRESULT CALLBACK mouseProc (int nCode, WPARAM wParam, LPARAM lParam)
{
    MOUSEHOOKSTRUCT * pMouseStruct = (MOUSEHOOKSTRUCT *)lParam;
    if (pMouseStruct != NULL){
        if(wParam == WM_LBUTTONDOWN)
        {
            printf( "clicked" ); 
        }
        printf("Mouse position X = %d  Mouse Position Y = %d\n", pMouseStruct->pt.x,pMouseStruct->pt.y);
    }
    LRESULT out = 33;//CallNextHookEx(hMouseHook, nCode, wParam, lParam);
    Sleep(10000);
    printf("returning %d ", out);
    return out;
}

DWORD WINAPI MyMouseLogger(LPVOID lpParm)
{
    HINSTANCE hInstance = GetModuleHandle(NULL);

    // here I put WH_MOUSE instead of WH_MOUSE_LL
    hMouseHook = SetWindowsHookEx( WH_MOUSE_LL, mouseProc, hInstance, NULL );

    MSG message;
    while (GetMessage(&message,NULL,0,0)) {
        TranslateMessage( &message );
        DispatchMessage( &message );
    }

    UnhookWindowsHookEx(hMouseHook);
    return 0;
}

int main(int argc, char** argv)
{
    HANDLE hThread;
    DWORD dwThread;

    hThread = CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)MyMouseLogger, (LPVOID) argv[0], NULL, &dwThread);
    if (hThread)
        return WaitForSingleObject(hThread,INFINITE);
    else
        return 1;

}