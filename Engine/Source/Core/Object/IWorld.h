#pragma once
#include "IObject.h"


namespace Eggy
{
	class IEntity;
	class IPrimitive;
	class IRenderScene;

	class IWorld : public IObject
	{
	public:
		IWorld();
		virtual ~IWorld();

		String Serialize() const override;
		void Deserialize(const String& s) override;

		void AddPrimitives(IPrimitive* prim);
		void DelPrimitives(IPrimitive* prim);

		virtual void Tick() {};
		List<IPrimitive*>& GetPrimitives() { return mPrimitives_; }

		virtual void StartFrame() = 0;
		virtual void EndFrame() = 0;

	protected:
		List<IEntity*> mEntities_;
		List<IPrimitive*> mPrimitives_;
	};
}