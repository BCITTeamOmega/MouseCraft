#include "Transform.h"
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

#define GLM_ENABLE_EXPERIMENTAL	// I have no idea why we can't put this in main

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

glm::vec3 Transform::getLocalPosition() const
{
	return _localPosition;
}

void Transform::setLocalPosition(glm::vec3 position)
{
	_localPosition = position;
}

glm::vec3 Transform::getLocalRotation() const
{
	return _localRotation;
}

void Transform::setLocalRotation(glm::vec3 rotation)
{
	_localRotation = rotation;
}

glm::vec3 Transform::getLocalScale() const
{
	return _localScale;
}

void Transform::setLocalScale(glm::vec3 scale)
{
	_localScale = scale;
}

glm::vec3 Transform::getWorldPosition() const
{
	return glm::vec3(
		_worldTransformation[3][0],
		_worldTransformation[3][1],
		_worldTransformation[3][2]
	);
}

void Transform::translate(glm::vec3 amount)
{
	if (amount == glm::vec3()) return;
	setLocalPosition(getLocalPosition() + amount);
}

void Transform::scale(float amount)
{
	if (amount == 1) return;
	setLocalScale(getLocalScale() * amount);
}

void Transform::rotate(glm::vec3 amount)
{
	if (amount == glm::vec3()) return;
	setLocalRotation(getLocalRotation() + amount);
}

// WARNING: Entity::getWorldRotation() should be more performant (at the cost of memory locality?)
glm::vec3 Transform::getWorldRotation() const
{
	float y, x, z;
	glm::extractEulerAngleYXZ(_worldTransformation, y, x, z);
	return glm::vec3(x,y,z);
}

// WARNING: UNTESTED.
glm::vec3 Transform::getWorldScale() const
{
	return glm::vec3(
		glm::length(glm::vec3(_worldTransformation[0])),
		glm::length(glm::vec3(_worldTransformation[1])),
		glm::length(glm::vec3(_worldTransformation[2]))
	);
}

glm::vec3 Transform::getLocalForward() const
{
	return -glm::normalize(glm::vec3(
		_localTransformation[2][0],
		_localTransformation[2][1],
		_localTransformation[2][2]
	));
}

glm::vec3 Transform::getLocalRight() const
{
	return glm::normalize(glm::vec3(
		_localTransformation[0][0],
		_localTransformation[0][1],
		_localTransformation[0][2]
	));
}

glm::vec3 Transform::getLocalUp() const
{
	return glm::normalize(glm::vec3(
		_localTransformation[1][0],
		_localTransformation[1][1],
		_localTransformation[1][2]
	));
}

glm::vec3 Transform::getWorldForward() const
{
	return -glm::normalize(glm::vec3(
		_worldTransformation[2][0],
		_worldTransformation[2][1],
		_worldTransformation[2][2]
	));
}

glm::vec3 Transform::getWorldRight() const
{
	return glm::normalize(glm::vec3(
		_worldTransformation[0][0],
		_worldTransformation[0][1],
		_worldTransformation[0][2]
	));
}

glm::vec3 Transform::getWorldUp() const
{
	return glm::normalize(glm::vec3(
		_worldTransformation[1][0],
		_worldTransformation[1][1],
		_worldTransformation[1][2]
	));
}

glm::mat4 Transform::getLocalTransformation() const
{
	return _localTransformation;
}

glm::mat4 Transform::getWorldTransformation() const
{
	return _worldTransformation;
}

void Transform::face2D(glm::vec2 dir)
{
	auto rotation = glm::atan(dir.x / dir.y);
	if (dir.y > 0) 
		rotation -= M_PI;
	setLocalRotation(glm::vec3(0, rotation, 0));

}

void Transform::face2D(glm::vec3 dir)
{
	face2D(glm::vec2(dir.x, dir.z));
}

void Transform::computeLocalTransformation()
{
	// we going super-sonic http://www.opengl-tutorial.org/assets/faq_quaternions/index.html#Q26
	_localTransformation = glm::mat4(1.0f);
	_localTransformation = glm::translate(_localTransformation, _localPosition);

	// this is the precalculated y*x*z rotation matrix. (x2 faster)
	auto A = glm::cos(_localRotation.x);
	auto B = glm::sin(_localRotation.x);
	auto C = glm::cos(_localRotation.y);
	auto D = glm::sin(_localRotation.y);
	auto E = glm::cos(_localRotation.z);
	auto F = glm::sin(_localRotation.z);
	auto BC = B * C;
	auto BD = B * D;

	glm::mat4 rot = glm::mat4(0.0f);
	rot[0][0] = C * E + BD * F;
	rot[0][1] = A * F;
	rot[0][2] = -D * E + BC * F;
	rot[1][0] = -C * F + BD * E;
	rot[1][1] = A * E;
	rot[1][2] = D * F + BC * E;
	rot[2][0] = A * D;
	rot[2][1] = -B;
	rot[2][2] = A * C;
	rot[3][3] = 1.0f;
	_localTransformation = _localTransformation * rot;

	_localTransformation = glm::scale(_localTransformation, _localScale);
}

void Transform::computeWorldTransformation(glm::mat4 parent)
{
	_worldTransformation = parent * _localTransformation;
}

/*
#include <glm/gtx/matrix_decompose.hpp>
void Transform::setLocalTransform(glm::mat4 matrix)
{
	// warning: assuming matrix bottom-right value is 1. If not divide everything in matrix by that value. 
	glm::quat rot;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(matrix, scale, rot, _localPosition, skew, perspective);
	rotation = glm::eulerAngles(rot);
}
*/