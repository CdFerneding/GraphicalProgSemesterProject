#include "PerspectiveCamera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"


PerspectiveCamera::PerspectiveCamera(const Frustrum& frustrum, const glm::vec3& position,
                                     const glm::vec3& lookAt,
                                     const glm::vec3& upVector) : Camera() {
    this->CameraFrustrum = frustrum;
    this->Position = position;
    this->LookAt = lookAt;
    this->UpVector = upVector;

}

void PerspectiveCamera::RecalculateMatrix() {
    this->ProjectionMatrix = glm::perspective(this->CameraFrustrum.angle, this->CameraFrustrum.width / this->CameraFrustrum.height,
                                              this->CameraFrustrum.near, this->CameraFrustrum.far);
    this->ViewMatrix = glm::lookAt(this->Position, this->LookAt, this->UpVector);
    this->ViewProjectionMatrix = this->ProjectionMatrix * this->ViewMatrix;
}