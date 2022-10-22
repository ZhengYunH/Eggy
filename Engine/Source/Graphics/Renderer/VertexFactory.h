#pragma once
#include "Core/Config.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Vector2.h"

#include <functional>


namespace Eggy
{
	struct ARGB
	{
		union{
			struct
			{
				byte b, g, r, a;
			};
			uint32 v = 0;
		};

		constexpr FORCEINLINE uint32& Argb() noexcept
		{
			return v;
		}

		constexpr FORCEINLINE const uint32& Argb() const noexcept
		{
			return v;
		}

		constexpr FORCEINLINE explicit ARGB(const Vector3& inColor, float alpha = 1.0f) noexcept
			: b{ static_cast<byte>(MapUnitToRange(inColor.x, 255.f)) }
			, g{ static_cast<byte>(MapUnitToRange(inColor.y, 255.f)) }
			, r{ static_cast<byte>(MapUnitToRange(inColor.z, 255.f)) }
			, a{ static_cast<byte>(MapUnitToRange(alpha, 255.f)) }
		{
		}
	};

	struct Color4B
	{
		union {
			uint32		u32;
			uint8		u8[4];
		};

		Color4B() { u32 = 0; }

		Color4B(Vector3 v)
		{
			FromVector3(v);
		}

		FORCEINLINE void FromVector3(Vector3 v) noexcept
		{
			v = (v + 1.0f) * 0.5f;
			u32 = ARGB(v, 0.f).Argb();
		}

		FORCEINLINE bool operator == (const Color4B& rhs) const noexcept
		{
			return u32 == rhs.u32;
		}
	};

	enum EVertexFormat : uint16
	{
		EVF_None = 0,
		// Geometric vertex.
		EVF_P3F = 1,					// Position(3F)
		EVF_P3F_C4B_T2F = 2,			// Position(3F), Color(4B), TexCoord(2F)
		EVF_P3F_N4B_T2F = 3				// Position(3F), Normal(4B), TexCoord(2F)
	};

	struct VF_P3F_N4B_T2F
	{
		static constexpr EVertexFormat FORMAT = EVertexFormat::EVF_P3F_N4B_T2F;
	};

	template<int _VertexFormat> class TVertexType {};

	template<>
	class TVertexType<EVF_P3F_N4B_T2F>
	{
	public:
		Vector3	Position;
		Color4B	Normal;
		Vector2 ST;
		
		constexpr FORCEINLINE bool operator ==(const TVertexType<EVF_P3F_N4B_T2F>& rhs) const noexcept
		{
			return this->Position == rhs.Position && this->Normal == rhs.Normal && this->ST == rhs.ST;
		}

		static constexpr EVertexFormat FORMAT = EVertexFormat::EVF_P3F_N4B_T2F;
	};

	struct VertexInfo
	{
		uint16 DataSize{ 0 };
	};

	static Map<EVertexFormat, VertexInfo> GVertexInfoMap = {
		{ EVF_P3F_N4B_T2F, {sizeof(Vector3) + sizeof(Color4B) + sizeof(Vector2) }}
	};

	class VertexFactory
	{
	public:
		static size_t GetDataTypeSize(EVertexFormat format) 
		{
			HYBRID_CHECK(GVertexInfoMap.find(format) != GVertexInfoMap.end());
			return GVertexInfoMap[format].DataSize;
		}
	};
}


namespace std
{
	using namespace Eggy;
	template<> struct hash<Color4B>
	{
		size_t operator()(Color4B const& color) const noexcept
		{
			return boost::hash_value(color.u32);
		}
	};

	template<> struct hash<Vector3>
	{
		size_t operator()(Vector3 const& vec3) const noexcept
		{
			return vec3.Hash();
		}
	};

	template<> struct hash<Vector2>
	{
		size_t operator()(Vector2 const& vec2) const noexcept
		{
			return vec2.Hash();
		}
	};

	template<> struct hash<TVertexType<EVF_P3F_N4B_T2F>>
	{
		size_t operator()(TVertexType<EVF_P3F_N4B_T2F> const& vertexType) const noexcept
		{
			size_t v = boost::hash_value(hash<Vector3>()(vertexType.Position));
			boost::hash_combine(v, hash<Color4B>()(vertexType.Normal));
			boost::hash_combine(v, hash<Vector2>()(vertexType.ST));
			return v;
		}
	};
}

