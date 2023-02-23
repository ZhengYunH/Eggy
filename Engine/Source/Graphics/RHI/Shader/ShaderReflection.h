#pragma once
#include "Core/Config.h"
#include "Graphics/RHI/IRenderResource.h"
#include "Core/System/FileSystem.h"
#include "Core/Engine/Event/Event.h"
#include "spirv_reflect.h"

namespace Eggy
{
	template<typename TImpl>
	class TSingleton
	{
	private:
		TSingleton() = default;

	public:
		static TImpl& GetInstance()
		{
			static TImpl instance;
			return instance;
		}
	};

	enum class SShaderInputNumericType
	{
		SCALER = 0,
		VECTOR = 1,
		MATRIX = 2,
	};

	struct SShaderNumericTraits
	{
		struct Scalar {
			uint32_t                        width;
			uint32_t                        signedness;
		} Scalar;

		union
		{
			struct Vector {
				uint32_t                        component_count;
			} Vector;

			struct Matrix {
				uint32_t                        column_count;
				uint32_t                        row_count;
				uint32_t                        stride; // Measured in bytes
			} Matrix;
		} Block;

		SShaderInputNumericType Type;
	};

	struct SShaderInputVariableData
	{
		std::string Name;
		uint32_t Location;
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

	struct SShaderUniformTrait
	{
		uint32_t Size;
	};

	struct SShaderSamplerTrait
	{

	};

	struct SSampledImageTrait
	{
		EFormat Format;
	};

	struct SShaderDescriptorData
	{
		std::string Name;
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
		ShaderReflection(const String& inputFilePath) : mInputFilePath_(inputFilePath) {}
		virtual ~ShaderReflection();

		const std::vector<SShaderInputVariableData>& GetInputVariable()
		{
			return mInputVariable_;
		}

		const std::unordered_map<uint32_t, std::unordered_map<uint32_t, SShaderDescriptorData>>& GetDescriptor()
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
		String	mInputFilePath_;
		std::vector<SShaderInputVariableData> mInputVariable_;
		std::unordered_map<uint32_t/*set*/, std::unordered_map<uint32_t/* binding*/, SShaderDescriptorData>> mDescriptor_;
		SpvReflectShaderModule mModule_;
		std::atomic<bool> mIsResourceReady_{ false };
	};

	class ShaderReflectionFactory : TSingleton<ShaderReflectionFactory>
	{
	public:
		ShaderReflection* GetReflection(const String& shaderPath);
		EShaderLanguage GetShaderLanguage(const String& shaderPath);

	private:
		Map<String, Event<void>> mShaderOverridedEvents_;
	};


	class ShaderReflectionSpirv : public ShaderReflection
	{
	public:
		ShaderReflectionSpirv(const String& inputFilePath) : ShaderReflection(inputFilePath) {}
	protected:
		void Prepare() override;
		virtual void PrepareSpirvFilePath();
	protected:
		void* mSpirvCode_{ nullptr };
		size_t mSpirvCode_nByte{ 0 };
		String mSpirvFilePath_{ 0 };
	};

	class ShaderReflectionHLSL : public ShaderReflectionSpirv
	{
	public:
		ShaderReflectionHLSL(const String& inputFilePath) : ShaderReflectionSpirv(inputFilePath) {}
	protected:
		void PrepareSpirvFilePath() override;
	};

	class ShaderReflectionGLSL : public ShaderReflectionSpirv
	{
	public:
		ShaderReflectionGLSL(const String& inputFilePath) : ShaderReflectionSpirv(inputFilePath) {}
	protected:
		void PrepareSpirvFilePath() override;
	};
}