#include "ShaderCollection.h"
#include "ShaderReflection.h"

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
		mEntrys_[VS] = StageData();
		mEntrys_[VS]._Entry = "VS";

		mEntrys_[PS] = StageData();
		mEntrys_[PS]._Entry = "PS";
		ParseReflectionImpl(shaderPath);
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

	void ShaderTechnique::ParseReflectionImpl(const String& shaderPath)
	{
		for (auto& pair : mEntrys_)
		{
			StageData& data = pair.second;
			data._Reflection = ShaderReflectionFactory::Instance().GetReflection(GetShaderPath(shaderPath, pair.first), data._Entry, pair.first);
			_ParseDescriptorInternel(shaderPath, data, *data._Reflection);
		}
	}

	void ShaderTechnique::_ParseDescriptorInternel(const String& shaderPath, StageData& stageData, ShaderReflection& reflection)
	{
		auto descriptor = reflection.GetDescriptor();
		if (descriptor.empty())
			return;

		// TODO: support multi-set, now just ignore binding set
		for (auto& setDescPair : descriptor)
		{
			uint32 set = setDescPair.first;
			for (auto pair : setDescPair.second)
			{
				uint32 binding = pair.first;
				SShaderDescriptorData& bindingData = pair.second;
				EShaderConstant esc = EShaderConstant::END;
				switch (bindingData.Type)
				{
				case EDescriptorType::UNIFORM_BUFFER:
					if (bindingData.Name == "Global")			esc = EShaderConstant::Global;
					else if (bindingData.Name == "Shader")		esc = EShaderConstant::Shader;
					else if (bindingData.Name == "Batch")		esc = EShaderConstant::Batch;
					else if (bindingData.Name == "Skeleton")	esc = EShaderConstant::Skeleton;
					else if (bindingData.Name == "Light")		esc = EShaderConstant::Light;
					else LOG(Shader, Warning) << "Invalid Shader Constant" << bindingData.Name << "; in Shader: " << shaderPath << "; in Stage: " << stageData._Entry;

					if(esc == EShaderConstant::END)
						continue;
					stageData._BatchMap[esc] = new ShadingBatch(bindingData.Uniform.Size);
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
					break;
				case EDescriptorType::SAMPLED_IMAGE:
					break;
				case EDescriptorType::SAMPLER:
					break;
				case EDescriptorType::INPUT_ATTACHMENT:
					break;
				default:
					break;
				}
			}
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
