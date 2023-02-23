#include "ShaderParamter.h"


namespace Eggy
{
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

	ShaderParamterFloat::ShaderParamterFloat(uint16 blockOffset, uint16 arrayCount)
		: IShaderParamter(blockOffset, arrayCount)
	{
		mType = EShaderParameterType::Float;
		mSize = sizeof(float) * arrayCount;
	}

	bool ShaderParamterFloat::GetFloat(const byte* block, uint16 offset, uint16 count, float* value) noexcept
	{
		if (count + offset > mArrayCount)
			return false;
		memcpy(value, block + mBlockOffset, sizeof(float) * count);
		return true;
	}

	bool ShaderParamterFloat::SetFloat(byte* block, uint16 offset, uint16 count, const float* value) noexcept
	{
		if (count + offset > mArrayCount)
			return false;
		memcpy(block + mBlockOffset, value, sizeof(float) * count);
		return true;
	}

	IShaderParamter* ShadingBatch::GetParameter(const String& name)
	{
		if (auto itr = mParams_.find(name); itr != mParams_.end())
		{
			return itr->second;
		}
		return nullptr;
	}

	bool ShadingBatch::SetInteget(const String& name, uint16 offset, uint16 count, const int* value) noexcept
	{
		if(auto param = GetParameter(name))
			return param->SetInteger(mParamterBlock_, offset, count, value);
		return false;
	}

	bool ShadingBatch::SetFloat(const String& name, uint16 offset, uint16 count, const float* value) noexcept
	{
		if (auto param = GetParameter(name))
			return param->SetFloat(mParamterBlock_, offset, count, value);
		return false;
	}

	bool ShadingBatch::SetBoolean(const String& name, uint16 offset, uint16 count, const bool* value) noexcept
	{
		if (auto param = GetParameter(name))
			return param->SetBoolean(mParamterBlock_, offset, count, value);
		return false;
	}


}

