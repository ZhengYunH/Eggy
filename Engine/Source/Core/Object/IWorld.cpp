#include "IWorld.h"
#include "IEntity.h"
#include "IComponent.h"
#include "IPrimitive.h"

// test code
#include "Client/RenderScene.h"
#include "Client/ClientScene.h" // #include "Scene.h"
#include "Client/PrimitivesComponent.h"


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
		entity1->AddComponent(new PrimitiveComponent());
		entity1->EnterWorld(this);
		mEntities_.push_back(entity1);

		IEntity* entity2 = new IEntity();
		PrimitiveComponent* prim = new PrimitiveComponent();
		prim->LoadResource("Package/EngineDefault/Mesh/CameraModel.fbx");
		entity2->AddComponent(prim);
		entity2->EnterWorld(this);
		Matrix4x3 transf;
		transf.SetTranslation(Vector3(1.f, 0, -0.5f));
		entity2->SetTransform(transf);
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

