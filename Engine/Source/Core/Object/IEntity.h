#pragma once
#include "IObject.h"
#include "Core/Math/Matrix4x3.h"


namespace Eggy
{
	class IComponent;
	class IEntity : public IObject
	{
	public:
		bool AddComponent(IComponent* comp);

		template<typename TComp>
		TComp* GetComponent()
		{
			for (auto comp : mComponents_)
			{
				TComp* comp = dynamic_cast<TComp*>(comp);
				if (comp)
					return comp;
			}
			return nullptr;
		}

		template<typename TComp, typename... TArgs>
		void AddComponent(TArgs... args)
		{
			auto comp = new TComp(std::forward<TArgs>(args)...);
			AddComponent(comp);
		}

		const Matrix4x3& GetTransform() { return mTransform_; }
		void SetTransform(Matrix4x3& transf) { mTransform_ = transf; }

	protected:
		List<IComponent*> mComponents_;
		Matrix4x3 mTransform_;
	};
}