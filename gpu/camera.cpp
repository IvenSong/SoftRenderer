#include "camera.h"

Camera::Camera(float fovy, float aspect, float n, float f, const vec3f& top)
{
	mTop = top;
	mProjectionMatrix = Math::perspective(fovy, aspect, n, f);
	update();
}

Camera::~Camera()
{
}

void Camera::setPose(const vec3f& position, const vec3f& target) {
	const vec3f direction = target - position;
	if (Math::lengthSQ(direction) <= 0.0f) {
		return;
	}
	mPosition = position;
	mFront = Math::normalize(direction);
	mPitch = std::asin(std::clamp(mFront.y, -1.0f, 1.0f)) * 180.0f / static_cast<float>(PI);
	mYaw = std::atan2(mFront.z, mFront.x) * 180.0f / static_cast<float>(PI);
	mViewMatrix = Math::lookAt<float>(mPosition, target, mTop);
}

void Camera::onRMouseDown(const int& x, const int& y) {
	mMouseMoving = true;
	mCurrentMouseX = x;
	mCurrentMouseY = y;
}

void Camera::onRMouseUp(const int& x, const int& y) {
	mMouseMoving = false;
}

void Camera::onKeyDown(const uint32_t& key) {
	switch (key) {
	case KEY_W:
		mMoveState |= MOVE_FRONT;
		break;
	case KEY_D:
		mMoveState |= MOVE_RIGHT;
		break;
	case KEY_A:
		mMoveState |= MOVE_LEFT;
		break;
	case KEY_S:
		mMoveState |= MOVE_BACK;
		break;
	}
}

void Camera::onKeyUp(const uint32_t& key)
{
	switch (key) {
	case KEY_W:
		mMoveState &= ~MOVE_FRONT;
		break;
	case KEY_D:
		mMoveState &= ~MOVE_RIGHT;
		break;
	case KEY_A:
		mMoveState &= ~MOVE_LEFT;
		break;
	case KEY_S:
		mMoveState &= ~MOVE_BACK;
		break;
	}
}

void Camera::onMouseMove(const int& x, const int& y) {
	if (mMouseMoving) {
		int xOffset = x - mCurrentMouseX;
		int yOffset = y - mCurrentMouseY;

		mCurrentMouseX = x;
		mCurrentMouseY = y;

		pitch(-yOffset);
		yaw(xOffset);
	}
}

void Camera::update() {
	vec3f moveDirection = { 0.0f, 0.0f, 0.0f };

	vec3f front = mFront;
	vec3f right = Math::normalize(cross(mFront, mTop));

	if (mMoveState & MOVE_FRONT) {
		moveDirection += front;
	}

	if (mMoveState & MOVE_BACK) {
		moveDirection += -front;
	}

	if (mMoveState & MOVE_RIGHT) {
		moveDirection += right;
	}

	if (mMoveState & MOVE_LEFT) {
		moveDirection += -right;
	}

	if (Math::lengthSQ(moveDirection) != 0) {
		moveDirection = Math::normalize(moveDirection);
		mPosition += moveDirection * mSpeed;
	}

	mViewMatrix = Math::lookAt<float>(mPosition, mPosition + mFront, mTop);
}

void Camera::pitch(int yOffset) {
	mPitch += yOffset * mSensitivity;

	if (mPitch >= 89.0f) {
		mPitch = 89.0f;
	}

	if (mPitch <= -89.0f) {
		mPitch = -89.0f;
	}

	mFront.y = sin(DEG2RAD(mPitch));
	mFront.x = cos(DEG2RAD(mYaw)) * cos(DEG2RAD(mPitch));
	mFront.z = sin(DEG2RAD(mYaw)) * cos(DEG2RAD(mPitch));
	mFront = Math::normalize(mFront);
}

void Camera::yaw(int xOffset) {
	mYaw += xOffset * mSensitivity;

	if (mYaw >= 360.0f) {
		mYaw -= 360.0f;
	}
	if (mYaw <= -360.0f) {
		mYaw += 360.0f;
	}


	mFront.y = sin(DEG2RAD(mPitch));
	mFront.x = cos(DEG2RAD(mYaw)) * cos(DEG2RAD(mPitch));
	mFront.z = sin(DEG2RAD(mYaw)) * cos(DEG2RAD(mPitch));

	mFront = Math::normalize(mFront);
}
