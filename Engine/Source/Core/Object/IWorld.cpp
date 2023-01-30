#include "IWorld.h"
#include "IEntity.h"
#include "IComponent.h"
#include "IPrimitive.h"

// test code
#include "Client/ClientScene.h" // #include "Scene.h"
#include "Client/PrimitivesComponent.h"


namespace Eggy
{
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
		IEntity* entity = new IEntity();
		entity->AddComponent(new PrimitiveComponent());
		entity->EnterWorld(this);
		mEntities_.push_back(entity);
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

	void IWorld::Tick()
	{
		for (auto prim : mPrimitives_)
		{
			prim->CollectPrimitives(mScene_->GetRenderScene());
		}
	}

}

