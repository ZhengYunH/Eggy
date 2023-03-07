#pragma once
#include "Core/Config.h"
#include "Graphics/RHI/IRenderResource.h"
#include "Core/System/FileSystem.h"
#include "Core/Engine/Event/Event.h"
#include "Core/DataType/DesignPattern/DP_Singleton.h"
#include "spirv_reflect.h"


namespace Eggy
{
	enum class SShaderNumericType
	{
		SCALER = 0,
		VECTOR = 1,
		MATRIX = 2,
	};

	enum class SShaderVariableType
	{
		Primitive = 0, // scaler, vector, matrix
		Struct = 1,
		Array = 2,
	};

	enum class SShaderPrimitiveType
	{
		Default,
		Boolean,
		Float,
		Integer
	};

	struct SShaderNumericTraits
	{
		void FillIn(SpvOp opType, SpvReflectNumericTraits& spvNumberTrait);

		struct Scalar {
			uint32                        width;
			uint32                        signedness;
		} Scalar;

		union
		{
			struct Vector {
				uint32                        component_count;
			} Vector;

			struct Matrix {
				uint32                        column_count;
				uint32                        row_count;
				uint32                        stride; // Measured in bytes
			} Matrix;
		} Block;

		SShaderNumericType Type;
		SShaderPrimitiveType PrimitiveType;
	};

	struct SShaderInputVariableData
	{
		std::string Name;
		uint32 Location;
		std::string Semantic;
		SShaderNumericTraits Numeric;
	};

	enum class EDescriptorType
	{
		SAMPLER = 0,
		SAMPLED_IMAGE,
		UNIFORM_BUFFER,
		INPUT_ATTACHMENT,

	};

	struct SBlockVariableTrait
	{
		void FillIn(SpvReflectBlockVariable* spvBlockVariable);
		String Name;
		uint32 Offset;
		uint32 Size;
		uint32 PaddingSize;
		SShaderVariableType VariableType;
		union
		{
			struct Array
			{
				uint32 ArrayCount;
				SBlockVariableTrait* Members;
				uint32 MemberCount;
			} Array;
			SShaderNumericTraits Numeric;
		};
	};

	struct SShaderUniformTrait
	{
		uint32 Size;
		uint32 MemberCount;
		SBlockVariableTrait* Members;
	};

	struct SShaderSamplerTrait
	{
	};

	struct SSampledImageTrait
	{
		void FindIn(SpvReflectImageTraits* spvImageTraits);
		EFormat Format;
		ETextureType Type;
	};

	struct SShaderDescriptorData
	{
		String Name;
		EDescriptorType Type;
		union
		{
			SShaderUniformTrait Uniform;
			SShaderSamplerTrait	Sampler;
			SSampledImageTrait	SampledImage;
		};
	};

	enum class EShaderLanguage
	{
		Default = 0,
		HLSL = 1,
		GLSL = 2
	};

	class ShaderReflection
	{
	public:
		ShaderReflection(const String& inputFilePath, const String& entry, EShaderStage shaderStage) 
			: mInputFilePath_(inputFilePath) 
			, mEntry_(entry)
			, mShaderStage_(shaderStage)
		{}
		virtual ~ShaderReflection();

		const List<SShaderInputVariableData>& GetInputVariable()
		{
			return mInputVariable_;
		}

		const Map<uint32/*set*/, Map<EDescriptorType, Map<uint8/*binding*/, SShaderDescriptorData>>>& GetDescriptor()
		{
			return mDescriptor_;
		}

		bool IsResourceReady() { return mIsResourceReady_.load(); }

	public:
		virtual void Prepare() = 0;
		
	protected:
		void _PrepareReflectModule(const void* spirvCode, size_t spirvCode_nByte);
		void _GenerateInputVariable();
		void _GenerateDescriptor();

	protected:
		String mInputFilePath_;
		String mEntry_;
		EShaderStage mShaderStage_;

		List<SShaderInputVariableData> mInputVariable_;
		Map<uint32/*set*/, Map<EDescriptorType, Map<uint8/*binding*/, SShaderDescriptorData>>> mDescriptor_;
		SpvReflectShaderModule mModule_;
		std::atomic<bool> mIsResourceReady_{ false };
	};

	class ShaderReflectionFactory : public TSingleton<ShaderReflectionFactory>
	{
	public:
		ShaderReflection* GetReflection(const String& shaderPath, const String& entry, EShaderStage shaderStage);
		EShaderLanguage GetShaderLanguage(const String& shaderPath);

	private:
		Map<String, Event<void>> mShaderOverridedEvents_;
	};


	class ShaderReflectionSpirv : public ShaderReflection
	{
	public:
		ShaderReflectionSpirv(const String& inputFilePath, const String& entry, EShaderStage shaderType)
			: ShaderReflection(inputFilePath, entry, shaderType)
		{}
	protected:
		virtual void Prepare() override;
		virtual void PrepareSpirvFilePath();
	protected:
		void* mSpirvCode_{ nullptr };
		size_t mSpirvCode_nByte{ 0 };
		String mSpirvFilePath_{ 0 };
	};

	class ShaderReflectionHLSL : public ShaderReflectionSpirv
	{
	public:
		ShaderReflectionHLSL(const String& inputFilePath, const String& entry, EShaderStage shaderStage)
			: ShaderReflectionSpirv(inputFilePath, entry, shaderStage) {}
	protected:
		void PrepareSpirvFilePath() override;
	};

	class ShaderReflectionGLSL : public ShaderReflectionSpirv
	{
	public:
		ShaderReflectionGLSL(const String& inputFilePath, const String& entry, EShaderStage shaderStage) 
			: ShaderReflectionSpirv(inputFilePath, entry, shaderStage) {}
	protected:
		void PrepareSpirvFilePath() override;
	};
}