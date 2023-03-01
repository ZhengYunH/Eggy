#pragma once
#include "Core/Config.h"
#include "Core/Misc/Guid.h"
#include "Core/Math/Matrix4x3.h"
#include "Core/Math/Matrix4x4.h"
#include "Graphics/RHI/IRenderResource.h"


namespace Eggy
{
	enum class EShaderParameterType : uint8
	{
		Integer,
		Float,
		Boolean,
		Matrix4x3,
		Matrix4x4,
		Texture
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

		virtual bool GetMatrix4x3(const byte* block, Matrix4x3& matrix) noexcept { return false; }
		virtual bool SetMatrix4x3(byte* block, const Matrix4x3& matrix) noexcept { return false; }

		virtual bool GetMatrix4x4(const byte* block, Matrix4x4& matrix) noexcept { return false; }
		virtual bool SetMatrix4x4(byte* block, const Matrix4x4& matrix) noexcept { return false; }

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
		ShaderParameterTexture(uint16 blockOffset);

		bool GetTexture(const byte* block, Guid& texture) noexcept override;
		bool SetTexture(byte* block, const Guid& texture) noexcept override;
		
		void SetFormat(EFormat format) { Format = format; }
		EFormat GetFormat() const { return Format; }

		void SetType(ETextureType type) { Type = type; }
		ETextureType GetType() const { return Type; }

	protected:
		EFormat			Format{ EFormat::UNDEFINED };
		ETextureType	Type{ ETextureType::UNDEFINE };
		uint8			__spare0__[1];
	};

	class ShaderParameterMatrix4x3 : public IShaderParamter
	{
	public:
		ShaderParameterMatrix4x3(uint16 blockOffset);

		bool GetMatrix4x3(const byte* block, Matrix4x3& matrix) noexcept override;
		bool SetMatrix4x3(byte* block, const Matrix4x3& matrix) noexcept override;
	};

	class ShaderParameterMatrix4x4 : public IShaderParamter
	{
	public:
		ShaderParameterMatrix4x4(uint16 blockOffset);

		bool GetMatrix4x4(const byte* block, Matrix4x4& matrix) noexcept override;
		bool SetMatrix4x4(byte* block, const Matrix4x4& matrix) noexcept override;
	};


	class ShadingBatch
	{
	public:
		ShadingBatch() = default;
		~ShadingBatch();

	public:
		IShaderParamter* AddParameter(const String& name, EShaderParameterType type, uint16 arrayCount=1);
		IShaderParamter* GetParameter(const String& name);
		uint16 GetParameterSize() { return mParameterOffset_; }
		void Clear();

	private:
		uint16 mParameterOffset_{ 0 };
		Map<String, IShaderParamter*> mParams_;
	};

	class ShadingParamterCollection
	{
		constexpr static uint16 BLOCK_SIZE = 4096;
	public:
		ShadingParamterCollection(ShadingBatch* batch)
			: mBatch_(batch)
		{
			if (mBatch_->GetParameterSize() < 0)
				mBlockAllocationSize_ = BLOCK_SIZE;
			else
				mBlockAllocationSize_ = mBatch_->GetParameterSize();
			mBlockTotalSize_ = mBlockAllocationSize_;
			mParameterBlock_ = new byte[mBlockTotalSize_];
			memset(mParameterBlock_, 0, mBlockTotalSize_);
		}

		bool SetInteget(const String& name, uint16 offset, uint16 count, const int* value) noexcept;
		bool SetFloat(const String& name, uint16 offset, uint16 count, const float* value) noexcept;
		bool SetBoolean(const String& name, uint16 offset, uint16 count, const bool* value) noexcept;

		IShaderParamter* GetParameter(const String& name);

	protected:
		void ExpandBlock() noexcept;

	private:
		ShadingBatch* mBatch_{ nullptr };
		ShadingBatch* mCustomBatch_{ nullptr };
		byte* mParameterBlock_{ nullptr };
		uint16 mBlockTotalSize_{ 0 };
		uint16 mBlockAllocationSize_{ 0 };
		bool mIsPropertyModified_{ true };
	};
}

