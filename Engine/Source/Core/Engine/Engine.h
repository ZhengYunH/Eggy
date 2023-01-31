#pragma once
#include "Core/Config.h"
#include "Core/Interface/ISystem.h"
#include "Core/Interface/IPlatform.h"


namespace Eggy
{
	class Engine : public ISystem<Engine>
	{
		DeclareSystem(Engine);

	public:
		Engine(IPlatform* platform);

		void Initialize() override;
		void Finalize() override;
	
	public:
		void TickLoop();
		class ClientScene* GetClientScene() { return mClientScene_; }

	public:
		IPlatform* Platform{ nullptr };
		class ClientScene* mClientScene_ { nullptr };

	protected:
		typedef std::chrono::steady_clock::time_point TimeType;
		TimeType mLastFrameTime_;
		TimeType mCurrFrameTime_;

		float _DeltaTime;

	public:
		float const& GetDeltaTime() { return _DeltaTime; }
	};
}