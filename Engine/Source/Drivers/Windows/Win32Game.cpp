#include "Win32Game.h"
#include "Core/System/ConfigSystem.h"
#include "Core/System/InputSystem.h"


#if defined(_WIN32)
namespace Eggy
{

	Win32Game::Win32Game()
	{
		InitializeWindow();
	}

	void Win32Game::PumpMessage()
	{
		BOOL bRet;
		MSG msg;
		while ((bRet = GetMessage(&msg, mWindow_, 0, 0)) != 0)
		{
			if (bRet == -1)
			{
				// handle the error and possibly exit
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	LRESULT Win32Game::WndProc(WINDOW_HANDLE hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		InputSystem::Get()->HandleMessage(hWnd, uMsg, wParam, lParam);
		return (DefWindowProc(hWnd, uMsg, wParam, lParam));
	}

	WINDOW_HANDLE Win32Game::InitializeWindow()
	{
		mWindowInstance_ = GetModuleHandle(0);

		WNDCLASSEX wndClass;

		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc = WndProc;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;
		wndClass.hInstance = mWindowInstance_;
		wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wndClass.lpszMenuName = NULL;
		wndClass.lpszClassName = Tool::stringToLPCWSTR(ConfigSystem::Get()->GetGlobalConfig("AppTitle", "Eggy"));
		wndClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

		if (!RegisterClassEx(&wndClass))
		{
			fflush(stdout);
			exit(1);
		}

		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		if (ConfigSystem::Get()->GetGlobalConfig("IsFullScreen", false))
		{
			if ((ConfigSystem::Get()->GetGlobalConfig("AppWidth", 800) != (uint32_t)screenWidth) && (ConfigSystem::Get()->GetGlobalConfig("AppHeight", 600) != (uint32_t)screenHeight))
			{
				DEVMODE dmScreenSettings;
				memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
				dmScreenSettings.dmSize = sizeof(dmScreenSettings);
				dmScreenSettings.dmPelsWidth = ConfigSystem::Get()->GetGlobalConfig("AppWidth", 800);
				dmScreenSettings.dmPelsHeight = ConfigSystem::Get()->GetGlobalConfig("AppHeight", 600);
				dmScreenSettings.dmBitsPerPel = 32;
				dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
				if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
				{
					if (MessageBox(NULL, L"Full Screen Mode not supported!\n Switch to window mode?", L"Error", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
					{
						ConfigSystem::Get()->SetGlobalConfig("IsFullScreen", false);
					}
					else
					{
						return nullptr;
					}
				}
				screenWidth = ConfigSystem::Get()->GetGlobalConfig("AppWidth", 800);
				screenHeight = ConfigSystem::Get()->GetGlobalConfig("AppHeight", 600);
			}
		}

		DWORD dwExStyle;
		DWORD dwStyle;

		if (ConfigSystem::Get()->GetGlobalConfig("IsFullScreen", false))
		{
			dwExStyle = WS_EX_APPWINDOW;
			dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		}
		else
		{
			dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
			dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		}

		RECT windowRect;
		windowRect.left = 0L;
		windowRect.top = 0L;
		windowRect.right = ConfigSystem::Get()->GetGlobalConfig("IsFullScreen", false) ? (long)screenWidth : (long)ConfigSystem::Get()->GetGlobalConfig("AppWidth", 800);
		windowRect.bottom = ConfigSystem::Get()->GetGlobalConfig("IsFullScreen", false) ? (long)screenHeight : (long)ConfigSystem::Get()->GetGlobalConfig("AppHeight", 600);

		AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

		std::string windowTitle = ConfigSystem::Get()->GetGlobalConfig("AppTitle", "Eggy");
		mWindow_ = CreateWindowEx(0,
			Tool::stringToLPCWSTR(windowTitle),
			Tool::stringToLPCWSTR(windowTitle),
			dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			0,
			0,
			windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top,
			NULL,
			NULL,
			mWindowInstance_,
			NULL);

		if (!ConfigSystem::Get()->GetGlobalConfig("IsFullScreen", false))
		{
			// Center on screen
			uint32_t x = (GetSystemMetrics(SM_CXSCREEN) - windowRect.right) / 2;
			uint32_t y = (GetSystemMetrics(SM_CYSCREEN) - windowRect.bottom) / 2;
			SetWindowPos(mWindow_, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		}

		if (!mWindow_)
		{
			printf("Could not create window!\n");
			fflush(stdout);
			return nullptr;
		}

		ShowWindow(mWindow_, SW_SHOW);
		SetForegroundWindow(mWindow_);
		SetFocus(mWindow_);
		return mWindow_;
	}
}
#endif