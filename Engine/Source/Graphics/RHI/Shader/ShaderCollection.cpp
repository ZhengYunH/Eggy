#include "ShaderCollection.h"
#include "Graphics/RHI/IShaderRenderResource.h"


namespace Eggy
{
	static String GetShaderPath(const String& shaderPath, EShaderStage stage)
	{
		switch (stage)
		{
		case EShaderStage::VS:
			return shaderPath + "_VS.hlsl";
			break;
		case EShaderStage::PS:
			return shaderPath + "_PS.hlsl";
			break;
		default:
			Unimplement();
			break;
		}
		return "";
	}

	ShaderStageInstance::ShaderStageInstance(EShaderStage stage, const String& shaderPath, String entry)
		: _Stage(stage)
		, _Entry(entry)
	{
		_ShaderPath = GetShaderPath(shaderPath, stage);
		_Reflection = ShaderReflectionFactory::Instance().GetReflection(_ShaderPath, _Entry, stage);
		_ParseDescriptorInternel();
		_ShaderRenderResource = new ShaderRenderResource(this);
	}

	ShaderStageInstance::~ShaderStageInstance()
	{
		_Reflection = nullptr;
		for (auto& pair : _BatchMap)
			SafeDestroy(pair.second);
	}

	bool ShaderStageInstance::GetTextureSlot(const String& name, uint8& outSlot) const
	{
		for (auto& image : _Images)
		{
			if (image.Name == name)
			{
				outSlot = image.Binding;
				return true;
			}
		}
		return false;
	}

	bool ShaderStageInstance::GetSamplerSlot(const String& name, uint8& outSlot) const
	{
		for (auto& ss : _SamplerStates)
		{
			if (ss.Name == name)
			{
				outSlot = ss.Binding;
				return true;
			}
		}
		return false;
	}

	List<EShaderConstant> ShaderStageInstance::GetReletedBatch() const
	{
		List<EShaderConstant> ret;
		for (auto& pair : _BatchMap)
			ret.push_back(pair.first);
		return ret;
	}

	void ShaderStageInstance::_ParseDescriptorInternel()
	{
		auto descriptor = _Reflection->GetDescriptor();
		if (descriptor.empty())
			return;

		// TODO: support multi-set, now just ignore binding set
		for (auto& set2Desc : descriptor)
		{
			uint32 set = set2Desc.first;
			for (auto desc2Binding : set2Desc.second)
			{
				switch (desc2Binding.first)
				{
				case EDescriptorType::UNIFORM_BUFFER:
					_ParseUniform(desc2Binding.second);
					break;
				case EDescriptorType::SAMPLED_IMAGE:
					_ParseImage(desc2Binding.second);
					break;
				case EDescriptorType::SAMPLER:
					_ParseSampler(desc2Binding.second);
					break;
				default:
					break;
				}
			}
		}
	}

	void ShaderStageInstance::_ParseUniform(Map<uint8, SShaderDescriptorData>& uniforms)
	{
		for (auto& binding2BindingData : uniforms)
		{
			EShaderConstant esc = EShaderConstant::END;
			auto& bindingData = binding2BindingData.second;
			if (bindingData.Name == "Global")			esc = EShaderConstant::Global;
			else if (bindingData.Name == "Shader")		esc = EShaderConstant::Shader;
			else if (bindingData.Name == "Batch")		esc = EShaderConstant::Batch;
			else if (bindingData.Name == "Skeleton")	esc = EShaderConstant::Skeleton;
			else if (bindingData.Name == "Light")		esc = EShaderConstant::Light;
			else LOG(Shader, Warning) << "Invalid Shader Constant" << bindingData.Name << "; in Shader: " << _ShaderPath << "; in Stage: " << _Entry;

			if (esc == EShaderConstant::END)
				continue;
			_BatchMap[esc] = new ShadingParameterTable();
			_BatchSlot[esc] = binding2BindingData.first;
			for (uint32 i = 0; i < bindingData.Uniform.MemberCount; ++i)
			{
				auto& member = bindingData.Uniform.Members[i];
				switch (member.Numeric.Type)
				{
				case SShaderInputNumericType::MATRIX:
					if (member.Numeric.Block.Matrix.column_count == 3 || member.Numeric.Block.Matrix.row_count == 3)
						_BatchMap[esc]->AddParameter(member.Name, EShaderParameterType::Matrix4x3);
					else
						_BatchMap[esc]->AddParameter(member.Name, EShaderParameterType::Matrix4x4);
					break;
				case SShaderInputNumericType::VECTOR:
					_BatchMap[esc]->AddParameter(member.Name, EShaderParameterType::Float, member.Numeric.Block.Vector.component_count);
					break;
				case SShaderInputNumericType::SCALER:
					_BatchMap[esc]->AddParameter(member.Name, EShaderParameterType::Float, 1);
					break;
				default:
					break;
				}
			}
		}
	}

	void ShaderStageInstance::_ParseImage(Map<uint8, SShaderDescriptorData>& images)
	{
		_MaxImageBinding = 0;
		for (auto& binding2BindingData : images)
			_MaxImageBinding = Max(_MaxImageBinding, binding2BindingData.first + 1);

		for (auto& binding2BindingData : images)
		{
			auto& bindingData = binding2BindingData.second;
			_Images.push_back({ binding2BindingData.first, bindingData.Name, bindingData.SampledImage.Format, bindingData.SampledImage.Type });
		}
	}

	void ShaderStageInstance::_ParseSampler(Map<uint8, SShaderDescriptorData>& samplers)
	{
		_MaxSamplerStateBinding = 0;
		for (auto& binding2BindingData : samplers)
			_MaxSamplerStateBinding = Max(_MaxSamplerStateBinding, binding2BindingData.first + 1);

		for (auto& binding2BindingData : samplers)
		{
			auto& bindingData = binding2BindingData.second;
			_SamplerStates.push_back({ binding2BindingData.first, bindingData.Name });
		}
	}

	ShaderTechnique::ShaderTechnique(ETechnique technique, const String& shaderPath)
	{
		using enum EShaderStage;
		mStageInstances_[VS] = new ShaderStageInstance(VS, shaderPath, "VS");
		mStageInstances_[PS] = new ShaderStageInstance(PS, shaderPath, "PS");
	}

	ShaderTechnique::~ShaderTechnique()
	{
		for (auto& pair : mStageInstances_)
			SafeDestroy(pair.second);
	}

	void ShaderTechnique::CreateDeviceResource(IRenderResourceFactory* factory)
	{
		for (auto& stage2Instance : mStageInstances_)
			stage2Instance.second->_ShaderRenderResource->CreateDeviceResource(factory);
	}

	const ShadingParameterTable* ShaderTechnique::GetConstantTable(EShaderConstant esc) const
	{
		if (mStageInstances_.at(EShaderStage::PS)->_BatchMap.contains(esc))
			return mStageInstances_.at(EShaderStage::PS)->_BatchMap.at(esc);
		return nullptr;
	}

	const ShaderStageInstance* ShaderTechnique::GetStageInstance(EShaderStage stage) const
	{
		if (!mStageInstances_.contains(stage))
			return nullptr;
		return mStageInstances_.at(stage);
	}

	bool ShaderTechnique::GetTextureSlot(EShaderStage stage, const String& name, uint8& outSlot) const
	{
		if (!mStageInstances_.contains(stage))
			return false;
		
		return mStageInstances_.at(stage)->GetTextureSlot(name, outSlot);
	}

	bool ShaderTechnique::GetSamplerSlot(EShaderStage stage, const String& name, uint8& outSlot) const
	{
		if (!mStageInstances_.contains(stage))
			return false;
		
		return mStageInstances_.at(stage)->GetSamplerSlot(name, outSlot);
	}

	ShaderCollection::ShaderCollection(const String& shaderPath)
	{
		mTechnique_[ETechnique::Shading] = new ShaderTechnique(ETechnique::Shading, shaderPath);
	}

	ShaderCollection::~ShaderCollection()
	{
		for (auto& pair : mTechnique_)
			SafeDestroy(pair.second);
	}

	bool ShaderCollection::IsTechniqueExist(ETechnique technique) const
	{
		return mTechnique_.contains(technique);
	}

	const Eggy::ShaderTechnique* ShaderCollection::GetShaderTechnique(ETechnique technique) const
	{
		HYBRID_CHECK(IsTechniqueExist(technique));
		return mTechnique_.at(technique);
	}

	ShaderCollectionFactory::~ShaderCollectionFactory()
	{
		for (auto& pair : mCollections_)
			SafeDestroy(pair.second);
	}

	const ShaderCollection* ShaderCollectionFactory::GetCollection(const String& shaderPath)
	{
		if (!mCollections_.contains(shaderPath))
			mCollections_[shaderPath] = Factory::Instance().Create(shaderPath);
		return mCollections_.at(shaderPath);
	}
}

