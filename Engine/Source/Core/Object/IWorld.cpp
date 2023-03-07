#include "IWorld.h"
#include "IEntity.h"
#include "IComponent.h"
#include "IPrimitive.h"

// test code
#include "Client/RenderScene.h"
#include "Client/ClientScene.h" // #include "Scene.h"
#include "Client/PrimitivesComponent.h"
#include "Client/LightComponent.h"


namespace Eggy
{

	IWorld::IWorld()
	{
	}

	IWorld::~IWorld()
	{
	}

	String IWorld::Serialize() const
	{
		String s;
		for (auto& entity : mEntities_)
		{
			s += entity->Serialize();
		}
		return s;
	}

	void IWorld::Deserialize(const String& s)
	{
		IEntity* entity1 = new IEntity();
		PrimitiveComponent* prim1 = new PrimitiveComponent();
		prim1->LoadResource("EngineDefault/Box/Model");
		entity1->AddComponent(prim1);
		entity1->EnterWorld(this);
		Matrix4x3 transf1;
		transf1.SetScale(Vector3(0.5f, 0.5f, 0.5f));
		entity1->SetTransform(transf1);
		mEntities_.push_back(entity1);

		IEntity* entity2 = new IEntity();
		PrimitiveComponent* prim2 = new PrimitiveComponent();
		prim2->LoadResource("EngineDefault/Box/Model");
		entity2->AddComponent(prim2);
		entity2->EnterWorld(this);
		Matrix4x3 transf2;
		transf2.SetTranslation(Vector3(1.f, 0, -0.5f));
		entity2->SetTransform(transf2);
		mEntities_.push_back(entity2);

		IEntity* directLight = new IEntity();
		Matrix4x3 transf3;
		transf3.SetPitchYawRoll(DegreeToRadian(-45.f), 0, 0);
		directLight->SetTransform(transf3);
		directLight->AddComponent(new DirectionLightComponent());
		directLight->EnterWorld(this);
		mEntities_.push_back(entity2);
	}

	void IWorld::AddPrimitives(IPrimitive* prim)
	{
		HYBRID_CHECK(std::find(mPrimitives_.begin(), mPrimitives_.end(), prim) == mPrimitives_.end());
		mPrimitives_.push_back(prim);
	}

	void IWorld::DelPrimitives(IPrimitive* prim)
	{
		auto itr = std::find(mPrimitives_.begin(), mPrimitives_.end(), prim);
		if (itr != mPrimitives_.end())
			mPrimitives_.erase(itr);
	}
}

