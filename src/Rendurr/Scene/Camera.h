#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace rnd {
enum class ProjectionType : uint8_t { Orthographic, Perspective };

struct Camera {
  glm::vec3 position;
  glm::vec3 target;
  glm::vec3 upDir{0.0f, 1.0f, 0.0f};

  float aspectRatio;
  float zoom;

  ProjectionType projType;
};
} // namespace rnd
