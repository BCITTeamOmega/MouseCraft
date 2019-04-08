#pragma once

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif // !GLM_ENABLE_EXPERIMENTAL

#include <map>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

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

class LinearConverter
{
public:
	virtual float Convert(float t) { return t; };
protected:
	static float Expand(float v) { return v * 2 - 1; }
	static float Linearize(float v) { return v * 0.5f + 0.5f; }
};

class SineConverter : public LinearConverter
{
public:
	float Convert(float t) override 
	{
		auto input = LinearConverter::Expand(t) * glm::pi<float>() / 2.0f;	// convert 0...1 to -pi/2...pi/2
		auto output = LinearConverter::Linearize( glm::sin(input) );				// return 0...1
		std::cout << "original: " << t << " modified: " << output << std::endl;
		return output;
	};
};

class Animation
{
public:
	std::string name;

	float duration;

	void AddPosition(float time, glm::vec3 position);

	void AddRotation(float time, glm::quat rotation);
	
	void AddRotation(float time, glm::vec3 rotation);

	void AddScale(float time, glm::vec3 scale);

	void SetPositionKeyFrames(std::vector<Vec3Keyframe> keyframes);

	void SetRotationKeyFrames(std::vector<QuatKeyframe> keyframes);

	void SetScaleKeyFrames(std::vector<Vec3Keyframe> keyframes);

	glm::vec3 GetPosition(float time) const;

	glm::quat GetRotation(float time) const;

	glm::vec3 GetScale(float time) const;

	int GetPositionsCount() const;

	int GetRotationCount() const;

	int GetScalesCount() const;

	void SetCurve(LinearConverter* converter);

private:
	std::vector<Vec3Keyframe> _keyframesPos;
	std::vector<QuatKeyframe> _keyframesRot;
	std::vector<Vec3Keyframe> _keyframesScl;
	LinearConverter* _converter = defaultConverter;
	static LinearConverter* defaultConverter;
};