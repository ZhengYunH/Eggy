#pragma once
#include "Core/Object/IWorld.h"

namespace Eggy
{
	class IRenderScene;
	class ClientScene;

	class World : public IWorld
	{
	public:
		World(ClientScene* scene);
		~World();

		IRenderScene* GetRenderScene() { return mRenderScene_; }

	protected:
		ClientScene* mScene_;
		IRenderScene* mRenderScene_{ nullptr };
	};
}


