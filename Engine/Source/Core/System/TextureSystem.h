#pragma once
#include "Config.h"
#include "Interface/ISystem.h"
#include "Interface/ITexture.h"
#include "Misc/Guid.h"
#include "System/SystemManager.h"


namespace Eggy
{
	class TextureSystem : public TSystem<TextureSystem>
	{
		DeclareSystem(TextureSystem);
	public:
		void Initialize() override;
		void Finalize() override;

	public:
		void AddTexture(const Guid& guid, ITexture* texture);
		ITexture* GetTexture(const Guid& guid);

	protected:
		Map<Guid, ITexture*> mTextureCache_;
	};
	DeclareDependency(TextureSystem)

}

