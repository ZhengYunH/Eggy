#pragma once
#include "Config.h"
#include "Object/IObject.h"
#include "Resource/ModelResource.h"


namespace Eggy
{
	class Mesh;
	class Material;
	class Model : public IObject
	{
	public:
		Model() = default;
		~Model();

	public:
		void SetResourceID(const Guid& resourceID);
		virtual bool IsReady();
		Mesh* GetMesh() const { return mMesh_; }
		Material* GetMaterial() const { return mMaterial_; }

	protected:
		void _OnResourceLoaded(uint8 token);

	protected:
		TSharedPtr<ModelResource> mResource_;
		uint8 mToken_{ 0 };
		Mesh* mMesh_{ nullptr };
		Material* mMaterial_{ nullptr };
	};
}