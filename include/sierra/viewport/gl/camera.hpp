// camera.hpp
// Licensed under LGPLv2.1

#pragma once

#include <QMatrix4x4>
#include <QVector3D>
#include <Qt>

#include <sierra/io/keyboard.hpp>

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

        private:
            QVector3D mPosition;
            QVector3D mForward;
            QVector3D mRight;
            QVector3D mUp;

            float mMoveSpeed = 5.0f;

    };
}