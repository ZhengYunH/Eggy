#pragma once
#include "IObject.h"


namespace Eggy
{
	class IEntity;
	class IPrimitive;
	class ClientScene;

	class IWorld : public IObject
	{
	public:
		IWorld(ClientScene* scene) : mScene_(scene)
		{
		}

		String Serialize() const override;
		void Deserialize(const String& s) override;

		void AddPrimitives(IPrimitive* prim);
		void DelPrimitives(IPrimitive* prim);

		void Tick();

	protected:
		List<class IEntity*> mEntities_;
		List<IPrimitive*> mPrimitives_;
		ClientScene* mScene_{ nullptr };
	};

	

}