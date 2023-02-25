#pragma once
#include "Core/Config.h"
#include "Core/Misc/Guid.h"
#include "Core/DataType/SharedObject.h"
#include "Core/DataType/DesignPattern/DP_Factory.h"


namespace Eggy
{
	enum class EResourceType
	{
		None,
		Model,
		Texture,
		Material,
		Mesh,
		Shader,
		End
	};

	// TODO: will be removed by reflection
	static Map<String, EResourceType> s_ResourceTypeMap = {
		{ "Model", EResourceType::Model },
		{ "Texture", EResourceType::Texture },
		{ "Material", EResourceType::Material },
		{ "Mesh", EResourceType::Mesh },
		{ "Shader", EResourceType::Shader },
		{ "End", EResourceType::End },
	};

	typedef std::function<void()> TLoadedCallback;
}

