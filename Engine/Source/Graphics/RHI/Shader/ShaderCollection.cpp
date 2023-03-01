#include "ShaderCollection.h"


namespace Eggy
{
	static String GetShaderPath(const String& shaderPath, EShaderType shaderType)
	{
		switch (shaderType)
		{
		case EShaderType::VS:
			return shaderPath + "_VS.hlsl";
			break;
		case EShaderType::PS:
			return shaderPath + "_PS.hlsl";
			break;
		default:
			Unimplement();
			break;
		}
		return "";
	}

	ShaderTechnique::ShaderTechnique(ETechnique technique, const String& shaderPath)
	{
		using enum EShaderType;
		mShaderPath_ = shaderPath;

		mEntrys_[VS] = StageData();
		mEntrys_[VS]._Entry = "VS";

		mEntrys_[PS] = StageData();
		mEntrys_[PS]._Entry = "PS";
		ParseReflectionImpl();
	}

	ShaderTechnique::~ShaderTechnique()
	{
		for (auto& pair : mEntrys_)
		{
			StageData& data = pair.second;
			for (auto& batchPair : data._BatchMap)
				SafeDestroy(batchPair.second);
		}
	}

	void ShaderTechnique::ParseReflectionImpl()
	{
		for (auto& pair : mEntrys_)
		{
			StageData& data = pair.second;
			data._Reflection = ShaderReflectionFactory::Instance().GetReflection(GetShaderPath(mShaderPath_, pair.first), data._Entry, pair.first);
			_ParseDescriptorInternel(data, *data._Reflection);
		}
	}

	void ShaderTechnique::_ParseDescriptorInternel(StageData& stageData, ShaderReflection& reflection)
	{
		auto descriptor = reflection.GetDescriptor();
		if (descriptor.empty())
			return;

		// TODO: support multi-set, now just ignore binding set
		for (auto& set2Desc: descriptor)
		{
			uint32 set = set2Desc.first;
			for (auto desc2Binding : set2Desc.second)
			{
				switch (desc2Binding.first)
				{
				case EDescriptorType::UNIFORM_BUFFER:
					_ParseUniform(stageData, desc2Binding.second);
					break;
				case EDescriptorType::SAMPLED_IMAGE:
					_ParseImage(stageData, desc2Binding.second);
					break;
				case EDescriptorType::SAMPLER:
					_ParseSampler(stageData, desc2Binding.second);
					break;
				default:
					break;
				}
			}
		}
	}

	void ShaderTechnique::_ParseUniform(StageData& stageData, Map<uint8, SShaderDescriptorData>& uniforms)
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
			else LOG(Shader, Warning) << "Invalid Shader Constant" << bindingData.Name << "; in Shader: " << mShaderPath_ << "; in Stage: " << stageData._Entry;

			if (esc == EShaderConstant::END)
				continue;
			stageData._BatchMap[esc] = new ShadingBatch();
			for (uint32 i = 0; i < bindingData.Uniform.MemberCount; ++i)
			{
				auto& member = bindingData.Uniform.Members[i];
				switch (member.Numeric.Type)
				{
				case SShaderInputNumericType::MATRIX:
					if (member.Numeric.Block.Matrix.column_count == 3 || member.Numeric.Block.Matrix.row_count == 3)
						stageData._BatchMap[esc]->AddParameter(member.Name, EShaderParameterType::Matrix4x3);
					else
						stageData._BatchMap[esc]->AddParameter(member.Name, EShaderParameterType::Matrix4x4);
					break;
				case SShaderInputNumericType::VECTOR:
					stageData._BatchMap[esc]->AddParameter(member.Name, EShaderParameterType::Float, member.Numeric.Block.Vector.component_count);
					break;
				case SShaderInputNumericType::SCALER:
					stageData._BatchMap[esc]->AddParameter(member.Name, EShaderParameterType::Float, 1);
					break;
				default:
					break;
				}
			}
		}
	}

	void ShaderTechnique::_ParseImage(StageData& stageData, Map<uint8, SShaderDescriptorData>& images)
	{
		stageData._MaxImageBinding = 0;
		for (auto& binding2BindingData : images)
			stageData._MaxImageBinding = Max(stageData._MaxImageBinding, binding2BindingData.first);

		for (auto& binding2BindingData : images)
		{
			auto& bindingData = binding2BindingData.second;
			stageData._Images.push_back({ binding2BindingData.first, bindingData.Name, bindingData.SampledImage.Format, bindingData.SampledImage.Type });
		}
	}

	void ShaderTechnique::_ParseSampler(StageData& stageData, Map<uint8, SShaderDescriptorData>& samplers)
	{
		stageData._MAxSamplerStateBinding = 0;
		for (auto& binding2BindingData : samplers)
			stageData._MAxSamplerStateBinding = Max(stageData._MAxSamplerStateBinding, binding2BindingData.first);

		for (auto& binding2BindingData : samplers)
		{
			auto& bindingData = binding2BindingData.second;
			stageData._SamplerStates.push_back({ binding2BindingData.first, bindingData.Name });
		}
	}

	ShaderCollection::ShaderCollection(const String& shaderPath)
	{
		using enum EShaderType;
		using enum ETechnique;
		mTechnique_[Shading] = new ShaderTechnique(Shading, shaderPath);
	}

	ShaderCollection::~ShaderCollection()
	{
		for (auto& pair : mTechnique_)
			SafeDestroy(pair.second);
	}

	bool ShaderCollection::IsTechniqueExist(ETechnique technique)
	{
		return mTechnique_.contains(technique);
	}

	const ShaderTechnique* ShaderCollection::GetShaderTechnique(ETechnique technique)
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

