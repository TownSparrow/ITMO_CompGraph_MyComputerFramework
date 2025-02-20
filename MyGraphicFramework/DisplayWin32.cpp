#include "DisplayWin32.h"
#include "Game.h"

LRESULT DisplayWin32::WndProc(
	HWND hwnd,
	UINT umessage,
	WPARAM wparam,
	LPARAM lparam) {
	  
	  Game* game = Game::GetInstance();
		
		switch (umessage) { 
		  case WM_KEYDOWN: {
			  std::cout << "Key: " << static_cast<unsigned int>(wparam) << std::endl;
				if (static_cast<unsigned int>(wparam) == 27)
					PostQuitMessage(0);
				return 0;
		  }
			default: {
				return DefWindowProc(hwnd, umessage, wparam, lparam);
			}
			case WM_INPUT: {
				UINT dwSize = 0;
				GetRawInputData(
					reinterpret_cast<HRAWINPUT>(lparam),
					RID_INPUT,
					nullptr,
					&dwSize,
					sizeof(RAWINPUTHEADER)
				);
				LPBYTE lpb = new BYTE[dwSize];
				if (lpb == nullptr) {
					return 0;
				}

				if (GetRawInputData(
					(HRAWINPUT)lparam,
					RID_INPUT,
					lpb,
					&dwSize,
					sizeof(RAWINPUTHEADER)) != dwSize
				)
					OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));

				RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(lpb);

				if (raw->header.dwType == RIM_TYPEKEYBOARD) {
					game->input->OnKeyDown({
						raw->data.keyboard.MakeCode,
						raw->data.keyboard.Flags,
						raw->data.keyboard.VKey,
						raw->data.keyboard.Message
					});
				}
				else if (raw->header.dwType == RIM_TYPEMOUSE) {
				game->input->OnMouseMove({
					raw->data.mouse.usFlags,
					raw->data.mouse.usButtonFlags,
					static_cast<int>(raw->data.mouse.ulExtraInformation),
					static_cast<int>(raw->data.mouse.ulRawButtons),
					static_cast<short>(raw->data.mouse.usButtonData),
					raw->data.mouse.lLastX,
					raw->data.mouse.lLastY
					});
				}

				delete[] lpb;
				return DefWindowProc(hwnd, umessage, wparam, lparam);
			}
		}
}

void DisplayWin32::Display() {
	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	ShowCursor(true);
}
