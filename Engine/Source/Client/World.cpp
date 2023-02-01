#include "World.h"
#include "ClientScene.h"
#include "RenderScene.h"
#include "Camera.h"


namespace Eggy
{
	World::World(ClientScene* scene)
		: IWorld()
		, mScene_(scene)
	{
		mRenderScene_ = new RenderScene();
		mMainCamera_ = new Camera();
		// test code
		{
			Matrix4x3 viewMat;
			viewMat.LookAt(Vector3(0.f, 0.f, -5.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f));
			mMainCamera_->setTransform(viewMat);
		}
		mRenderScene_->SetCamera(mMainCamera_);
	}

	World::~World()
	{
		SafeDestroy(mMainCamera_);
		SafeDestroy(mRenderScene_);
	}

	void World::StartFrame()
	{
		mRenderScene_->StartFrame();
	}

	void World::EndFrame()
	{
		mRenderScene_->EndFrame();
	}

	void World::Tick()
	{
		mMainCamera_->tick(Engine::Get()->GetDeltaTime());
	}

}