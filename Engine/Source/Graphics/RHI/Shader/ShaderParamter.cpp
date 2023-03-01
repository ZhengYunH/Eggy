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
			param = new ShaderParameterTexture(mParameterOffset_);
			break;
		case EShaderParameterType::Matrix4x3:
			param = new ShaderParameterMatrix4x3(mParameterOffset_);
			break;
		case EShaderParameterType::Matrix4x4:
			param = new ShaderParameterMatrix4x4(mParameterOffset_);
			break;
		default:
			Unimplement(0);
			break;
		}
		mParameterOffset_ += param->GetSize();
		mParams_[name] = param;
		return param;
	}

	void ShadingBatch::Clear()
	{
		for (auto& pair : mParams_)
			delete pair.second;
		mParams_.clear();
		mParameterOffset_ = 0;
	}

	ShadingBatch::~ShadingBatch()
	{
		Clear();
	}

	bool ShadingParamterCollection::SetInteget(const String& name, uint16 offset, uint16 count, const int* value) noexcept
	{
		if (auto param = GetParameter(name))
			return param->SetInteger(mParameterBlock_, offset, count, value);
		return false;
	}

	bool ShadingParamterCollection::SetFloat(const String& name, uint16 offset, uint16 count, const float* value) noexcept
	{
		if (auto param = GetParameter(name))
			return param->SetFloat(mParameterBlock_, offset, count, value);
		return false;
	}

	bool ShadingParamterCollection::SetBoolean(const String& name, uint16 offset, uint16 count, const bool* value) noexcept
	{
		if (auto param = GetParameter(name))
			return param->SetBoolean(mParameterBlock_, offset, count, value);
		return false;
	}

	IShaderParamter* ShadingParamterCollection::GetParameter(const String& name)
	{
		IShaderParamter* param = mBatch_->GetParameter(name);
		if (param)
			return param;
		return nullptr;
	}

	void ShadingParamterCollection::ExpandBlock() noexcept
	{
		byte* _alloc = new byte[mBlockTotalSize_ + mBlockAllocationSize_];
		memset(_alloc + mBlockTotalSize_, 0, mBlockAllocationSize_);
		memcpy(_alloc, mParameterBlock_, mBlockTotalSize_);
		delete[] mParameterBlock_;
		mParameterBlock_ = _alloc;
		mBlockTotalSize_ = mBlockTotalSize_ + mBlockAllocationSize_;
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

	ShaderParameterTexture::ShaderParameterTexture(uint16 blockOffset)
		: IShaderParamter(blockOffset, 1)
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

	ShaderParameterMatrix4x3::ShaderParameterMatrix4x3(uint16 blockOffset)
		: IShaderParamter(blockOffset, 1)
	{
		mType = EShaderParameterType::Matrix4x3;
		mSize = sizeof(Matrix4x3);
	}

	bool ShaderParameterMatrix4x3::GetMatrix4x3(const byte* block, Matrix4x3& matrix) noexcept
	{
		memcpy(&matrix, block, sizeof(Matrix4x3));
		return true;
	}

	bool ShaderParameterMatrix4x3::SetMatrix4x3(byte* block, const Matrix4x3& matrix) noexcept
	{
		memcpy(block, &matrix, sizeof(Matrix4x3));
		return true;
	}

	ShaderParameterMatrix4x4::ShaderParameterMatrix4x4(uint16 blockOffset)
		: IShaderParamter(blockOffset, 1)
	{
		mType = EShaderParameterType::Matrix4x4;
		mSize = sizeof(Matrix4x4);
	}

	bool ShaderParameterMatrix4x4::GetMatrix4x4(const byte* block, Matrix4x4& matrix) noexcept
	{
		memcpy(&matrix, block, sizeof(Matrix4x4));
		return true;
	}

	bool ShaderParameterMatrix4x4::SetMatrix4x4(byte* block, const Matrix4x4& matrix) noexcept
	{
		memcpy(block, &matrix, sizeof(Matrix4x4));
		return true;
	}

}

