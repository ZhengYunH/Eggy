#include "Camera.h"

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
		mScreenHeight_ = static_cast<float>(SCREEN_INIT_HEIGHT);
		mScreenWidth_ = static_cast<float>(SCREEN_INIT_WIDTH);
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

	void Camera::tick(float deltaTime)
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
	}

	Matrix4x3 Camera::getViewMatrix() const
	{
		Matrix4x3 viewMat = mTransform_.GetInverse();
		return viewMat;
	}

	const Eggy::Matrix4x4& Camera::getProjMatrix() const
{
		return mProjMatrix_;
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
