#include "IShadingBatch.h"
#include "Core/Object/Shader.h"


namespace Eggy
{
	IShadingBatch::IShadingBatch(class Shader* shader)
	{
		auto shaderCollection = shader->GetShaderCollection();
		auto shaderTechnique = shaderCollection->GetShaderTechnique(ETechnique::Shading);
		mShaderTechnique_[ETechnique::Shading] = shaderTechnique;
		mResourceBindings_ = new ResourceBinding(shaderTechnique->GetMaxImageBinding(), 0, 0);
	}

	IShadingBatch::~IShadingBatch()
	{
		SafeDestroy(mResourceBindings_);

		for (auto& pair : mShadingParameters_)
			delete pair.second;
		mShadingParameters_.clear();
	}

	void IShadingBatch::CreateDeviceResource(IRenderResourceFactory* factory)
	{
		for (auto& pair : mShadingParameters_)
			pair.second->CreateDeviceResource(factory);
		mResourceBindings_->CreateDeviceResource(factory);
	}

	ShadingParameterCollection* IShadingBatch::GetConstantBuffer(EShaderConstant esc)
	{
		if (!mShadingParameters_.contains(esc))
			return nullptr;
		return mShadingParameters_[esc];
	}

	void IShadingBatch::SetConstantBuffer(EShaderConstant esc, ShadingParameterCollection* param)
	{
		mShadingParameters_[esc] = param;
	}

	void IShadingBatch::SetTextureBinding(uint16 i, ITextureBuffer* tex)
	{
		mResourceBindings_->SetTexture(i, tex);
	}

	bool IShadingBatch::GetTextureSlot(ETechnique technique, const String& name, uint8& outSlot)
	{
		if (!mShaderTechnique_.contains(technique))
			return false;
		return mShaderTechnique_[technique]->GetTextureSlot(name, outSlot);
	}

	bool IShadingBatch::GetSamplerSlot(ETechnique technique, const String& name, uint8& outSlot)
	{
		if (!mShaderTechnique_.contains(technique))
			return false;
		return mShaderTechnique_[technique]->GetSamplerSlot(name, outSlot);
	}

	ResourceBinding* IShadingBatch::GetResourceBinding()
	{
		HYBRID_CHECK(mResourceBindings_);
		return mResourceBindings_;
	}

}
