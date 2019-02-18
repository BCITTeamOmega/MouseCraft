#include "Camera.h"
#include "../Core/Entity.h"

Camera::Camera() {

}

Camera::~Camera() {

}

Transform Camera::getTransform() {
	return GetEntity()->transform;
}

float Camera::getFOV() {
	return _fov;
}

void Camera::setFOV(float fov) {
	_fov = fov;
}

float Camera::getCloseClip() {
	return _closeClip;
}

void Camera::setCloseClip(float clip) {
	_closeClip = clip;
}

float Camera::getFarClip() {
	return _farClip;
}

void Camera::setFarClip(float clip) {
	_farClip = clip;
}