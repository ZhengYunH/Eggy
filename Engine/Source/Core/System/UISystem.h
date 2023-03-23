#pragma once
#include "Core/Config.h"
#include "Core/Interface/ISystem.h"
#include "Core/Reflection/Reflection.h"
#include "DataType/SharedObject.h"
#include "Core/Engine/Event/Event.h"


namespace Eggy
{
	class IUITranslator;
	class IUIWidget;

	class IUIHandle : public TSharedPtr<IUIWidget>
	{
		using Super = TSharedPtr<IUIWidget>;
	public:
		IUIHandle(IUIWidget* widget) : Super(widget) {}
		IUIHandle(std::nullptr_t) : Super(nullptr) {}

	public:
		virtual IUIWidget* GetWidget() { return nullptr; }
	};

	template<typename _TWidget>
	class UIHandle : public IUIHandle
	{
		using type = _TWidget;
	public:
		UIHandle(_TWidget* widget)
			: IUIHandle(widget)
		{
		}

		UIHandle(std::nullptr_t) : IUIHandle(nullptr)
		{
		}

		virtual _TWidget* GetWidget() override
		{
			return (_TWidget*)Get();
		}
	};


	class UIPanel;
	template<typename _T> class TSliderWidget;
	using FloatSliderWidget = TSliderWidget<float>;
	class IUITranslator
	{
	public:
		virtual void Panel(UIPanel*) = 0;
		virtual void FloatSlider(FloatSliderWidget*) = 0;
	};

	template<typename _TWidget>
	struct WidgetTraits
	{
		static void Translate(IUITranslator* translator, _TWidget* widget) {}
	};

	class IUIWidget : public IRefObject
	{
	public:
		IUIWidget(const String& key) : mKey_(key)
		{
		}

		const String GetKey() { return mKey_; }

		IUIHandle GetChildByName(const String& key);
		Map<String, IUIHandle>& GetChildren() { return mSubWidgets_; }

		void AddChild(String key, IUIHandle handle);

		virtual void Translate(IUITranslator* translator) = 0;

	protected:
		String mKey_;
		Map<String, IUIHandle> mSubWidgets_;
	};

	template<typename _T>
	class TSliderWidget : public IUIWidget
	{
	public:
		TSliderWidget(const String& key, uint8 memberSize = 1, bool readonly=false) 
			: IUIWidget(key)
			, mMemberSize_(memberSize)
			, mReadOnly_(readonly)
		{
		}

	public:
		void Translate(IUITranslator* translator) override
		{
			WidgetTraits<TSliderWidget<_T>>::Translate(translator, this);
		}

	public:
		void SetRange(const _T& _min, const _T& _max)
		{
			Min(_min);
			Max(_max);
		}

		void BindData(_T* binding)
		{
			mDataBinding_ = binding;
		}

		_T* GetData()
		{
			HYBRID_CHECK(mDataBinding_ != nullptr);
			return mDataBinding_;
		}

		uint8 MemberSize() const { return mMemberSize_; }
		void MemberSize(uint8 val) { mMemberSize_ = val; }

		bool ReadOnly() const { return mReadOnly_; }
		void ReadOnly(bool val) { mReadOnly_ = val; }

		_T& Min() { return mMin_; }
		void Min(_T val) { mMin_ = val; }

		_T& Max() { return mMax_; }
		void Max(_T val) { mMax_ = val; }

		_T& Speed() { return mSpeed_; }
		void Speed(_T val) { mSpeed_ = val; }

	public:
		Event<> ValueChanged;

	protected:
		_T* mDataBinding_{ nullptr };
		uint8 mMemberSize_{ 1 };
		bool mReadOnly_{ false };

		_T mMin_{ std::numeric_limits<_T>::min() };
		_T mMax_{ std::numeric_limits<_T>::max() };

		_T mSpeed_{ _T(1) };
	};

	template<> struct WidgetTraits<FloatSliderWidget>
	{
		static void Translate(IUITranslator* translator, FloatSliderWidget* widget)
		{
			translator->FloatSlider(widget);
		}
	};


	class UIPanel : public IUIWidget
	{
	public:
		UIPanel(const String& key) : IUIWidget(key) { mTitle_ = key; }
		void SetTitle(const String& title)
		{
			mTitle_ = title;
		}

		String GetTitle()
		{
			return mTitle_;
		}

		template<typename _T>
		UIHandle<TSliderWidget<_T>> CreateOrGetSlider(const String& key, uint8 memberSize=1, bool readOnly=false)
		{
			auto handle = GetChildByName(key);
			if (!handle)
			{
				auto widget = new TSliderWidget<_T>(key, memberSize, readOnly);
				AddChild(key, widget);
				handle = widget;
			}
			TSliderWidget<_T>* widget = dynamic_cast<TSliderWidget<_T>*>(handle.Get());
			HYBRID_CHECK(widget, "Not a compatible type");
			return UIHandle<TSliderWidget<_T>>(widget);
		}

		void Translate(IUITranslator* translator) override
		{
			translator->Panel(this);
		}

	protected:
		String mTitle_{ "" };
	};

	class UISystem : public TSystem<UISystem>
	{
		DeclareSystem(UISystem);
	public:
		void Initialize() override;
		void Finalize() override;

	public:
		UIHandle<UIPanel> CreateOrGetPanel(String name)
		{
			if (!mPanels_.contains(name))
			{
				mPanels_.insert({ name, new UIPanel(name) });
			}
			return UIHandle<UIPanel>(mPanels_.at(name));
		}

		List<UIHandle<UIPanel>> GetAllPanels()
		{
			List<UIHandle<UIPanel>> panels;
			for (auto& key2Panel : mPanels_)
				panels.push_back(key2Panel.second);
			return panels;
		}

	protected:
		Map<String, UIHandle<UIPanel>> mPanels_;
	};

	static UIHandle<UIPanel> GetDebugPanel()
	{
		return UISystem::Get()->CreateOrGetPanel("Debug");
	}
}
