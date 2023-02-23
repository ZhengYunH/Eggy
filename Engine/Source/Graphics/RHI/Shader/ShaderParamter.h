#pragma once
#include "Core/Config.h"
#include "Core/Misc/Guid.h"
#include "Graphics/RHI/IRenderResource.h"


namespace Eggy
{
	enum class EShaderParameterType : uint8
	{
		Integer,
		Float,
		Boolean,
		Texture,
	};

	class IShaderParamter
	{
	public:
		IShaderParamter(uint16 blockOffset, uint16 arrayCount): 
			mBlockOffset(blockOffset), mArrayCount(arrayCount)
		{}

		const EShaderParameterType& GetType() { return mType; }
		const uint16& GetSize() { return mSize; }

		virtual bool GetInteger(const byte* block, uint16 offset, uint16 count, int* value) noexcept { return false; }
		virtual bool SetInteger(byte* block, uint16 offset, uint16 count, const int* value) noexcept { return false; }

		virtual bool GetFloat(const byte* block, uint16 offset, uint16 count, float* value) noexcept { return false; }
		virtual bool SetFloat(byte* block, uint16 offset, uint16 count, const float* value) noexcept { return false; }

		virtual bool GetBoolean(const byte* block, uint16 offset, uint16 count, bool* value) noexcept { return false; }
		virtual bool SetBoolean(byte* block, uint16 offset, uint16 count, const bool* value) noexcept { return false; }

		virtual bool GetTexture(const byte* block, Guid& texture) noexcept { return false; }
		virtual bool SetTexture(byte* block, const Guid& texture) noexcept { return false; }

	protected:
		// TODO: add allocator
		EShaderParameterType	mType;
		uint8					__spare__[1];
		uint16					mSize{ 0 };
		uint16					mArrayCount{ 0 };
		uint16					mBlockOffset{ 0 };
	};

	class ShaderParamaterInteger : public IShaderParamter
	{
	public:
		ShaderParamaterInteger(uint16 blockOffset, uint16 arrayCount);
		virtual bool GetInteger(const byte* block, uint16 offset, uint16 count, int* value) noexcept;
		virtual bool SetInteger(byte* block, uint16 offset, uint16 count, const int* value) noexcept;
	};

	class ShaderParamterFloat : public IShaderParamter
	{
	public:
		ShaderParamterFloat(uint16 blockOffset, uint16 arrayCount);
		virtual bool GetFloat(const byte* block, uint16 offset, uint16 count, float* value) noexcept;
		virtual bool SetFloat(byte* block, uint16 offset, uint16 count, const float* value) noexcept;
	};

	class ShadingBatch
	{
	public:
		bool SetInteget(const String& name, uint16 offset, uint16 count, const int* value) noexcept;
		bool SetFloat(const String& name, uint16 offset, uint16 count, const float* value) noexcept;
		bool SetBoolean(const String& name, uint16 offset, uint16 count, const bool* value) noexcept;

	protected:
		IShaderParamter* GetParameter(const String& name);

	protected:
		byte* mParamterBlock_;
		Map<String, IShaderParamter*> mParams_;
		bool mIsPropertyModified_{ true };
	};
}

