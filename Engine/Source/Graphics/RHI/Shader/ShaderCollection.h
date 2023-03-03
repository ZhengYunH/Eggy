#pragma once
#include "Config.h"
#include "Graphics/RHI/IRenderHeader.h"
#include "Core/DataType/DesignPattern/DP_Factory.h"
#include "ShaderParamter.h"
#include "ShaderReflection.h"


namespace Eggy
{
	struct ShaderTextureInfo
	{
		uint8 Binding;
		String Name;
		EFormat Format;
		ETextureType Type;
	};

	struct ShaderSamplerState
	{
		uint8 Binding;
		String Name;
	};

	struct ShaderRenderResource;
	class ShaderStageInstance
	{
	public:
		ShaderStageInstance(EShaderStage stage, const String& shaderPath, String entry);
		~ShaderStageInstance();

	public:
		bool GetTextureSlot(const String& name, uint8& outSlot) const;
		bool GetSamplerSlot(const String& name, uint8& outSlot) const;
		List<EShaderConstant> GetReletedBatch() const;

	protected:
		void _ParseDescriptorInternel();
		void _ParseUniform(Map<uint8, SShaderDescriptorData>& uniforms);
		void _ParseImage(Map<uint8, SShaderDescriptorData>& images);
		void _ParseSampler(Map<uint8, SShaderDescriptorData>& samplers);

	public:
		struct ShaderRenderResource* _ShaderRenderResource{ nullptr };

	public:
		EShaderStage _Stage;
		String _Entry;
		String _ShaderPath;
		ShaderReflection* _Reflection{ nullptr };
		Map<EShaderConstant, uint8> _BatchSlot;
		Map<EShaderConstant, ShadingParameterTable*> _BatchMap;
		List<ShaderTextureInfo> _Images;
		List<ShaderSamplerState> _SamplerStates;
		uint8 _MaxImageBinding{ 0 };
		uint8 _MaxSamplerStateBinding{ 0 };
	};


	class ShaderTechnique
	{
	public:
		ShaderTechnique() = default;
		ShaderTechnique(ETechnique technique, const String& shaderPath);
		virtual ~ShaderTechnique();

	public:
		void CreateDeviceResource(IRenderResourceFactory* factory);

	public:
		const ShadingParameterTable* GetConstantTable(EShaderConstant esc) const;
		const ShaderStageInstance* GetStageInstance(EShaderStage stage) const;
		bool GetTextureSlot(const String& name, uint8& outSlot) const;
		bool GetSamplerSlot(const String& name, uint8& outSlot) const;

	protected:
		Map<EShaderStage, ShaderStageInstance*> mStageInstances_;
	};

	class ShaderCollection
	{
	public:
		ShaderCollection() = default;
		ShaderCollection(const String& shaderPath);
		~ShaderCollection();
		bool IsTechniqueExist(ETechnique technique) const;
		const ShaderTechnique* GetShaderTechnique(ETechnique technique) const;

	protected:
		Map<ETechnique, ShaderTechnique*> mTechnique_;
	};

	class ShaderCollectionFactory : public TSingleton<ShaderCollectionFactory>
	{
		typedef TSimpleFactory<ShaderCollection> Factory;
	public:
		~ShaderCollectionFactory();
		const ShaderCollection* GetCollection(const String& shaderPath);

	protected:
		Map<String, ShaderCollection*> mCollections_;
	};
}

