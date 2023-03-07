#pragma once
#include "Core/Config.h"


namespace Eggy
{
	class Camera;
	class ILight;
	class IRenderScene 
	{
	public:
		virtual void StartFrame() = 0;
		virtual void EndFrame() = 0;

		virtual void StartDeviceFrame() = 0;
		virtual void EndDeviceFrame() = 0;

		virtual Camera* GetCamera() = 0;
		virtual void SetCamera(Camera* camera) = 0;

		virtual void AddLight(ILight* light) = 0;
		virtual void DelLight(ILight* light) = 0;
		virtual List<ILight*>& GetLights() = 0;
	};
}