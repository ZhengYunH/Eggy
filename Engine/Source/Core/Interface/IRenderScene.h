#pragma once
#include "Core/Config.h"
#include "Graphics/RHI/IRenderHeader.h"
#include "Graphics/RHI/IRenderPipeline.h"


namespace Eggy
{
	struct IRenderObject;
	struct IRenderElement;
	class Camera;

	class IRenderScene 
	{
	public:
		virtual void StartFrame() = 0;
		virtual void EndFrame() = 0;

		virtual void StartDeviceFrame() = 0;
		virtual void EndDeviceFrame() = 0;

		virtual Camera* GetCamera() = 0;
		virtual void SetCamera(Camera* camera) = 0;
	};
}