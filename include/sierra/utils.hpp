// utils.hpp
// Licensed under LGPLv2.1

#pragma once

#include <glm/glm.hpp>

#define PI 3.14159265358979323846f
#define TAU (2 * PI)
#define DEG2RAD(x) ((x) * (PI / 180.0f))
#define RAD2DEG(x) ((x) * (180.0f / PI))

namespace SierraEditor {
    static glm::vec3 rgbtofloats(glm::vec3 color) {
        return glm::vec3(
            color.r / 255.0f,
            color.g / 255.0f,
            color.b / 255.0f
        );
    }
}