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

		const EShaderParameterType& GetType() const { return mType; }
		const uint16& GetSize() const { return mSize; }

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

		bool GetInteger(const byte* block, uint16 offset, uint16 count, int* value) noexcept override;
		bool SetInteger(byte* block, uint16 offset, uint16 count, const int* value) noexcept override;
	};

	class ShaderParameterFloat : public IShaderParamter
	{
	public:
		ShaderParameterFloat(uint16 blockOffset, uint16 arrayCount);

		bool GetFloat(const byte* block, uint16 offset, uint16 count, float* value) noexcept override;
		bool SetFloat(byte* block, uint16 offset, uint16 count, const float* value) noexcept override;
	};

	class ShaderParameterBoolean : public IShaderParamter
	{
	public:
		ShaderParameterBoolean(uint16 blockOffset, uint16 arrayCount);

		bool GetBoolean(const byte* block, uint16 offset, uint16 count, bool* value) noexcept override;
		bool SetBoolean(byte* block, uint16 offset, uint16 count, const bool* value) noexcept override;
	};

	class ShaderParameterTexture : public IShaderParamter
	{
	public:
		ShaderParameterTexture(uint16 blockOffset, uint16 arrayCount);

		bool GetTexture(const byte* block, Guid& texture) noexcept override;
		bool SetTexture(byte* block, const Guid& texture) noexcept override;
	};

	class ShadingBatch
	{
		constexpr static uint16 BLOCK_SIZE = 4096;
	public:
		ShadingBatch();
		~ShadingBatch();

		bool SetInteget(const String& name, uint16 offset, uint16 count, const int* value) noexcept;
		bool SetFloat(const String& name, uint16 offset, uint16 count, const float* value) noexcept;
		bool SetBoolean(const String& name, uint16 offset, uint16 count, const bool* value) noexcept;

	public:
		IShaderParamter* AddParameter(const String& name, EShaderParameterType type, uint16 arrayCount=1);
		void Clear();

	private:
		IShaderParamter* GetParameter(const String& name);
		void ExpandBlock();

	private:
		byte* mParameterBlock_{ nullptr };
		uint16 mParameterOffset_{ 0 };
		uint16 mBlockTotalSize_{ 0 };
		Map<String, IShaderParamter*> mParams_;
		bool mIsPropertyModified_{ true };
	};
}

