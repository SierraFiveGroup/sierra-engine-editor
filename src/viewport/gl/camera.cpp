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

        float rotationSpeed = 45.0f; // degrees per second
        // Camera rotation in all axes (arrow keys) - generic implementation (move to mouse later)
        float pitch = 0.0f;
        float yaw = 0.0f;
        if (kb.isKeyPressed(Qt::Key_Up))
            pitch += rotationSpeed * dt;
        if (kb.isKeyPressed(Qt::Key_Down))
            pitch -= rotationSpeed * dt;
        if (kb.isKeyPressed(Qt::Key_Left))
            yaw += rotationSpeed * dt;
        if (kb.isKeyPressed(Qt::Key_Right))
            yaw -= rotationSpeed * dt;
        
        if (pitch != 0.0f || yaw != 0.0f) {
            QMatrix4x4 rotation;
            rotation.setToIdentity();
            rotation.rotate(yaw, mUp);
            rotation.rotate(pitch, mRight);
            mForward = (rotation * QVector4D(mForward, 0.0f)).toVector3D().normalized();
            mRight = QVector3D::crossProduct(mForward, mUp).normalized();
        }
    }

    QMatrix4x4 Camera::view() const {
        QMatrix4x4 v;
        v.lookAt(mPosition, mPosition + mForward, mUp);
        return v;
    }
}