#pragma once

#include <glm/glm.hpp>

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

#pragma endreigon

private:
	glm::vec3 _localPosition;
	glm::vec3 _localRotation;
	glm::vec3 _localScale = glm::vec3(1.0f);
	glm::mat4 _localTransformation;
	glm::mat4 _worldTransformation;
};

