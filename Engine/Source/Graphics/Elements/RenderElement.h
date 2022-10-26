#pragma once
#include "Core/Config.h"
#include "Graphics/RHI/IRenderResource.h"
#include "Core/Interface/IRenderElement.h"
#include "Graphics/Renderer/VertexFactory.h"
#include "Graphics/RHI/IShader.h"


#include "Core/Math/Matrix4x3.h"
#include "Core/Math/Matrix4x4.h"

#include <DirectXMath.h>
#include <DirectXMathMatrix.inl>


namespace Eggy
{
	struct ObjectInfo
	{
		Matrix4x4 ModelTransform;
		Matrix4x4 ViewTransform;
		Matrix4x4 ProjectTransform;
	};

	class RenderElement : public IRenderElement
	{
	public:
		IGeometry Geometry;
		IShaderCollection ShaderCollection;
		IConstantBuffer ConstantBuffer;	
		ObjectInfo ObjectConstantData;
	};

	class RenderHelperElement : public RenderElement
	{
	public:
		using VertexType = TVertexType<EVF_P3F_C4B>;
		using IndexType = uint16;
		RenderHelperElement() = default;

		virtual void Initialize() override
		{
			FillInData();

			ShaderCollection.SetShaderPath(EShaderType::VS, "Engine/Shader/HLSL/Basic_VS.hlsl");
			ShaderCollection.SetShaderPath(EShaderType::PS, "Engine/Shader/HLSL/Basic_PS.hlsl");

			// fill Layout Desc
			VertexType::GetDesc(Geometry.Layout.Descs);

			// file VertexBuffer Desc
			GetVertexData(Geometry.VertexBuffer.Data, Geometry.VertexBuffer.Count, Geometry.VertexBuffer.ByteWidth);
			GetIndexData(Geometry.IndexBuffer.Data, Geometry.IndexBuffer.Count, Geometry.IndexBuffer.ByteWidth);
			Geometry.Layout.ShaderCollection = &ShaderCollection;
			ConstantBuffer.ByteWidth = sizeof(ObjectInfo);
			
			ObjectConstantData.ModelTransform.SetIdentity();

			Matrix4x3 viewMat;
			viewMat.LookAt(Vector3(0.f, 0.f, -5.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f));
			ObjectConstantData.ViewTransform = viewMat;

			float fov = 45;
			float rad = DegreeToRadian(fov);
			float h = Cos(0.5f * rad) / Sin(0.5f * rad);
			float mScreenHeight_ = 600;
			float mScreenWidth_ = 800;
			float mNear_ = 1;
			float mFar_ = 5000;
			float w = h * mScreenHeight_ / mScreenWidth_;
			ObjectConstantData.ProjectTransform = Matrix4x4(
				w, 0, 0, 0,
				0, h, 0, 0,
				0, 0, mFar_ / (mNear_ - mFar_), -1,
				0, 0, -(mFar_ * mNear_) / (mFar_ - mNear_), 0
			);

		}

		void CreateResource(IRenderResourceFactory* factory) override
		{
			static float phi = 0.0f, theta = 0.0f;
			Vector3 Translation;

			phi += 0.00001f;
			theta += 0.00015f;
			// ObjectConstantData.ModelTransform.SetRotationX(phi, Translation);
			// ObjectConstantData.ModelTransform.SetRotationY(theta, Translation);

			ShaderCollection.CreateDeviceResource(factory);
			Geometry.CreateDeviceResource(factory);
			if (ConstantBuffer.Usage == EBufferUsage::Dynamic)
			{

			}
			ConstantBuffer.CreateDeviceResource(factory);
		}


		void GetVertexData(void*& Data, size_t& VertexCount, size_t& ByteWidth) override
		{
			HYBRID_CHECK(!mVertexs_.empty());

			Data = (void*)mVertexs_.data();
			VertexCount = mVertexs_.size();
			auto a =sizeof(mVertexs_);
			ByteWidth = VertexCount * VertexType::GetSize();
		}

		void GetIndexData(void*& Data, size_t& IndexCount, size_t& ByteWidth) override
		{
			HYBRID_CHECK(!mIndexs_.empty());

			Data = (void*)mIndexs_.data();
			IndexCount = mIndexs_.size();
			auto a = sizeof(mVertexs_);

			ByteWidth = sizeof(IndexType) / sizeof(uint8) * IndexCount;
		}

	protected:
		virtual void FillInData() = 0;

	protected:
		List<VertexType> mVertexs_;
		List<IndexType> mIndexs_;
	};

	class RenderTriangleElement : public RenderHelperElement
	{
	protected:
		virtual void FillInData() override
		{
			mVertexs_ = List<VertexType>({
				{ Vector3(0.0f, 0.5f, 0.5f), Color4B(0.0f, 1.0f, 0.0f, 1.0f) },
				{ Vector3(0.5f, -0.5f, 0.5f), Color4B(0.0f, 0.0f, 1.0f, 1.0f) },
				{ Vector3(-0.5f, -0.5f, 0.5f), Color4B(1.0f, 0.0f, 0.0f, 1.0f) }
			});

			mIndexs_ = List<IndexType>({0, 1, 2});
		}
	};

	class RenderCubeElement : public RenderHelperElement
	{
	protected:
		virtual void FillInData() override
		{
			mVertexs_ = List<VertexType>({
				{ Vector3(-1.0f, -1.0f, -1.0f), Color4B(0.0f, 0.0f, 0.0f, 1.0f) },
				{ Vector3(-1.0f, 1.0f, -1.0f), Color4B(1.0f, 0.0f, 0.0f, 1.0f) },
				{ Vector3(1.0f, 1.0f, -1.0f), Color4B(1.0f, 1.0f, 0.0f, 1.0f) },
				{ Vector3(1.0f, -1.0f, -1.0f), Color4B(0.0f, 1.0f, 0.0f, 1.0f) },
				{ Vector3(-1.0f, -1.0f, 1.0f), Color4B(0.0f, 0.0f, 1.0f, 1.0f) },
				{ Vector3(-1.0f, 1.0f, 1.0f), Color4B(1.0f, 0.0f, 1.0f, 1.0f) },
				{ Vector3(1.0f, 1.0f, 1.0f), Color4B(1.0f, 1.0f, 1.0f, 1.0f) },
				{ Vector3(1.0f, -1.0f, 1.0f), Color4B(0.0f, 1.0f, 1.0f, 1.0f) }
			});

			mIndexs_ = List<IndexType>({
				0, 1, 2, 2, 3, 0,	// Front
				4, 5, 1, 1, 0, 4,	// Left
				1, 5, 6, 6, 2, 1,	// Top
				7, 6, 5, 5, 4, 7,	// Back
				3, 2, 6, 6, 7, 3,	// Right
				4, 0, 3, 3, 7, 4	// Bottom
			});
		}
	};
}