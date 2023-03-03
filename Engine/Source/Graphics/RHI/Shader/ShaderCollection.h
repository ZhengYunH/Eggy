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
		friend struct ShaderRenderResource;
	public:
		ShaderStageInstance(EShaderStage stage, const String& shaderPath, String entry);
		~ShaderStageInstance();

	public:
		const ShadingParameterTable* GetBatchTable(EShaderConstant esc) const;
		bool GetTextureSlot(const String& name, uint8& outSlot) const;
		bool GetSamplerSlot(const String& name, uint8& outSlot) const;
		uint8 GetMaxImageBinding() const { return _MaxImageBinding; }
		uint8 GetMaxSamplerBinding() const { return _MaxSamplerStateBinding; }

	public:
		List<EShaderConstant> GetReletedBatch() const;
		bool IsImageBinding(uint8 slot) const;
		bool IsSamplerBinding(uint8 slot) const;
		bool GetBatchSlot(EShaderConstant esc, uint8& outSlot) const;

	protected:
		void _ParseDescriptorInternel();
		void _ParseUniform(Map<uint8, SShaderDescriptorData>& uniforms);
		void _ParseImage(Map<uint8, SShaderDescriptorData>& images);
		void _ParseSampler(Map<uint8, SShaderDescriptorData>& samplers);

	public:
		struct ShaderRenderResource* _ShaderRenderResource{ nullptr };

	protected:
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
		uint8 GetMaxImageBinding() const;
		uint8 GetMaxSamplerBinding() const;

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

