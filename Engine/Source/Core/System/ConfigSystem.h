#pragma once
#include "Core/Config.h"
#include "Core/Interface/ISystem.h"
#include "Core/Reflection/Reflection.h"

namespace Eggy
{
	struct ConfigItem
	{
		ConfigItem(const Type* type, void* instance)
			: Type(type), Instance(instance)
		{
		}

		~ConfigItem()
		{
			Type->Destruct(Instance);
			Instance = nullptr;
		}
		const Type* Type;
		void* Instance;
	};

	class Configuration
	{
	public:
		Configuration() = default;
		void Clear()
		{
			for (auto& pair : mItems_)
				delete pair.second;
		}
		~Configuration()
		{
			Clear();
		}
		Map<Name, ConfigItem*> mItems_;
	};

	class ConfigSystem : public TSystem<ConfigSystem>
	{
		DeclareSystem(ConfigSystem);

		static constexpr std::string_view GLOBAL_CONFIG = "__Global";
	public:
		Configuration* InsertConfiguration(const Name& config, bool bReplaceMode = false);

		template<typename _TType>
		_TType GetData(const Name& config, const Name& key, const _TType defaultValue)
		{
			auto pair = mConfigs_.find(config);
			if (pair == mConfigs_.end())
				return defaultValue;
			auto configuration = pair->second;
			auto pairItr = configuration->mItems_.find(key);
			if (pairItr == configuration->mItems_.end())
				return defaultValue;
			return *((_TType*)pairItr->second->Instance);
		}
		
		template<typename _TType>
		bool SetData(const Name& config, const Name& key, const _TType value)
		{
			auto pair = mConfigs_.find(config);
			if (pair == mConfigs_.end())
				return false;
			auto configuration = pair->second;;
			const Type* Type = GetType<_TType>();
			_TType* rvalue = static_cast<_TType*>(Type->Construct());
			*rvalue = value;
			configuration->mItems_[key] = new ConfigItem(Type, (void*)rvalue);
			return true;
		}

		template<typename _TType>
		const _TType GetGlobalConfig(const Name& key, const _TType defaultValue)
		{
			return GetData<_TType>(GLOBAL_CONFIG.data(), key, defaultValue);
		}

		template<typename _TType>
		bool SetGlobalConfig(const Name& key, const _TType value)
		{
			return SetData<_TType>(GLOBAL_CONFIG.data(), key, value);
		}

		void Initialize() override;
		void Finalize() override;

	protected:
		Configuration* GetConfiguration(const Name& key)
		{
			if (auto itr = mConfigs_.find(key); itr == mConfigs_.end())
				return itr->second;
			return nullptr;
		}

	protected:
		Map<Name, Configuration*> mConfigs_;
	};
}