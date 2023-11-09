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
    PerspectiveCamera::RecalculateMatrix();
}

void PerspectiveCamera::RecalculateMatrix() {
    this->ProjectionMatrix = glm::perspective(this->CameraFrustrum.angle, this->CameraFrustrum.width / this->CameraFrustrum.height,
                                              this->CameraFrustrum.near, this->CameraFrustrum.far);
    this->ViewMatrix = glm::lookAt(this->Position, this->LookAt, this->UpVector);

    this->ViewProjectionMatrix = glm::mat4(1.0f);

}

void PerspectiveCamera::zoom(float zoomValue) {
    if((zoomValue < 0 && glm::degrees(this->CameraFrustrum.angle) > 20.0f) || (zoomValue > 0 && glm::degrees(this->CameraFrustrum.angle) < 120.0f)) {
        this->CameraFrustrum.angle += glm::radians(zoomValue);
        this->RecalculateMatrix();
    }
}

void PerspectiveCamera::rotateArroundLookAt(float degree) {
    glm::vec3 direction = this->Position - this->LookAt;
    glm::mat4 rotationMat(1); // Creates a identity matrix
    rotationMat = glm::rotate(rotationMat, glm::radians(degree), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::vec3 rotatedDirection = rotationMat * glm::vec4(direction, 1.0f);
    this->Position = this->LookAt + rotatedDirection;
    this->RecalculateMatrix();
}