#include "RenderScene.h"
#include "Graphics/RHI/IRenderHeader.h"
#include "Graphics/Elements/RenderElement.h"

namespace Eggy
{

	RenderScene::RenderScene()
	{
		for (uint32 i = uint32(ERenderSet::START); i < uint32(ERenderSet::END); ++i)
		{
			mRenderObjects_[ERenderSet(i)] = List<IRenderObject*>();
		}
	}

	// TODO add pool
	IRenderElement* RenderScene::AllocateRenderElement()
	{
		IRenderElement* ele = new RenderElement();
		mRenderElements_.push_back(ele);
		return ele;
	}

	List<IRenderObject*>& RenderScene::GetRenderObjects(ERenderSet set)
	{
		return mRenderObjects_[set];
	}

	void RenderScene::StartFrame()
	{
	}

	void RenderScene::EndFrame()
	{
		// clear RenderObject
		for (auto& pair : mRenderObjects_)
		{
			auto& objects = pair.second;
			for (auto* obj : objects)
			{
				delete obj;
			}
		}
		mRenderObjects_.clear();

		// clear RenderElement
		for (auto ele : mRenderElements_)
		{
			delete ele;
		}
		mRenderElements_.clear();
	}

	RenderObject* RenderScene::AllocateRenderObject()
	{
		return new RenderObject();
	}

	RenderObject* RenderScene::SubmitRenderElement(ERenderSet set, IRenderElement* element)
	{
		RenderObject* obj = AllocateRenderObject();
		mRenderObjects_[set].push_back(obj);

		// fill geometry
		{
			auto& Geometry = obj->Geometry;
			element->GetVertexData(Geometry.VertexBuffer.Data, Geometry.VertexBuffer.Count, Geometry.VertexBuffer.ByteWidth);
			element->GetIndexData(Geometry.IndexBuffer.Data, Geometry.IndexBuffer.Count, Geometry.IndexBuffer.ByteWidth);
			element->GetVertexDesc(Geometry.Layout.Descs);
		}

		// fill shader collection
		{
			auto& ShaderCollection = obj->ShaderCollection;
			ShaderCollection.SetShaderPath(EShaderType::VS, element->mMaterial->GetShaderPath(EShaderType::VS));
			ShaderCollection.SetShaderPath(EShaderType::PS, element->mMaterial->GetShaderPath(EShaderType::PS));
		}

		// fill constant buffer
		{
			auto& ObjectConstantData = obj->ObjectConstantData;
			ObjectConstantData = element->mObjectInfo;
		}

		obj->Consolidate();
		return obj;
	}

}
