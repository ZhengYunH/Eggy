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

		void StartFrame() override;
		void EndFrame() override;

		const Camera* GetCamera() { return mMainCamera_; }


	protected:
		ClientScene* mScene_{ nullptr };
		IRenderScene* mRenderScene_{ nullptr };
		Camera* mMainCamera_{ nullptr };;
	};
}


