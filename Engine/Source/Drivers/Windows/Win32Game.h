#pragma once
#include "Core/Config.h"
#include "Core/Interface/IPlatform.h"

#if defined(_WIN32)
namespace Eggy
{
	class Win32Game : public IPlatform
	{
	public:
		Win32Game();
		WINDOW_HANDLE GetWindow() override { return mWindow_; }
		void PumpMessage() override;

	protected:
		static LRESULT CALLBACK WndProc(WINDOW_HANDLE hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		WINDOW_HANDLE InitializeWindow();

	protected:
		WINDOW_HANDLE mWindow_;
		HINSTANCE mWindowInstance_;
	};
	
}
#endif