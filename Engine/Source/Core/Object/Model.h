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
		
		void SetMesh(Mesh* mesh) { HYBRID_CHECK(!mMesh_); mMesh_ = mesh; }
		Mesh* GetMesh() const { return mMesh_; }

		void SetMaterial(Material* mat) { HYBRID_CHECK(!mMaterial_); mMaterial_ = mat; }
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