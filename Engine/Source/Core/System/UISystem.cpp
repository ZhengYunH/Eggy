#include "UISystem.h"


namespace Eggy
{
	DefineSystem(UISystem);

	void UISystem::Initialize()
	{
	}

	void UISystem::Finalize()
	{
	}

	IUIHandle IUIWidget::GetChildByName(const String& key)
	{
		if (!mSubWidgets_.contains(key))
			return nullptr;
		return mSubWidgets_.at(key);
	}

	void IUIWidget::AddChild(String key, IUIHandle handle)
	{
		mSubWidgets_.insert({ key, handle });
	}
}
