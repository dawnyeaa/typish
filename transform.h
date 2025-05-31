#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>

using glm::vec3, glm::mat4;

class Transform {
public:
  Transform() {}

  void setPosition(float x, float y, float z);
  void setRotation(float x, float y, float z);
  void setScale(float scale);
  void setScale(float x, float y, float z);

  void rotate(float x, float y, float z);

  mat4 getMatrix();
private:
  vec3 position = vec3(0.f, 0.f, 0.f);
  vec3 rotation = vec3(0.f, 0.f, 0.f);
  vec3 scale = vec3(1.f, 1.f, 1.f);

  mat4 modelMatrix;
  bool modelMatrixSet = false;
};

#endif //TRANSFORM_H
