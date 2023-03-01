#include "IShadingBatch.h"
#include "Core/Object/Shader.h"


namespace Eggy
{
	IShadingBatch::IShadingBatch(class Shader* shader)
	{
		auto shaderCollection = shader->GetShaderCollection();
		auto shaderTechnique = shaderCollection->GetShaderTechnique(ETechnique::Shading);
		mShaderTechnique_[ETechnique::Shading] = shaderTechnique;
		for (uint8 i = uint8(EShaderStage::START) + 1; i < uint8(EShaderStage::END); ++i)
		{
			EShaderStage stage = EShaderStage(i);
			auto sd = shaderTechnique->GetStageInstance(EShaderStage(i));
			if (!sd)
				continue;
			uint16 textures = sd->_MaxImageBinding;
			uint16 buffers = 0;
			uint16 views = 0;
			mResourceBindings_[stage] = new ResourceBinding(textures, buffers, views);
		}
	}

	IShadingBatch::~IShadingBatch()
	{
		for (auto& pair : mResourceBindings_)
			delete pair.second;
		mResourceBindings_.clear();

		for (auto& pair : mShadingParameters_)
			delete pair.second;
		mShadingParameters_.clear();
	}

	void IShadingBatch::CreateDeviceResource(IRenderResourceFactory* factory)
	{
		for (auto& pair : mShadingParameters_)
			pair.second->CreateDeviceResource(factory);
		for (auto& pair : mResourceBindings_)
			pair.second->CreateDeviceResource(factory);
	}

	ShadingParameterCollection* IShadingBatch::GetConstantBuffer(EShaderConstant esc)
	{
		if (!mShadingParameters_.contains(esc))
			mShadingParameters_[esc] = new ShadingParameterCollection(nullptr);
		return nullptr;
	}

	void IShadingBatch::SetConstantBuffer(EShaderConstant esc, ShadingParameterCollection* param)
	{
		mShadingParameters_[esc] = param;
	}

	void IShadingBatch::SetTextureBinding(EShaderStage stage, uint16 i, ITextureBuffer* tex)
	{
		mResourceBindings_[stage]->SetTexture(i, tex);
	}

	bool IShadingBatch::GetTextureSlot(ETechnique technique, EShaderStage stage, const String& name, uint8& outSlot)
	{
		if (!mShaderTechnique_.contains(technique))
			return false;
		return mShaderTechnique_[technique]->GetTextureSlot(stage, name, outSlot);
	}

	bool IShadingBatch::GetSamplerSlot(ETechnique technique, EShaderStage stage, const String& name, uint8& outSlot)
	{
		if (!mShaderTechnique_.contains(technique))
			return false;
		return mShaderTechnique_[technique]->GetSamplerSlot(stage, name, outSlot);
	}

	ResourceBinding* IShadingBatch::GetResourceBinding(EShaderStage stage)
	{
		if (!mResourceBindings_.contains(stage))
			return nullptr;
		return mResourceBindings_.at(stage);
	}

}
