#pragma once
#include "Core/Config.h"
#include "Core/Interface/ISystem.h"
#include "Core/Engine/Event/Event.h"
#include "Core/KeyCodes.h"


#define INPUT_SYSTEM_OP_EVENT(OP) Event_##OP
#define DEFINE_EVENT(OP, ...) Event<void, ##__VA_ARGS__> INPUT_SYSTEM_OP_EVENT(OP)
#define IS_MOUSE_EVENT(msg) (msg > WM_MOUSEFIRST && msg < WM_MOUSELAST)
#define IS_KEY_EVENT(msg) (msg == WM_KEYDOWN || msg == WM_KEYUP)

namespace Eggy
{
	class InputSystem : public TSystem<InputSystem>
	{
		DeclareSystem(InputSystem);

	public:
		DEFINE_EVENT(Close);
		DEFINE_EVENT(Size, bool/*IsMinimized*/, uint32/*Width*/, uint32/*Height*/);
		DEFINE_EVENT(EnterResizeMove);
		DEFINE_EVENT(ExitResizeMove);

		// Keyboard Event
		DEFINE_EVENT(KeyDown, uint32/* Key Code */);
		DEFINE_EVENT(KeyUp, uint32/* Key Code */);

		// Mouse Event
		DEFINE_EVENT(LeftMouseDown, uint32/* XPosition */, uint32/* YPosition */);
		DEFINE_EVENT(RightMouseDown, uint32/* XPosition */, uint32/* YPosition */);
		DEFINE_EVENT(MidMouseDown, uint32/* XPosition */, uint32/* YPosition */);
		DEFINE_EVENT(LeftMouseUp, uint32/* XPosition */, uint32/* YPosition */);
		DEFINE_EVENT(RightMouseUp, uint32/* XPosition */, uint32/* YPosition */);
		DEFINE_EVENT(MidMouseUp, uint32/* XPosition */, uint32/* YPosition */);
		DEFINE_EVENT(MouseWheel, uint32 /* Wheel Offset */);
		DEFINE_EVENT(MouseMove, uint32/* XPosition */, uint32/* YPosition */);

		// Copy Data Event
		DEFINE_EVENT(CopyData, uint32, void*);

		void Initialize() override;
		void Finalize() override;

		Event<bool, UINT, WPARAM, LPARAM > MessageEvent;


	private:
#if defined(_WIN32)
		friend class Win32Game;
		void HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif
	};
}
#undef DEFINE_EVENT

#define BindInputEvent(OPType, Object, Func) EVENT_BIND_MEMBER_FUNC(InputSystem::Get()->Event_##OPType, Object, Func)
