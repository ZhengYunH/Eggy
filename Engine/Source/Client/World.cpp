#include "World.h"
#include "ClientScene.h"
#include "RenderScene.h"

namespace Eggy
{
	World::World(ClientScene* scene)
		: IWorld()
		, mScene_(scene)
	{
		mRenderScene_ = new RenderScene();
	}

	World::~World()
	{
		delete mRenderScene_;
	}
}