#include "Camera.h"
#include "Core/System/ConfigSystem.h"
#include "Core/System/UISystem.h"


namespace Eggy
{
	/*
		Left Hand, z-forward, y-upward, x-rightward

		^ Y    Z
		|    /
		|  /
		|/
	    0 - - - > X
	*/

	Camera::Camera()
	{
		mTransform_.SetIdentity();
		mTransform_.LookAt(Vector3(0.f, 0.f, -5.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f));
		mScreenHeight_ = static_cast<float>(ConfigSystem::Get()->GetGlobalConfig("AppHeight", 600));
		mScreenWidth_ = static_cast<float>(ConfigSystem::Get()->GetGlobalConfig("AppWidth", 800));
		updateProjMatrix();

		BindInputEvent(KeyDown, this, Camera::EventKeyDown);
		BindInputEvent(KeyUp, this, Camera::EventKeyUp);

		BindInputEvent(LeftMouseDown, this, Camera::EventLeftMouseDown);
		BindInputEvent(RightMouseDown, this, Camera::EventRightMouseDown);
		BindInputEvent(MidMouseDown, this, Camera::EventMidMouseDown);
		BindInputEvent(LeftMouseUp, this, Camera::EventLeftMouseUp);
		BindInputEvent(RightMouseUp, this, Camera::EventRightMouseUp);
		BindInputEvent(MidMouseUp, this, Camera::EventMidMouseUp);
		BindInputEvent(MouseMove, this, Camera::EventMouseMove);
		BindInputEvent(MouseWheel, this, Camera::EventMouseWheel);
	}

	void Camera::handleInputKeyDown(WPARAM key)
	{
		switch (key)
		{
#define HANDLE_CASE(CHAR) \
		case KEY_##CHAR: \
			mPressingKeyBit_ |= ECAMERA_KEY_BIT::EC_KEY_##CHAR; \
			break;
			HANDLE_CASE(W);
			HANDLE_CASE(S);
			HANDLE_CASE(A);
			HANDLE_CASE(D);
			HANDLE_CASE(Q);
			HANDLE_CASE(E);
#undef HANDLE_CASE

		case KEY_B:
			reset();
			break;
		default:
			break;
		}
	}

	void Camera::handleInputKeyUp(WPARAM key)
	{
		switch (key)
		{
#define HANDLE_CASE(CHAR) \
		case KEY_##CHAR: \
			mPressingKeyBit_ &= ~ECAMERA_KEY_BIT::EC_KEY_##CHAR; \
			break;
			HANDLE_CASE(W);
			HANDLE_CASE(S);
			HANDLE_CASE(A);
			HANDLE_CASE(D);
			HANDLE_CASE(Q);
			HANDLE_CASE(E);
#undef HANDLE_CASE

		default:
			break;
		}
	}

	void Camera::handleMouseButtonDown(EMOUSE_BUTTON key, int32_t x, int32_t y)
	{
		switch (key)
		{
		case LEFT:
			mPressingMouseKeyBit_ |= EC_MOUSE_LEFT;
			break;
		case MID:
			mPressingMouseKeyBit_ |= EC_MOUSE_MID;
			break;
		case RIGHT:
			mPressingMouseKeyBit_ |= EC_MOUSE_RIGHT;
			break;
		default:
			break;
		}
		mLastMousePositionX = x;
		mLastMousePositionY = y;
	}

	void Camera::handleMouseButtonUp(EMOUSE_BUTTON key, int32_t x, int32_t y)
	{
		switch (key)
		{
		case LEFT:
			mPressingMouseKeyBit_ &= ~EC_MOUSE_LEFT;
			break;
		case MID:
			mPressingMouseKeyBit_ &= ~EC_MOUSE_MID;
			break;
		case RIGHT:
			mPressingMouseKeyBit_ &= ~EC_MOUSE_RIGHT;
			break;
		default:
			break;
		}
	}

	void Camera::handleMouseWheel(short delta)
	{
		mFov_ = Clamp(mFov_ - delta / 120.f, 5.f, 90.f);
		updateProjMatrix();
	}

	void Camera::handleMouseMove(int32_t x, int32_t y, float deltaTime)
	{
		if (mPressingMouseKeyBit_)
		{
			float fixMoveDistance = 0.001f;
			float fixRotateRate = 5.f;
			float deltaX = float(x - mLastMousePositionX);
			float deltaY = float(y - mLastMousePositionY);
#define HANDLE_MOUSE(BUTTON) if (mPressingMouseKeyBit_ &  ECAMEAR_MOUSE_BUTTON::EC_MOUSE_##BUTTON)

			HANDLE_MOUSE(LEFT)
			{
				mTransform_.SetPitchYawRoll(
					Clamp(mTransform_.GetPitch() + fixRotateRate * deltaTime * deltaY, -MATH_HALF_PI + 0.1f, MATH_HALF_PI - 0.1f),
					mTransform_.GetYaw() + fixRotateRate * deltaTime * deltaX,
					mTransform_.GetRoll()
				);
			}

			HANDLE_MOUSE(MID)
			{
				mTransform_.SetTranslation(
					mTransform_.GetTranslation()
					- mTransform_.GetYAxis() * deltaY * fixMoveDistance
					+ mTransform_.GetXAxis() * deltaX * fixMoveDistance
				);
			}

			HANDLE_MOUSE(RIGHT)
			{

			}

#undef HANDLE_MOUSE

			mLastMousePositionX = x;
			mLastMousePositionY = y;
		}
	}

	void Camera::Tick(float deltaTime)
	{
		if (mPressingKeyBit_)
		{
#define HANDLE_KEY(CHAR) if (mPressingKeyBit_ & ECAMERA_KEY_BIT::EC_KEY_##CHAR)
			HANDLE_KEY(W) mTransform_.SetTranslation(mTransform_.GetTranslation() + mTransform_.GetZAxis() * deltaTime * mMoveSpeed_);
			HANDLE_KEY(S) mTransform_.SetTranslation(mTransform_.GetTranslation() - mTransform_.GetZAxis() * deltaTime * mMoveSpeed_);
			HANDLE_KEY(A) mTransform_.SetTranslation(mTransform_.GetTranslation() - mTransform_.GetXAxis() * deltaTime * mMoveSpeed_);
			HANDLE_KEY(D) mTransform_.SetTranslation(mTransform_.GetTranslation() + mTransform_.GetXAxis() * deltaTime * mMoveSpeed_);
			HANDLE_KEY(Q) mTransform_.SetTranslation(mTransform_.GetTranslation() - mTransform_.GetYAxis() * deltaTime * mMoveSpeed_);
			HANDLE_KEY(E) mTransform_.SetTranslation(mTransform_.GetTranslation() + mTransform_.GetYAxis() * deltaTime * mMoveSpeed_);
#undef HANDLE_KEY
		}
		SetupData();

		UIPanel* panelPtr = GetDebugPanel().GetWidget();
		panelPtr->CreateOrGetSlider<float>("WBasisX", 3, true).GetWidget()->BindData(mWBasisX_.GetPointer());
		panelPtr->CreateOrGetSlider<float>("WBasisY", 3, true).GetWidget()->BindData(mWBasisY_.GetPointer());
		panelPtr->CreateOrGetSlider<float>("WBasisZ", 3, true).GetWidget()->BindData(mWBasisZ_.GetPointer());
	}

	Matrix4x3 Camera::getViewMatrix() const
	{
		return mViewMatrix_;
	}

	const Matrix4x4& Camera::getProjMatrix() const
	{
		return mProjMatrix_;
	}

	void Camera::SetupData()
	{
		mViewMatrix_ = mTransform_.GetInverse();

		Vector3 vX{ mViewMatrix_.GetRow3(0) };
		Vector3 vY{ mViewMatrix_.GetRow3(1) };
		Vector3 vZ{ mViewMatrix_.GetRow3(2) };
		vZ.Normalize();
		vX.Normalize();
		vY.Normalize();


		float tanHalfFov = std::tan(DegreeToRadian(mFov_) * 0.5f);
		vY *= mFar_ * tanHalfFov;
		vX *= mFar_ * tanHalfFov * mScreenWidth_ / mScreenHeight_;
		vZ *= mFar_;

		// remapping for input in range (x:[0, 1], y:[0, 1], z:[0,1])
		vZ = vZ - vX - vY;
		vX *= 2;
		vY *= 2;

		mWBasisX_.x = vX.x; mWBasisX_.y = vX.y; mWBasisX_.z = vX.z; mWBasisX_.w = 0.0f;
		mWBasisY_.x = vY.x; mWBasisY_.y = vY.y; mWBasisY_.z = vY.z; mWBasisY_.w = 0.0f;
		mWBasisZ_.x = vZ.x; mWBasisZ_.y = vZ.y; mWBasisZ_.z = vZ.z; mWBasisZ_.w = 0.0f;
	}

	void Camera::updateProjMatrix()
	{
		float rad = DegreeToRadian(mFov_);
		float h = Cos(0.5f * rad) / Sin(0.5f * rad);
		float w = h * mScreenHeight_ / mScreenWidth_;
		if (mPerspective_)
		{
			mProjMatrix_ = Matrix4x4(
				w, 0, 0, 0,
				0, h, 0, 0,
				0, 0, mFar_ / (mFar_ - mNear_), 1,
				0, 0, -(mFar_ * mNear_) / (mFar_ - mNear_), 0
			);
		}
		else
		{
			mProjMatrix_ = Matrix4x4(
				2.f / w, 0, 0, 0,
				0, 2.f / h, 0, 0,
				0, 0, -1 / (mFar_ - mNear_), 0,
				0, 0, mNear_ / (mFar_ - mNear_), 1
			);
		}
		
	}

}
