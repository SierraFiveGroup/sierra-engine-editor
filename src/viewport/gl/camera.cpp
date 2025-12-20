// camera.cpp
// Licensed under LGPLv2.1

#include <sierra/viewport/gl/camera.hpp>
#include <sierra/logger.hpp>
#include <sierra/utils.hpp>

#include <cmath>

namespace SierraEditor::Viewport::GL {
    Camera::Camera()
        : 
        mPosition(0.0f, 2.0f, 5.0f),
        mForward(0.0f, 0.0f, -1.0f),
        mUp(0.0f, 1.0f, 0.0f)
    {
        mRight = QVector3D::crossProduct(mForward, mUp).normalized();
        mPrevMouseX = IO::Mouse::getInstance().getMouseX();
        mPrevMouseY = IO::Mouse::getInstance().getMouseY();

        // Initialize yaw/pitch from the starting forward vector
        mYaw = RAD2DEG(std::atan2(mForward.z(), mForward.x()));
        mPitch = RAD2DEG(std::asin(mForward.y()));
    }

    void Camera::update(float dt) {
        const auto& kb = IO::Keyboard::getInstance();

        if (kb.isKeyPressed(Qt::Key_Shift)) {
            mMoveSpeed = 15.0f;
        } else {
            mMoveSpeed = 5.0f;
        }

        
        if (IO::Mouse::getInstance().isButtonPressed(Qt::RightButton)) {
            float currentMouseX = IO::Mouse::getInstance().getMouseX();
            float currentMouseY = IO::Mouse::getInstance().getMouseY();

            float deltaX = currentMouseX - mPrevMouseX;
            float deltaY = currentMouseY - mPrevMouseY;

            mYaw   -= -deltaX * mLookSensitivity;
            mPitch += -deltaY * mLookSensitivity;
            mPitch = std::clamp(mPitch, -89.0f, 89.0f); // avoid gimbal lock

            mPrevMouseX = currentMouseX;
            mPrevMouseY = currentMouseY;
        } else {
            // Reset previous mouse positions when right button is not pressed
            mPrevMouseX = IO::Mouse::getInstance().getMouseX();
            mPrevMouseY = IO::Mouse::getInstance().getMouseY();
        }

        // Recompute orientation from yaw/pitch to keep basis orthonormal
        const float yawRad = DEG2RAD(mYaw);
        const float pitchRad = DEG2RAD(mPitch);

        mForward = QVector3D(
            std::cos(pitchRad) * std::cos(yawRad),
            std::sin(pitchRad),
            std::cos(pitchRad) * std::sin(yawRad)
        ).normalized();

        const QVector3D worldUp(0.0f, 1.0f, 0.0f);
        QVector3D right = QVector3D::crossProduct(mForward, worldUp);
        if (right.lengthSquared() > 1e-6f) {
            mRight = right.normalized();
        }
        mUp = QVector3D::crossProduct(mRight, mForward).normalized();

        // Movement using the refreshed basis vectors
        float velocity = mMoveSpeed * dt;

        if (kb.isKeyPressed(Qt::Key_W))
            mPosition += mForward * velocity;
        if (kb.isKeyPressed(Qt::Key_S))
            mPosition -= mForward * velocity;

        if (kb.isKeyPressed(Qt::Key_A))
            mPosition -= mRight * velocity;
        if (kb.isKeyPressed(Qt::Key_D))
            mPosition += mRight * velocity;
    }

    QMatrix4x4 Camera::view() const {
        QMatrix4x4 v;
        v.lookAt(mPosition, mPosition + mForward, mUp);
        return v;
    }
}