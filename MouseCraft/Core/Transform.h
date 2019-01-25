#pragma once

#include <glm/glm.hpp>

class Transform
{
public:
	Transform() {};
	~Transform() {};

#pragma region Getters/Setters

	// Gets the local position.
	glm::vec3 getLocalPosition();

	// Sets the local position.
	void setLocalPosition(glm::vec3 position);

	// Gets the local rotation.
	glm::vec3 getLocalRotoation();
	
	// Sets the local rotation.
	void setLocalRotation(glm::vec3 rotation);
	
	// Gets the local scale.
	glm::vec3 getLocalScale();
	
	// Sets the local scale.
	void setLocalScale(glm::vec3 scale);

	// Gets the world position based on the current world transformation.
	glm::vec3 getWorldPosition();

	// ===== WARNING: SECTION UNTESTED ===== //

	// Gets the world rotation based on the current world transformation.
	glm::vec3 getWorldRotation();
	
	// Gets the world scale based on the current world transformation.
	glm::vec3 getWorldScale();

	// Gets the local forward direction.
	glm::vec3 getLocalForward();

	// Gets the local right direciton.
	glm::vec3 getLocalRight();

	// Gets the local up direction.
	glm::vec3 getLocalUp();

	// Gets the world forward direction.
	glm::vec3 getWorldForward();

	// Gets the world right direction.
	glm::vec3 getWorldRight();

	// Gets the world up direction.
	glm::vec3 getWorldUp();

	// ========== END SECTION ========== //

	// Gets the world transformation matrix.
	glm::mat4 getLocalTransformation();

	// Gets the local transformation matrix. 
	glm::mat4 getWorldTransformation();


#pragma endregion

	// computes the local transformation matrix using local position, rotation, scale.
	void computeLocalTransformation();

	// compute the world transformation matrix with a given parent transformation.
	void computeWorldTransformation(glm::mat4 parent = glm::mat4(1.0f));

private:
	glm::vec3 _localPosition;
	glm::vec3 _localRotation;
	glm::vec3 _localScale = glm::vec3(1.0f);
	glm::mat4 _localTransformation;
	glm::mat4 _worldTransformation;
};

