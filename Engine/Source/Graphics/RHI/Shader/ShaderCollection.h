#pragma once
#include "Config.h"
#include "Graphics/RHI/IRenderHeader.h"
#include "Core/DataType/DesignPattern/DP_Factory.h"
#include "Graphics/RHI/Shader/ShaderParamter.h"
#include "ShaderReflection.h"


namespace Eggy
{
	class ShaderReflection;
	class ShaderTechnique
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

		struct StageData
		{
			String _Entry;
			ShaderReflection* _Reflection{ nullptr };
			Map<EShaderConstant, ShadingBatch*> _BatchMap;
			List<ShaderTextureInfo> _Images;
			List<ShaderSamplerState> _SamplerStates;
			uint8 _MaxImageBinding{ 0 };
			uint8 _MAxSamplerStateBinding{ 0 };
		};

		friend class ShaderCollection;
	public:
		ShaderTechnique() = default;
		ShaderTechnique(ETechnique techniquem, const String& shaderPath);
		virtual ~ShaderTechnique();

	protected:
		virtual void ParseReflectionImpl();
		void _ParseDescriptorInternel(StageData& stageData, ShaderReflection& reflection);
		void _ParseUniform(StageData& stageData, Map<uint8, SShaderDescriptorData>& uniforms);
		void _ParseImage(StageData& stageData, Map<uint8, SShaderDescriptorData>& images);
		void _ParseSampler(StageData& stageData, Map<uint8, SShaderDescriptorData>& samplers);

	protected:
		Map<EShaderType, StageData> mEntrys_;
		String mShaderPath_;
	};

	class ShaderCollection
	{
	public:
		ShaderCollection() = default;
		ShaderCollection(const String& shaderPath);
		~ShaderCollection();
		bool IsTechniqueExist(ETechnique technique);
		const ShaderTechnique* GetShaderTechnique(ETechnique technique);

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

