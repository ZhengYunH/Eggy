#include "Core/Config.h"
#include "Core/Interface/ISystem.h"
#include "Core/Reflection/Reflection.h"

namespace Eggy
{
	struct ConfigItem
	{
		const Type* Type;
		void* Instance;
	};

	class Configuration
	{

	};

	class ConfigSystem : public ISystem<ConfigSystem>
	{
		DeclareSystem(ConfigSystem);
	public:
		ConfigSystem() { HYBRID_CHECK(!GInstance); GInstance = this; }

	public:
		template<typename _TType>
		_TType GetData(const Name& config, const Name& key);

		template<typename _TType>
		_TType GetGlobalConfig(const Name& key, _TType defaultValue)
		{
			auto pairItr = mGlobalConfig_.find(key);
			if (pairItr == mGlobalConfig_.end())
				return defaultValue;
			_TType ret = *((_TType*)pairItr->second->Instance);
			return ret;
		}

		template<typename _TType>
		void SetGlobalConfig(const Name& key, _TType value)
		{
			const Type* Type = Type::GetDefaultObject(); //GetType<_TType>();
			_TType* rvalue = new _TType();
			*rvalue = value;
			mGlobalConfig_[key] = new ConfigItem(Type, (void*)rvalue);
		}

		void Initialize() override;
		void Finalize() override;

	protected:
		List<Configuration*> mConfigs_;
		Map<Name, ConfigItem*> mGlobalConfig_;
	};

	template<typename _TType>
	_TType Eggy::ConfigSystem::GetData(const Name& config, const Name& key)
	{

	}

}