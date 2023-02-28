#include "ShaderReflection.h"
#include "Core/File/File.h"
#include "spirv_reflect.cpp"


// can refer to SPIRV-Reflect/examples/main_descriptors.cpp
namespace Eggy
{
	void SShaderNumericTraits::FillIn(SpvOp opType, SpvReflectNumericTraits& spvNumberTrait)
	{
		Scalar.width = spvNumberTrait.scalar.width;
		Scalar.signedness = spvNumberTrait.scalar.signedness;

		switch (opType)
		{
		case SpvOpTypeBool:
		case SpvOpTypeInt:
		case SpvOpTypeFloat:
			Type = SShaderInputNumericType::SCALER;
			break;
		case SpvOpTypeVector:
			Type = SShaderInputNumericType::VECTOR;
			Block.Vector.component_count = spvNumberTrait.vector.component_count;
			break;
		case SpvOpTypeMatrix:
			Type = SShaderInputNumericType::MATRIX;
			Block.Matrix.row_count = spvNumberTrait.matrix.row_count;
			Block.Matrix.column_count = spvNumberTrait.matrix.column_count;
			Block.Matrix.stride = spvNumberTrait.matrix.stride;
			break;
		default:
			Unimplement(0);
			break;
		}
	}

	void SBlockVariableTrait::FillIn(SpvReflectBlockVariable* spvBlockVariable)
	{
		Name = spvBlockVariable->name;
		Offset = spvBlockVariable->offset;
		Size = spvBlockVariable->size;
		PaddingSize = spvBlockVariable->offset;
		Numeric.FillIn(spvBlockVariable->type_description->op, spvBlockVariable->numeric);
	}

	void SSampledImageTrait::FindIn(SpvReflectImageTraits* spvImageTraits)
	{
		using enum EFormat;
		switch (spvImageTraits->image_format)
		{
		case SpvImageFormatUnknown:
			Format = UNDEFINED;
			break;
		case SpvImageFormatRgba32f:
			Format = A32R32G32B32F;
			break;
		case SpvImageFormatRgba8:
			Format = R8G8B8A8;
			break;
		case SpvImageFormatR8ui:
			Format = EFormat::R8_UINT;
			break;
		case SpvImageFormatR32f:
			Format = R32_UINT;
			break;
		default:
			Unimplement();
			break;
		}

		switch (spvImageTraits->dim)
		{
		case SpvDim2D:
			Type = ETextureType::Texture2D;
			break;
		case SpvDim3D:
			Type = ETextureType::Texture3D;
			break;
		case SpvDimCube:
			Type = ETextureType::TextureCube;
			break;
		default:
			Unimplement();
			break;
		}
	}

	ShaderReflection* ShaderReflectionFactory::GetReflection(const String& shaderPath, const String& entry, EShaderType shaderType)
	{
		EShaderLanguage sl = GetShaderLanguage(shaderPath);
		ShaderReflection* reflect = nullptr;
		switch (sl)
		{
		case EShaderLanguage::Default:
			reflect = new ShaderReflectionSpirv(shaderPath, entry, shaderType);
			break;
		case EShaderLanguage::HLSL:
			reflect = new ShaderReflectionHLSL(shaderPath, entry, shaderType);
			break;
		case EShaderLanguage::GLSL:
			reflect = new ShaderReflectionGLSL(shaderPath, entry, shaderType);
			break;
		default:
			Unimplement();
		}
		reflect->Prepare();
		return reflect;
	}

	EShaderLanguage ShaderReflectionFactory::GetShaderLanguage(const String& shaderPath)
	{
		FPath path(shaderPath);
		String postFix = path.getPostFix();
		if (postFix == "hlsl")
			return EShaderLanguage::HLSL;
		else if (postFix == "glsl")
			return EShaderLanguage::GLSL;
		else
			return EShaderLanguage::Default;
	}

	ShaderReflection::~ShaderReflection()
	{
		if (IsResourceReady())
			spvReflectDestroyShaderModule(&mModule_);
	}

	void ShaderReflection::_PrepareReflectModule(const void* spirvCode, size_t spirvCode_nByte)
	{
		SpvReflectResult result;
		result = spvReflectCreateShaderModule(spirvCode_nByte, spirvCode, &mModule_);
		HYBRID_CHECK(result == SPV_REFLECT_RESULT_SUCCESS);
	}

	void ShaderReflectionSpirv::Prepare()
	{
		PrepareSpirvFilePath();
		auto file = FileSystem::Get()->LoadFile(mSpirvFilePath_);
		size_t nSpirvCode = 0;
		file->Read(nullptr, nSpirvCode);
		byte* spirvCode = new byte[nSpirvCode];
		file->Read(spirvCode, nSpirvCode);
		HYBRID_CHECK(nSpirvCode);

		_PrepareReflectModule(spirvCode, nSpirvCode);
		{
			_GenerateInputVariable();
			_GenerateDescriptor();
		}
		mIsResourceReady_.store(true);
	}

	void ShaderReflectionSpirv::PrepareSpirvFilePath()
	{
		mSpirvFilePath_ = (FileSystem::Get()->GetShaderRoot() + mInputFilePath_).ToString();
	}

	void ShaderReflectionHLSL::PrepareSpirvFilePath()
	{
		// convert hlsl -> spirv
		FPath shaderConductorEXE = FileSystem::Get()->GetToolRoot() + "ShaderConductor/ShaderConductorCmd.exe";
		FPath hlslShaderFile = FileSystem::Get()->GetShaderRoot() + mInputFilePath_;
		FPath spirvFilePath = hlslShaderFile;
		spirvFilePath.replaceFilePostfix("spirv.cache");

		std::stringstream ss;
		ss << shaderConductorEXE.ToString();
		ss << " -I " << hlslShaderFile.ToString();
		ss << " -S " << ESHADER_TYPE_STR[uint8(mShaderType_)];
		ss << " -E " << mEntry_;
		ss << " -T spirv";
		ss << " -O " << spirvFilePath.ToString();
		String s = ss.str().c_str();
		int result = system(ss.str().c_str());
		HYBRID_CHECK(result == 0, "Compile Error");

		mSpirvFilePath_ = spirvFilePath.ToString();
	}

	void ShaderReflectionGLSL::PrepareSpirvFilePath()
	{
		// convert glsl -> spirv
		Unimplement();
	}

	void ShaderReflection::_GenerateInputVariable()
	{
		HYBRID_CHECK(mInputVariable_.empty());

		SpvReflectResult result;

		uint32 varCount = 0;
		result = spvReflectEnumerateInputVariables(&mModule_, &varCount, NULL);
		HYBRID_CHECK(result == SPV_REFLECT_RESULT_SUCCESS);

		SpvReflectInterfaceVariable** inputVars = (SpvReflectInterfaceVariable**)malloc(varCount * sizeof(SpvReflectInterfaceVariable*));
		result = spvReflectEnumerateInputVariables(&mModule_, &varCount, inputVars);
		HYBRID_CHECK(result == SPV_REFLECT_RESULT_SUCCESS);

		mInputVariable_.resize(varCount);
		for (size_t i = 0; i < varCount; ++i)
		{
			SpvReflectInterfaceVariable* inputVar = inputVars[i];
			if(inputVar->name)
				mInputVariable_[i].Name = inputVar->name;
			if(inputVar->location != ~(uint32)0)
				mInputVariable_[i].Location = inputVar->location;
			if (inputVar->semantic)
				mInputVariable_[i].Semantic = inputVar->semantic;

			mInputVariable_[i].Numeric.FillIn(inputVar->type_description->op, inputVar->numeric);
		}
	}

	void ShaderReflection::_GenerateDescriptor()
	{
		SpvReflectResult result;

		uint32 descSetCount = 0;
		result = spvReflectEnumerateDescriptorSets(&mModule_, &descSetCount, NULL);
		HYBRID_CHECK(result == SPV_REFLECT_RESULT_SUCCESS);
		SpvReflectDescriptorSet** inputSets = (SpvReflectDescriptorSet**)malloc(descSetCount * sizeof(SpvReflectDescriptorSet*));
		result = spvReflectEnumerateDescriptorSets(&mModule_, &descSetCount, inputSets);
		HYBRID_CHECK(result == SPV_REFLECT_RESULT_SUCCESS);

		for (size_t i = 0; i < descSetCount; ++i)
		{
			SpvReflectDescriptorSet* inputSet = inputSets[i];
			mDescriptor_[inputSet->set] = std::unordered_map<uint32, SShaderDescriptorData>();
			auto& descs = mDescriptor_[inputSet->set];
			for (size_t j = 0; j < inputSet->binding_count; ++j)
			{
				SpvReflectDescriptorBinding* inDescBinding = inputSet->bindings[j];
				SShaderDescriptorData& desc = descs[inDescBinding->binding] = SShaderDescriptorData();
				desc.Name = inDescBinding->name;

				switch (inDescBinding->descriptor_type)
				{
				case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLER:
				case SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
					desc.Type = EDescriptorType::SAMPLER;
					break;
				case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
					desc.Type = EDescriptorType::UNIFORM_BUFFER;
					desc.Uniform.Size = inDescBinding->block.size;
					desc.Uniform.MemberCount = inDescBinding->block.member_count;
					desc.Uniform.Members = new SBlockVariableTrait[desc.Uniform.MemberCount];
					for (uint32 i = 0; i < inDescBinding->block.member_count; ++i)
						desc.Uniform.Members[i].FillIn(&inDescBinding->block.members[i]);
					break;
				case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
					desc.Type = EDescriptorType::SAMPLED_IMAGE;
					desc.SampledImage.FindIn(&inDescBinding->image);
					break;
				default:
					Unimplement(0);
					break;
				}
			}
		}
	}
}

