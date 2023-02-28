#include "ShaderParamter.h"


namespace Eggy
{
	IShaderParamter* ShadingBatch::GetParameter(const String& name)
	{
		if (auto itr = mParams_.find(name); itr != mParams_.end())
		{
			return itr->second;
		}
		return nullptr;
	}

	IShaderParamter* ShadingBatch::AddParameter(const String& name, EShaderParameterType type, uint16 arrayCount/*=1*/)
	{
		IShaderParamter* param = GetParameter(name);
		HYBRID_CHECK(!param, "Duplicate ShaderParmeter: ", name);
		switch (type)
		{
		case EShaderParameterType::Integer:
			param = new ShaderParamaterInteger(mParameterOffset_, arrayCount);
			break;
		case EShaderParameterType::Float:
			param = new ShaderParameterFloat(mParameterOffset_, arrayCount);
			break;
		case EShaderParameterType::Boolean:
			param = new ShaderParameterBoolean(mParameterOffset_, arrayCount);
			break;
		case EShaderParameterType::Texture:
			param = new ShaderParameterTexture(mParameterOffset_, arrayCount);
			break;
		default:
			Unimplement(0);
			break;
		}
		mParameterOffset_ += param->GetSize();
		if (mParameterOffset_ > mBlockTotalSize_)
			ExpandBlock();
		mParams_[name] = param;
		return param;
	}

	void ShadingBatch::Clear()
	{
		for (auto& pair : mParams_)
		{
			delete pair.second;
		}
		mParams_.clear();
		mParameterOffset_ = 0;
	}

	ShadingBatch::ShadingBatch(int predictSize)
	{
		if (predictSize < 0)
			mBlockAllocationSize_ = BLOCK_SIZE;
		else
			mBlockAllocationSize_ = mBlockTotalSize_;
		mBlockTotalSize_ = mBlockAllocationSize_;
		mParameterBlock_ = new byte[mBlockTotalSize_];
	}

	ShadingBatch::~ShadingBatch()
	{
		Clear();
		delete[] mParameterBlock_;
	}

	void ShadingBatch::ExpandBlock()
	{
		byte* _alloc = new byte[mBlockTotalSize_ + mBlockAllocationSize_];
		memcpy(_alloc, mParameterBlock_, mBlockTotalSize_);
		delete[] mParameterBlock_;
		mParameterBlock_ = _alloc;
		mBlockTotalSize_ = mBlockTotalSize_ + mBlockAllocationSize_;
	}

	bool ShadingBatch::SetInteget(const String& name, uint16 offset, uint16 count, const int* value) noexcept
	{
		if (auto param = GetParameter(name))
			return param->SetInteger(mParameterBlock_, offset, count, value);
		return false;
	}

	bool ShadingBatch::SetFloat(const String& name, uint16 offset, uint16 count, const float* value) noexcept
	{
		if (auto param = GetParameter(name))
			return param->SetFloat(mParameterBlock_, offset, count, value);
		return false;
	}

	bool ShadingBatch::SetBoolean(const String& name, uint16 offset, uint16 count, const bool* value) noexcept
	{
		if (auto param = GetParameter(name))
			return param->SetBoolean(mParameterBlock_, offset, count, value);
		return false;
	}

	ShaderParamaterInteger::ShaderParamaterInteger(uint16 blockOffset, uint16 arrayCount)
		: IShaderParamter(blockOffset, arrayCount)
	{
		mType = EShaderParameterType::Integer;
		mSize = sizeof(int) * arrayCount;
	}

	bool ShaderParamaterInteger::GetInteger(const byte* block, uint16 offset, uint16 count, int* value) noexcept
	{
		if (count + offset > mArrayCount)
			return false;
		memcpy(value, block + mBlockOffset, sizeof(int) * count);
		return true;
	}

	bool ShaderParamaterInteger::SetInteger(byte* block, uint16 offset, uint16 count, const int* value) noexcept
	{
		if (count + offset > mArrayCount)
			return false;
		memcpy(block + mBlockOffset, value, sizeof(int) * count);
		return true;
	}

	ShaderParameterFloat::ShaderParameterFloat(uint16 blockOffset, uint16 arrayCount)
		: IShaderParamter(blockOffset, arrayCount)
	{
		mType = EShaderParameterType::Float;
		mSize = sizeof(float) * arrayCount;
	}

	bool ShaderParameterFloat::GetFloat(const byte* block, uint16 offset, uint16 count, float* value) noexcept
	{
		if (count + offset > mArrayCount)
			return false;
		memcpy(value, block + mBlockOffset, sizeof(float) * count);
		return true;
	}

	bool ShaderParameterFloat::SetFloat(byte* block, uint16 offset, uint16 count, const float* value) noexcept
	{
		if (count + offset > mArrayCount)
			return false;
		memcpy(block + mBlockOffset, value, sizeof(float) * count);
		return true;
	}

	ShaderParameterBoolean::ShaderParameterBoolean(uint16 blockOffset, uint16 arrayCount)
		: IShaderParamter(blockOffset, arrayCount)
	{
		mType = EShaderParameterType::Boolean;
		mSize = sizeof(bool);
	}

	bool ShaderParameterBoolean::GetBoolean(const byte* block, uint16 offset, uint16 count, bool* value) noexcept
	{
		if (count + offset > mArrayCount)
			return false;
		memcpy(value, block + mBlockOffset, sizeof(bool) * count);
		return true;
	}

	bool ShaderParameterBoolean::SetBoolean(byte* block, uint16 offset, uint16 count, const bool* value) noexcept
	{
		if (count + offset > mArrayCount)
			return false;
		memcpy(block + mBlockOffset, value, sizeof(bool) * count);
		return true;
	}

	ShaderParameterTexture::ShaderParameterTexture(uint16 blockOffset, uint16 arrayCount)
		: IShaderParamter(blockOffset, arrayCount)
	{
		mType = EShaderParameterType::Texture;
		mSize = sizeof(Guid);
	}

	bool ShaderParameterTexture::GetTexture(const byte* block, Guid& texture) noexcept
	{
		memcpy(&texture, block + mBlockOffset, mSize);
		return true;
	}

	bool ShaderParameterTexture::SetTexture(byte* block, const Guid& texture) noexcept
	{
		memcpy(block + mBlockOffset, &texture, mSize);
		return true;
	}
}

