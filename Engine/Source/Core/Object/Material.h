#pragma once
#include "Core/Config.h"
#include "Core/Object/IObject.h"
#include "Resource/MaterialResource.h"
#include "Resource/TextureResource.h"
#include "Graphics/RHI/Shader/ShaderParamter.h"


namespace Eggy
{
	class Shader;
	class Texture;
	class Material : public IObject
	{
	public:
		Material() = default;
		Material(Shader* shader);

		Material(TSharedPtr<MaterialResource> resource);
		~Material();

		Map<String, ITexture*>& GetTextures() { return mTextures_; }
		ShadingParameterCollection* GetParameters() { return mParams_; }
		Shader* GetShader() { return mShader_; }
		ERenderSets GetRenderSet() const { return mRenderSet_; }
		void SetRenderSet(ERenderSets val) { mRenderSet_ = val; }
		void AddRenderSet(ERenderSet val) { mRenderSet_ |= ERenderSets(val); }

	protected:
		Shader* mShader_{ nullptr };
		Map<String, ITexture*> mTextures_;
		ShadingParameterCollection* mParams_{ nullptr };
		ERenderSets mRenderSet_{ ERenderSets(ERenderSet::Main) };
		
	};
}