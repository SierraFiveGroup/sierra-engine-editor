// camera.hpp
// Licensed under LGPLv2.1

#pragma once

#include <QMatrix4x4>
#include <QVector3D>
#include <Qt>

#include <sierra/io/keyboard.hpp>
#include <sierra/io/mouse.hpp>

#include <glm/glm.hpp>

namespace SierraEditor::Viewport::GL {
    class Camera {
        public:
            Camera();

            void update(float dt);
            QMatrix4x4 view() const;

            void setSpeed(float speed) {
                mMoveSpeed = speed;
            };

            void setPosition(const QVector3D& position) {
                mPosition = position;
            };

            void setSensitivity(float sensitivity) {
                mLookSensitivity = sensitivity;
            };

            glm::vec3 getGLMPosition() const {
                return glm::vec3(mPosition.x(), mPosition.y(), mPosition.z());
            }

            glm::vec2 getGLMYawPitch() const {
                return glm::vec2(mYaw, mPitch);
            }

        private:
            QVector3D mPosition;
            QVector3D mForward;
            QVector3D mRight;
            QVector3D mUp;

            float mPrevMouseX, mPrevMouseY;

            float mMoveSpeed = 5.0f;
            float mLookSensitivity = 0.5f;

            float mYaw = 0.0f;
            float mPitch = 0.0f;

    };
}