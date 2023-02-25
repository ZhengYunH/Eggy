#include "XMLArchive.h"


namespace Eggy
{
	void Archive::BeginSection(const std::string& sectionName)
	{
		_BeginSection(sectionName);
	}

	void Archive::_BeginSection(const std::string& sectionName)
	{
		if (mUsage_ == ARCHIVE_USAGE::SAVE)
		{
			if (!mCurrentTreeNode_)
			{
				_BeforeSave();
				mDebugCurrentSection_ = sectionName;
			}
			else
			{
				mDebugCurrentSection_ = mDebugCurrentSection_ + "." + sectionName;
			}
			ArchiveTreeNode* newTreeNode = new ArchiveTreeNode(sectionName);
			if (mCurrentTreeNode_)
			{
				mCurrentTreeNode_->InsertChild(newTreeNode);
			}
			mCurrentTreeNode_ = newTreeNode;
		}
		else if (mUsage_ == ARCHIVE_USAGE::LOAD)
		{
			Unimplement();
		}
		else
		{
			Unimplement();
		}

	}

	void Archive::EndSection()
	{
		if (mUsage_ == ARCHIVE_USAGE::SAVE)
		{
			HYBRID_CHECK(mCurrentTreeNode_);
			mCurrentTreeNode_ = mCurrentTreeNode_->mParent_;
			if (!mCurrentTreeNode_->mParent_) // Root Node
			{
				mRootTreeNode_->Consolidate();
				Save();
				_AfterSave();
				return;
			}

			{
				size_t splitIdx = mDebugCurrentSection_.rfind('.');
				if (splitIdx != std::string::npos)
					mDebugCurrentSection_.erase(splitIdx);
				else
					mDebugCurrentSection_ = "";
			}
		}
		else if (mUsage_ == ARCHIVE_USAGE::LOAD)
		{
			Unimplement();
		}
		else
		{
			Unimplement();
		}
	}

	void Archive::_BeforeSave()
	{
		HYBRID_CHECK(mState_ == ARCHIVE_STATE::None);

		mState_ = ARCHIVE_STATE::Saving;
		mDebugCurrentSection_.clear();
		mCurrentTreeNode_ = mRootTreeNode_;
	}

	void Archive::_AfterSave()
	{
		mState_ = ARCHIVE_STATE::Saved;
		mRootTreeNode_->Clear();
		mCurrentTreeNode_ = nullptr;
	}

	void Archive::_BeforeLoad()
	{
		mState_ = ARCHIVE_STATE::Loading;
	}

	void Archive::_AfterLoad()
	{
		mState_ = ARCHIVE_STATE::Loaded;
	}

	void Archive::_TryEndSection(const std::string& sectionName)
	{
		// this may cause error check when two subtree has same name, 
		// but i ignore this situation
		if (mCurrentTreeNode_ && mCurrentTreeNode_->mKey_ == sectionName)
		{
			EndSection();
		}
	}
}

