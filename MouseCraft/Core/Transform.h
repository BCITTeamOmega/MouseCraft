#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Vector2D.h"

class Transform
{
public:
	Transform() {};
	~Transform() {};

#pragma region Getters/Setters

	// Gets the local position.
	glm::vec3 getLocalPosition() const;

	// Sets the local position.
	void setLocalPosition(glm::vec3 position);

	// Gets the local rotation.
	glm::vec3 getLocalRotation() const;

	// Sets the local rotation.
	void setLocalRotation(glm::vec3 rotation);

	// Gets the local rotation as a quaternion.
	glm::quat getLocalQuaternion() const;

	// Sets the local rotation.
	void setLocalRotation(glm::quat rotation);

	// Gets the local scale.
	glm::vec3 getLocalScale() const;

	// Sets the local scale.
	void setLocalScale(glm::vec3 scale);

	// Gets the world position based on the current world transformation.
	glm::vec3 getWorldPosition() const;

	// Increments the local position by amount.
	void translate(glm::vec3 amount);

	// Times the local scale by amount.
	void scale(float amount);

	// Increments the local rotation by amount.
	void rotate(glm::vec3 amount);

	// Gets the world rotation based on the current world transformation.
	glm::vec3 getWorldRotation() const;

	// Gets the world scale based on the current world transformation.
	glm::vec3 getWorldScale() const;

	// Gets the local forward direction.
	glm::vec3 getLocalForward() const;

	// Gets the local right direciton.
	glm::vec3 getLocalRight() const;

	// Gets the local up direction.
	glm::vec3 getLocalUp() const;

	// Gets the world forward direction.
	glm::vec3 getWorldForward() const;

	// Gets the world right direction.
	glm::vec3 getWorldRight() const;

	// Gets the world up direction.
	glm::vec3 getWorldUp() const;

	// Gets the local transformation matrix.
	glm::mat4 getLocalTransformation() const;

	// Gets the world transformation matrix. 
	glm::mat4 getWorldTransformation() const;

	// Face towards the direction vector 
	void face2D(glm::vec2 dir);
	void face2D(glm::vec3 dir);

#pragma endregion

	// computes the local transformation matrix using local position, rotation, scale.
	void computeLocalTransformation();

	// compute the world transformation matrix with a given parent transformation.
	void computeWorldTransformation(glm::mat4 parent = glm::mat4(1.0f));

#pragma region Aliases

	// local
	inline glm::vec3 pos()		const { return getLocalPosition(); }
	inline glm::vec3 rot()		const { return getLocalRotation(); }
	inline glm::vec3 scl()		const { return getLocalScale(); }
	inline glm::vec3 forward()	const { return getLocalForward(); }
	inline glm::vec3 right()	const { return getLocalRight(); }
	inline glm::vec3 up()		const { return getLocalUp(); }
	inline glm::mat4 matrix()	const { return getLocalTransformation(); }
	// world
	inline glm::vec3 wPos()		const { return getWorldPosition(); }
	inline glm::vec3 wRot()		const { return getWorldRotation(); }
	inline glm::vec3 wScl()		const { return getWorldScale(); }
	inline glm::vec3 wForward()	const { return getWorldForward(); }
	inline glm::vec3 wRight()	const { return getWorldRight(); }
	inline glm::vec3 wUp()		const { return getWorldUp(); }
	inline glm::mat4 wMatrix()	const { return getWorldTransformation(); }
	// local setters
	inline void pos(glm::vec3 v) { setLocalPosition(v); }
	inline void rot(glm::vec3 v) { setLocalRotation(v); }
	inline void scl(glm::vec3 v) { setLocalScale(v); }

#pragma endregion

#pragma region Vector2D Interfaces

	// local
	Vector2D getLocalPosition2D() const;
	float    getLocalRotation2D() const;
	Vector2D getLocalScale2D()    const;
	Vector2D getLocalForward2D()  const;
	Vector2D getLocalRight2D()	  const;
	// world 
	Vector2D getWorldPosition2D() const;
	float    getWorldRotation2D() const;
	Vector2D getWorldScale2D()    const;
	Vector2D getWorldForward2D()  const;
	Vector2D getWorldRight2D()    const;
	// setters
	void setLocalPosition2D(Vector2D pos);
	void setLocalRotation2D(float angle);
	void setLocalScale2D(Vector2D scl);

#pragma endregion

private:
	static float getAngle2D(glm::vec2 dir);
	static float getAngle2D(glm::vec3 dir);

private:
	glm::vec3 _localPosition;
	glm::vec3 _localRotation;
	glm::quat _localQuat;
	glm::vec3 _localScale = glm::vec3(1.0f);
	glm::mat4 _localTransformation;
	glm::mat4 _worldTransformation;
};
