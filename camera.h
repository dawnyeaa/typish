#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>

#include "transform.h"

class Camera {
public:
  Camera();

  void setPosition(float x, float y, float z);

  void onKeyboard();

  glm::mat4 getMatrix();
private:
  Transform transform;
};

#endif //CAMERA_H
