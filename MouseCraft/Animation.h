#pragma once

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif // !GLM_ENABLE_EXPERIMENTAL

#include <map>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct Vec3Keyframe 
{
	float time;
	glm::vec3 value;
	
	bool operator<(const Vec3Keyframe& other) const 
	{
		return time < other.time;
	}
};

struct QuatKeyframe
{
	float time;
	glm::quat value;

	bool operator<(const QuatKeyframe& other) const
	{
		return time < other.time;
	}
};

class Animation
{
public:
	std::string name;

	float duration;

	void AddPosition(float time, glm::vec3);

	void AddRotation(float time, glm::quat);

	void AddScale(float time, glm::vec3);

	void SetPositionKeyFrames(std::vector<Vec3Keyframe> keyframes);

	void SetRotationKeyFrames(std::vector<QuatKeyframe> keyframes);

	void SetScaleKeyFrames(std::vector<Vec3Keyframe> keyframes);

	glm::vec3 GetPosition(float time) const;

	glm::quat GetRotation(float time) const;

	glm::vec3 GetScale(float time) const;

	int GetPositionsCount() const;

	int GetRotationCount() const;

	int GetScalesCount() const;

private:
	std::vector<Vec3Keyframe> _keyframesPos;
	std::vector<QuatKeyframe> _keyframesRot;
	std::vector<Vec3Keyframe> _keyframesScl;
};