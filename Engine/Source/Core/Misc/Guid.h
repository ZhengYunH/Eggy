#pragma once
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <istream>
#include <sstream>
#include "Core/Config.h"


namespace Eggy
{
	class Guid
	{
		typedef boost::uuids::uuid UUID;
	public:
		Guid() = default;
		Guid(const Guid& rhs) noexcept = default;
		Guid(Guid&& rhs) noexcept = default;
		explicit Guid(const String& str) noexcept
		{
			*this = FromString(str);
		}

		static constexpr uint32 GetStringSize() { return sizeof(uint8) * 32 + sizeof(char) * 3; }
		
		static Guid Generate()
		{
			UUID uid = boost::uuids::random_generator()();
			return *(Guid*)&uid;
		}
		
		static Guid FromString(const String& s)
		{
			HYBRID_CHECK(s.size() == GetStringSize());
			const char* rule = "XXXXXXXX-XXXXXXXX-XXXXXXXX-XXXXXXXX";
			Guid guid{};
			std::stringstream ss;
			for (size_t i = 0; i < s.size(); ++i)
			{
				if (rule[i] == 'X')
				{
					HYBRID_CHECK(isxdigit(s[i]));
					ss << s[i];
				}
				else
				{
					HYBRID_CHECK(s[i] == '-');
					ss >> std::hex >> guid.mU32[i / 8];
					ss.clear();
				}
			}
			return guid;
		}

		String ToString() const noexcept
		{
			std::stringstream ss;
			for (int i = 0; i < 4; ++i)
			{
				ss << std::setfill('0') << std::setw(8) << std::hex << mU32[i];
				if (i != 3)
					ss << '-';
			}
			return String(ss.str());
		}

		Guid& operator = (const Guid& rhs) noexcept = default;
		Guid& operator = (Guid&& rhs) noexcept = default;
		inline bool operator == (const Guid& rhs) const { return memcmp(mU32, rhs.mU32, sizeof(uint32) * 4) == 0; }
		inline bool operator != (const Guid& rhs) const { return *this == rhs; }

	protected:
		uint32 mU32[4];
	};

	namespace GuidTester
	{
		static void TestImportExport()
		{
			Guid generateGuid = Guid::Generate();
			String str = generateGuid.ToString();
			Guid guid1(str);
			Guid guid2 = Guid::FromString(str);

			DEBUG_CHECK(generateGuid == guid1);
			DEBUG_CHECK(generateGuid == guid2);
		}
	}
}

namespace std
{
	using namespace Eggy;
	template<> struct hash<Guid>
	{
		size_t operator()(Guid const& guid) const noexcept
		{
			return boost::uuids::hash_value(*(boost::uuids::uuid*)(&guid));
		}
	};
}