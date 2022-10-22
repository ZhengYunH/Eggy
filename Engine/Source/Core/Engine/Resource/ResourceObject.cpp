#include "ResourceObject.h"
#include "Core/File/File.h"


namespace Eggy
{
	void MeshResourceObject::Deserialize(IFile* file) noexcept
	{
		EFileType fileType = file->GetType();
		ObjFile* objFile;

		switch (fileType)
		{
		case Eggy::NONE:
			Unimplement();
			break;
		case Eggy::OBJ:
			objFile = dynamic_cast<ObjFile*>(file);
			size_t dataSize;
			objFile->Read(mData_, dataSize);
			objFile->GetIndex(mIndexs_);
			mVertexFormat_ = SelectVertexType<EFileType::OBJ>::type::FORMAT;
			mVertexStride_ = VertexFactory::GetDataTypeSize(mVertexFormat_);
			break;
		case Eggy::FBX:
			Unimplement();
			break;
		default:
			Unimplement();
			break;
		}
	}

	void MeshResourceObject::GetData(uint16 index, void* VertexData, List<uint32>& IndexData) noexcept
	{
		HYBRID_CHECK(index < mIndexs_.size());
		IndexData = mIndexs_[index];
		size_t offset = 0;
		if (index != 0)
			offset = mIndexs_[index - 1].size();
		VertexData = (char*)mData_ + mVertexStride_ * offset;
	}

}