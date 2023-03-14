#include "World.h"
#include "ClientScene.h"
#include "RenderScene.h"
#include "Camera.h"
#include "Core/Object/IEntity.h"
#include "Resource/ResourceModule.h"


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

	void World::StartLogicFrame()
	{
		ResourceModule::Instance().Tick_rst();
	}

	void World::EndLogicFrame()
	{
	}

	void World::StartRenderFrame()
	{
		mRenderScene_->StartFrame();
	}

	void World::EndRenderFrame()
	{
		mRenderScene_->EndFrame();
	}

	void World::StartDeviceFrame()
	{
		mRenderScene_->StartDeviceFrame();
	}

	void World::EndDeviceFrame()
	{
		mRenderScene_->EndDeviceFrame();
	}

	void World::Tick()
	{
		static bool isForward = true;

		IEntity* entity = *(mEntities_.cbegin());
		Matrix4x3 transf = entity->GetTransform();
		auto trans = transf.GetTranslation();
		float dt = Engine::Get()->GetDeltaTime();
		if (trans.y > 1)
			isForward = false;
		if (trans.y < -1)
			isForward = true;
		transf.SetTranslation(transf.GetTranslation() + Vector3(0, isForward ? dt : -dt, 0));
		entity->SetTransform(transf);

		mMainCamera_->Tick(Engine::Get()->GetDeltaTime());
	}

}