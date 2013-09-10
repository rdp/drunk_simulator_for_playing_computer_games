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


LRESULT CALLBACK LowLevelKeyboardProc( int nCode,
                                       WPARAM wParam,
                                       LPARAM lParam)
{
  printf("here we are\n");
  BOOL fEatKeystroke = FALSE;
  PKBDLLHOOKSTRUCT p;
  if (nCode == HC_ACTION)
  {
     switch (wParam)
     {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
 
           // Get hook struct
           p = ( PKBDLLHOOKSTRUCT ) lParam;
           /*fEatKeystroke = (( p->vkCode == VK_TAB ) &&
                           (( p->flags & LLKHF_ALTDOWN ) != 0 )) ||
                           (( p->vkCode == VK_ESCAPE ) &&
                           (( p->flags & LLKHF_ALTDOWN ) != 0 )) ||
                           (( p->vkCode == VK_ESCAPE ) &&
                           (( GetKeyState( VK_CONTROL ) & 0x8000) != 0 ));*/
                   
           fEatKeystroke = ( p->vkCode == VK_TAB );
 
           // Check whether we have a key
           if( fEatKeystroke )
           {
              MessageBox( 0, "Eating key stroke", "Error", MB_OK );
              break;
           }
     }// End switch
  }// End if
 
   // Did we trap a key??
   return( fEatKeystroke ? 1 : CallNextHookEx( NULL, nCode, wParam, lParam ));
}// End LowLevelKeyboardProc
 


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
    //LRESULT out = 33;//CallNextHookEx(hMouseHook, nCode, wParam, lParam);
    //Sleep(10000);
    //printf("returning %d ", out);
    return CallNextHookEx(hMouseHook, nCode, wParam, lParam);
    //return out;
    return CallNextHookEx(hMouseHook, nCode, wParam, lParam);
}

// gleaned from http://ntcoder.com/bab/2007/06/12/wh_keyboard_ll/ and http://stackoverflow.com/questions/11180773/setwindowshookex-for-wh-mouse

DWORD WINAPI MyMouseLogger(LPVOID lpParm)
{
    HINSTANCE hInstance = GetModuleHandle(NULL);

    hMouseHook = SetWindowsHookEx( WH_MOUSE_LL, mouseProc, hInstance, NULL );
    
    HHOOK hhkLowLevelKybd  = SetWindowsHookEx( WH_KEYBOARD_LL,
                                              LowLevelKeyboardProc,
                                              hInstance,
                                              0 );
    MSG message;
    while (GetMessage(&message,NULL,0,0)) {
        TranslateMessage( &message );
        DispatchMessage( &message );
    }
    
UnhookWindowsHookEx(hhkLowLevelKybd);
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