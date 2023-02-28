#pragma once
#include "Config.h"
#include "Graphics/RHI/IRenderHeader.h"
#include "Core/DataType/DesignPattern/DP_Factory.h"
#include "Graphics/RHI/Shader/ShaderParamter.h"


namespace Eggy
{
	class ShaderReflection;
	class ShaderTechnique
	{
		struct StageData
		{
			String _Entry;
			ShaderReflection* _Reflection;
			Map<EShaderConstant, ShadingBatch*> _BatchMap;
			SamplerState _SamplerState;
		};

		friend class ShaderCollection;
	public:
		ShaderTechnique() = default;
		ShaderTechnique(ETechnique techniquem, const String& shaderPath);
		virtual ~ShaderTechnique();

	protected:
		virtual void ParseReflectionImpl(const String& shaderPath);
		void _ParseDescriptorInternel(const String& shaderPath, StageData& stageData, ShaderReflection& reflection);

	protected:
		Map<EShaderType, StageData> mEntrys_;
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

