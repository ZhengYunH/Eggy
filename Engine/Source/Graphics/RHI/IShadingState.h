#pragma once
#include "IRenderResource.h"
#include "Core/Object/Material.h"
#include "Core/Object/Shader.h"
#include "System/TextureSystem.h"
#include "IShadingBatch.h"
#include "Shader/ShaderCollection.h"


namespace Eggy
{


	class Texture;
	class IShadingState
	{
	public:
		IShadingState(Shader* shader)
		{
			mCollection_ = shader->GetShaderCollection();
			mBatch_ = new IShadingBatch(shader);
		}

		~IShadingState()
		{
			SafeDestroy(mBatch_);
		}

	public:
		void CreateDeviceResource(IRenderResourceFactory* factory)
		{
			mBatch_->CreateDeviceResource(factory);
		}

		void SetDataFromMaterial(Material* material)
		{
			for (auto& pair : material->GetTextures())
			{
				uint8 slot = 0;
				EShaderStage stage = EShaderStage::PS;
				if (mBatch_->GetTextureSlot(ETechnique::Shading, stage, pair.first, slot))
					mBatch_->mResourceBindings_[stage]->SetTexture(slot, pair.second->GetRenderTexture());
			}
			mBatch_->SetConstantBuffer(EShaderConstant::Shader, material->GetParameters());
		}

		IShadingBatch* GetBatch() { return mBatch_; }

		const ShaderCollection* GetShaderCollection() { return mCollection_; }

	protected:
		const ShaderCollection* mCollection_;
		IShadingBatch* mBatch_;
	};
}
