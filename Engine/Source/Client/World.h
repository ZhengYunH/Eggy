#pragma once
#include "Core/Object/IWorld.h"

namespace Eggy
{
	class IRenderScene;
	class ClientScene;
	class Camera;

	class World : public IWorld
	{
	public:
		World(ClientScene* scene);
		~World();

		IRenderScene* GetRenderScene() { return mRenderScene_; }
		void Tick() override;

		void StartLogicFrame() override;
		void EndLogicFrame() override;

		void StartRenderFrame();
		void EndRenderFrame();

		void StartDeviceFrame();
		void EndDeviceFrame();

		const Camera* GetCamera() { return mMainCamera_; }


	protected:
		ClientScene* mScene_{ nullptr };
		IRenderScene* mRenderScene_{ nullptr };
		Camera* mMainCamera_{ nullptr };;
	};
}


