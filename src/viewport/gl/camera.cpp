// camera.cpp
// Licensed under LGPLv2.1

#include <sierra/viewport/gl/camera.hpp>
#include <sierra/logger.hpp>

namespace SierraEditor::Viewport::GL {
    Camera::Camera()
        : 
        mPosition(0.0f, 2.0f, 5.0f),
        mForward(0.0f, 0.0f, -1.0f),
        mUp(0.0f, 1.0f, 0.0f)
    {
        mRight = QVector3D::crossProduct(mForward, mUp).normalized();
    }

    void Camera::update(float dt) {
        const auto& kb = IO::Keyboard::getInstance();

        float velocity = mMoveSpeed * dt;

        if (kb.isKeyPressed(Qt::Key_W))
            mPosition += mForward * velocity;
        if (kb.isKeyPressed(Qt::Key_S))
            mPosition -= mForward * velocity;

        if (kb.isKeyPressed(Qt::Key_A))
            mPosition -= mRight * velocity;
        if (kb.isKeyPressed(Qt::Key_D))
            mPosition += mRight * velocity;

        // simple vertical pan (arrow keys)
        if (kb.isKeyPressed(Qt::Key_Up))
            mPosition += mUp * velocity;
        if (kb.isKeyPressed(Qt::Key_Down))
            mPosition -= mUp * velocity;

        LOG("Camera position: (" + 
            std::to_string(mPosition.x()) + ", " + 
            std::to_string(mPosition.y()) + ", " + 
            std::to_string(mPosition.z()) + ")");
    }

    QMatrix4x4 Camera::view() const {
        QMatrix4x4 v;
        v.lookAt(mPosition, mPosition + mForward, mUp);
        return v;
    }
}