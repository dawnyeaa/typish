#include "transform.h"

void Transform::setPosition(float x, float y, float z) {
  position.x = x;
  position.y = y;
  position.z = z;

  modelMatrixSet = false;
}

void Transform::setRotation(float x, float y, float z) {
  rotation.x = x;
  rotation.y = y;
  rotation.z = z;

  modelMatrixSet = false;
}

void Transform::setScale(float scale) {
  this->scale.x = scale;
  this->scale.y = scale;
  this->scale.z = scale;

  modelMatrixSet = false;
}

void Transform::setScale(float x, float y, float z) {
  scale.x = x;
  scale.y = y;
  scale.z = z;

  modelMatrixSet = false;
}

void Transform::rotate(float x, float y, float z) {
  rotation.x += x;
  rotation.y += y;
  rotation.z += z;

  modelMatrixSet = false;
}

mat4 Transform::getMatrix() {
  if (modelMatrixSet) return modelMatrix;

  mat4 scale(this->scale.x, 0.f,           0.f,           0.f,
             0.f,           this->scale.y, 0.f,            0.f,
             0.f,           0.f,           this->scale.z, 0.f,
             0.f,           0.f,           0.f,           1.f);

  auto thetaX = glm::radians(rotation.x);
  auto thetaY = glm::radians(rotation.y);
  auto thetaZ = glm::radians(rotation.z);

  mat4 rotx(1.f, 0.f,               0.f,              0.f,
            0.f, glm::cos(thetaX),  glm::sin(thetaX),  0.f,
            0.f, -glm::sin(thetaX), glm::cos(thetaX), 0.f,
            0.f, 0.f,               0.f,              1.f);

  mat4 roty(glm::cos(thetaY),  0.f, glm::sin(thetaY), 0.f,
            0.f,               1.f, 0.f,               0.f,
            -glm::sin(thetaY), 0.f, glm::cos(thetaY), 0.f,
            0.f,               0.f, 0.f,              1.f);

  mat4 rotz(glm::cos(thetaZ),  glm::sin(thetaZ), 0.f, 0.f,
            -glm::sin(thetaZ), glm::cos(thetaZ), 0.f,  0.f,
            0.f,               0.f,              1.f, 0.f,
            0.f,               0.f,              0.f, 1.f);

  auto rotation = rotz * roty * rotx;

  mat4 translation(1.f,        0.f,        0.f,        0.f,
                   0.f,        1.f,        0.f,         0.f,
                   0.f,        0.f,        1.f,        0.f,
                   position.x, position.y, position.z, 1.f);

  mat4 transform = translation * rotation * scale;

  modelMatrix = transform;
  modelMatrixSet = true;

  return transform;
}




