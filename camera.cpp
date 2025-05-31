#include "camera.h"

Camera::Camera() {
  transform = Transform();
  transform.setPosition(0.f, 0.f, 0.f);
  transform.setRotation(0.f, 0.f, 0.f);
}

void Camera::setPosition(float x, float y, float z) {
  transform.setPosition(x, y, z);
}




