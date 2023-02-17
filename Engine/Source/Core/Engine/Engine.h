#pragma once
#include "Core/Config.h"
#include "Core/Interface/ISystem.h"
#include "Core/Interface/IPlatform.h"


namespace Eggy
{
	class Engine : public TSystem<Engine>
	{
		DeclareSystem(Engine);

	public:
		void Initialize() override;
		void PostInitialize();
		void Finalize() override;
	
	public:
		void SetPlatform(IPlatform* platform);
		bool TickLoop();
		class ClientScene* GetClientScene() { return mClientScene_; }
		float const& GetDeltaTime() { return _DeltaTime; }

	public:
		IPlatform* Platform{ nullptr };
		class ClientScene* mClientScene_ { nullptr };

	protected:
		typedef std::chrono::steady_clock::time_point TimeType;
		TimeType mLastFrameTime_;
		TimeType mCurrFrameTime_;

		float _DeltaTime;

		class SystemManager* mSystemManager_{ nullptr };
	};
}