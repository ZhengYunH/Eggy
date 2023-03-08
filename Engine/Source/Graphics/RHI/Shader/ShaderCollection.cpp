#include "ShaderCollection.h"
#include "Graphics/RHI/IShaderRenderResource.h"
#include "Core/System/InputSystem.h"


namespace Eggy
{
	ShaderStageInstance::ShaderStageInstance(EShaderStage stage, const String& shaderPath, String entry)
		: _Stage(stage)
		, _Entry(entry)
	{
		_ShaderPath = shaderPath + ".hlsl";
		_Reflection = ShaderReflectionFactory::Instance().GetReflection(_ShaderPath, _Entry, stage);
		_ParseDescriptorInternel();
		_CreateRenderResource();
	}

	ShaderStageInstance::~ShaderStageInstance()
	{
		_Reflection = nullptr;
		for (auto& pair : _BatchMap)
			SafeDestroy(pair.second);
	}

	const ShadingParameterTable* ShaderStageInstance::GetBatchTable(EShaderConstant esc) const
	{
		if (_BatchMap.contains(esc))
			return _BatchMap.at(esc);
		return nullptr;
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

	bool ShaderStageInstance::Reload()
	{
		auto modTime = FileSystem::GetFileModifyTime((FileSystem::Get()->GetShaderRoot() + _ShaderPath).ToString());
		if (_Stage == EShaderStage::PS && _CreateTime != modTime)
		{
			modTime = FileSystem::GetFileModifyTime((FileSystem::Get()->GetShaderRoot() + _ShaderPath).ToString());
			_CreateRenderResource();
			return true;
		}
		return false;
	}

	void ShaderStageInstance::_CreateRenderResource()
	{
		SafeDestroy(_ShaderRenderResource);
		delete _ShaderRenderResource;
		_ShaderRenderResource = new ShaderRenderResource(this);
		_CreateTime = FileSystem::GetFileModifyTime((FileSystem::Get()->GetShaderRoot() + _ShaderPath).ToString());
	}

	List<EShaderConstant> ShaderStageInstance::GetReletedBatch() const
	{
		List<EShaderConstant> ret;
		for (auto& pair : _BatchMap)
			ret.push_back(pair.first);
		return ret;
	}

	bool ShaderStageInstance::IsImageBinding(uint8 slot) const
{
		for (auto& pair : _Images)
		{
			if (pair.Binding == slot)
				return true;
		}
		return false;
	}

	bool ShaderStageInstance::IsSamplerBinding(uint8 slot) const
	{
		for (auto& pair : _SamplerStates)
		{
			if (pair.Binding == slot)
				return true;
		}
		return false;
	}

	bool ShaderStageInstance::GetBatchSlot(EShaderConstant esc, uint8& outSlot) const
	{
		if (_BatchSlot.contains(esc))
		{
			outSlot = _BatchSlot.at(esc);
			return true;
		}
		return false;
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

	static void InsertPrimitiveParameter(ShadingParameterTable* table, SBlockVariableTrait& trait)
	{
		switch (trait.Numeric.Type)
		{
		case SShaderNumericType::MATRIX:
			if (trait.Numeric.Block.Matrix.column_count == 3 || trait.Numeric.Block.Matrix.row_count == 3)
				table->AddParameter(trait.Name, EShaderParameterType::Matrix4x3);
			else
				table->AddParameter(trait.Name, EShaderParameterType::Matrix4x4);
			break;
		case SShaderNumericType::VECTOR:
			table->AddParameter(trait.Name, EShaderParameterType::Float, trait.Numeric.Block.Vector.component_count);
			break;
		case SShaderNumericType::SCALER:
			switch (trait.Numeric.PrimitiveType)
			{
			case SShaderPrimitiveType::Float:
				table->AddParameter(trait.Name, EShaderParameterType::Float, 1);
				break;
			case SShaderPrimitiveType::Integer:
				table->AddParameter(trait.Name, EShaderParameterType::Integer, 1);
				break;
			case SShaderPrimitiveType::Boolean:
				table->AddParameter(trait.Name, EShaderParameterType::Boolean, 1);
				break;
			default:
				Unimplement(0);
				break;
			}
			break;
		default:
			break;
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
				switch (member.VariableType)
				{
				case SShaderVariableType::Primitive:
					InsertPrimitiveParameter(_BatchMap[esc], member);
					break;
				case SShaderVariableType::Array:
				{
					ShaderParamaterStruct* structTable = dynamic_cast<ShaderParamaterStruct*>(_BatchMap[esc]->AddParameter(member.Name, EShaderParameterType::Struct, member.Array.ArrayCount, member.Size / member.Array.ArrayCount));
					HYBRID_CHECK(structTable);
					for (uint32 i = 0; i < member.Array.MemberCount; ++i)
						InsertPrimitiveParameter(structTable, member.Array.Members[i]);
					break;
				}
				default:
					Unimplement();
					break;
				}
			}
		}
	}

	void ShaderStageInstance::_ParseImage(Map<uint8, SShaderDescriptorData>& images)
	{
		_MaxImageBinding = 0;
		for (auto& binding2BindingData : images)
			_MaxImageBinding = Max(GetMaxImageBinding(), binding2BindingData.first + 1);

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

	void ShaderTechnique::Reload()
	{
		for (auto& stage2Instance : mStageInstances_)
			stage2Instance.second->Reload();
	}

	const ShadingParameterTable* ShaderTechnique::GetConstantTable(EShaderConstant esc) const
	{
		for (auto& pair : mStageInstances_)
		{
			if (auto batchTable = pair.second->GetBatchTable(esc))
				return batchTable;
		}
		return nullptr;
	}

	const ShaderStageInstance* ShaderTechnique::GetStageInstance(EShaderStage stage) const
	{
		if (!mStageInstances_.contains(stage))
			return nullptr;
		return mStageInstances_.at(stage);
	}

	bool ShaderTechnique::GetTextureSlot(const String& name, uint8& outSlot) const
	{
		for (auto& pair : mStageInstances_)
		{
			if (pair.second->GetTextureSlot(name, outSlot))
				return true;
		}
		return false;
	}

	bool ShaderTechnique::GetSamplerSlot(const String& name, uint8& outSlot) const
	{
		for (auto& pair : mStageInstances_)
		{
			if (pair.second->GetSamplerSlot(name, outSlot))
				return true;
		}
		return false;
	}

	uint8 ShaderTechnique::GetMaxImageBinding() const
	{
		uint8 maxIB = 0;
		for (auto& pair : mStageInstances_)
			maxIB = Max(maxIB, pair.second->GetMaxImageBinding());
		return maxIB;
	}

	uint8 ShaderTechnique::GetMaxSamplerBinding() const
	{
		uint8 maxSB = 0;
		for (auto& pair : mStageInstances_)
			maxSB = Max(maxSB, pair.second->GetMaxSamplerBinding());
		return maxSB;
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

	const ShaderTechnique* ShaderCollection::GetShaderTechnique(ETechnique technique) const
	{
		HYBRID_CHECK(IsTechniqueExist(technique));
		return mTechnique_.at(technique);
	}

	void ShaderCollection::Reload()
	{
		for (auto& pair : mTechnique_)
			pair.second->Reload();
	}

	ShaderCollectionFactory::ShaderCollectionFactory()
	{
		InputSystem::Get()->Event_CopyData.Bind([this](uint32 msg, void* data) {
			if (msg == CopyDataSignal::ShaderChange)
			{
				char* shaderName = (char*)data;
				this->_ReloadShader(shaderName);
			}
		});
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

	void ShaderCollectionFactory::_ReloadShader(const String& shaderPath)
	{
		if (!mCollections_.contains(shaderPath))
			return;
		mCollections_[shaderPath]->Reload();
	}

}

