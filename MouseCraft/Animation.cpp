#include "Animation.h"

#include <algorithm>
#include <glm/gtx/compatibility.hpp>

LinearConverter* Animation::defaultConverter = new LinearConverter();

void Animation::AddPosition(float time, glm::vec3 position)
{
	_keyframesPos.push_back(Vec3Keyframe{ time, position });
	std::sort(_keyframesPos.begin(), _keyframesPos.end());
}

void Animation::AddRotation(float time, glm::quat rotation)
{
	_keyframesRot.push_back(QuatKeyframe{ time, rotation });
	std::sort(_keyframesRot.begin(), _keyframesRot.end());
}

void Animation::AddRotation(float time, glm::vec3 rotation)
{
	AddRotation(time, glm::quat(rotation));
}

void Animation::AddScale(float time, glm::vec3 scale)
{
	_keyframesScl.push_back(Vec3Keyframe{ time, scale });
	std::sort(_keyframesScl.begin(), _keyframesScl.end());
}

void Animation::SetPositionKeyFrames(std::vector<Vec3Keyframe> keyframes)
{
	_keyframesPos = keyframes;
	std::sort(_keyframesPos.begin(), _keyframesPos.end());
}

void Animation::SetRotationKeyFrames(std::vector<QuatKeyframe> keyframes)
{
	_keyframesRot = keyframes;
	std::sort(_keyframesRot.begin(), _keyframesRot.end());
}

void Animation::SetScaleKeyFrames(std::vector<Vec3Keyframe> keyframes)
{
	_keyframesScl = keyframes;
	std::sort(_keyframesScl.begin(), _keyframesScl.end());
}

int Animation::GetPositionsCount() const
{
	return _keyframesPos.size();
}

int Animation::GetRotationCount() const
{
	return _keyframesRot.size();
}

int Animation::GetScalesCount() const
{
	return _keyframesScl.size();
}

void Animation::SetCurve(LinearConverter * converter)
{
	if (converter != nullptr)
		_converter = converter;
}

Animation * Animation::CreateFromJson(json json)
{
	Animation* a = new Animation();
	a->name = json["name"].get<std::string>();
	a->duration = json["duration"].get<float>();

	if (json.find("positions") != json.end())
	{
		auto positions = json["positions"];
		for (auto& j : positions)
		{
			auto t = j["time"].get<float>();
			auto pos = glm::vec3(
				j["value"][0].get<float>(), 
				j["value"][1].get<float>(), 
				j["value"][2].get<float>());
			a->AddPosition(t, pos);
		}
	}

	if (json.find("rotations") != json.end())
	{
		auto rotations = json["rotations"];
		for (auto& j : rotations)
		{
			auto t = j["time"].get<float>();
			auto rot = glm::vec3(
				j["value"][0].get<float>(),
				j["value"][1].get<float>(),
				j["value"][2].get<float>());
			a->AddRotation(t, rot);
		}
	}

	if (json.find("scales") != json.end())
	{
		auto scales = json["scales"];
		for (auto& j : scales)
		{
			auto t = j["time"].get<float>();
			auto scl = glm::vec3(
				j["value"][0].get<float>(),
				j["value"][1].get<float>(),
				j["value"][2].get<float>());
			a->AddRotation(t, scl);
		}
	}

	auto curve = json["curve"];
	if (curve == "sine")
		a->SetCurve(new SineConverter());
	// else LINEAR 

	return a;
}

glm::vec3 Animation::GetPosition(float time) const
{
	// WARNING: no keyframes
	if (_keyframesPos.size() == 0) return glm::vec3();			
	
	auto& first = _keyframesPos[0];
	auto& last = _keyframesPos[_keyframesPos.size() - 1];
	
	// Only one keyframe
	if (_keyframesPos.size() == 1) return first.value;
	// Time is before first keyframe.
	if (first.time > time) return first.value;
	// Time is later than last keyframe.
	if (last.time < time) return last.value;

	// Find interpolated value
	glm::vec3 ret;
	for (int i = 0; i < _keyframesPos.size(); ++i)
	{
		if (_keyframesPos[i].time > time)
		{
			auto& prev = _keyframesPos[i - 1];
			auto& next = _keyframesPos[i];
			float percent = (time - prev.time) / (next.time - prev.time);
			ret = glm::lerp(prev.value, next.value, _converter->Convert(percent));
			break;
		}
	}
	return ret;
}

glm::quat Animation::GetRotation(float time) const
{
	// WARNING: no keyframes
	if (_keyframesRot.size() == 0) return glm::vec3();
	auto& first = _keyframesRot[0];
	auto& last = _keyframesRot[_keyframesRot.size() - 1];
	// Only one keyframe
	if (_keyframesRot.size() == 1) return first.value;
	// Time is before first keyframe.
	if (first.time > time) return first.value;
	// Time is later than last keyframe.
	if (last.time < time) return last.value;
	// Find interpolated value
	glm::quat ret;
	for (int i = 0; i < _keyframesRot.size(); ++i)
	{
		if (_keyframesRot[i].time > time)
		{
			auto& prev = _keyframesRot[i - 1];
			auto& next = _keyframesRot[i];
			float percent = (time - prev.time) / (next.time - prev.time);
			ret = glm::lerp(prev.value, next.value, _converter->Convert(percent));
			break;
		}
	}
	return ret;
}

glm::vec3 Animation::GetScale(float time) const
{
	// WARNING: no keyframes
	if (_keyframesScl.size() == 0) return glm::vec3();
	auto& first = _keyframesScl[0];
	auto& last = _keyframesScl[_keyframesScl.size() - 1];
	// Only one keyframe
	if (_keyframesScl.size() == 1) return first.value;
	// Time is before first keyframe.
	if (first.time > time) return first.value;
	// Time is later than last keyframe.
	if (last.time < time) return last.value;
	// Find interpolated value
	glm::vec3 ret;
	for (int i = 0; i < _keyframesScl.size(); ++i)
	{
		if (_keyframesScl[i].time > time)
		{
			auto& prev = _keyframesScl[i - 1];
			auto& next = _keyframesScl[i];
			float percent = (time - prev.time) / (next.time - prev.time);
			ret = glm::lerp(prev.value, next.value, _converter->Convert(percent));
			break;
		}
	}
	return ret;
}
