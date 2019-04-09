#pragma once

#include <map>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Core/UpdatableComponent.h"
#include "Animation.h"

#include "Loading/PrefabLoader.h"
#include "json.hpp"
using json = nlohmann::json;

class TransformAnimator : public UpdatableComponent
{
public:
	TransformAnimator();
	~TransformAnimator();

	virtual void Update(float deltaTime);

	// Adds an Animation object to this component.
	void AddAnimation(Animation* animation);

	// Adds an Animation object to this component, overriding the name.
	void AddAnimation(std::string name, Animation* animation);

	// Sets the current animation to be played.
	void SetCurrentAnimation(std::string name);
	
	// Sets how far into the current animation this animator is in. 
	// Takes value 0 to 1.
	void SetProgress(float percent);

	// Changes the playback speed of the animation
	void SetSpeed(float multiplier);

	// If the animation should play only once. Animation will be unset on finish.
	void SetOneShot(bool oneShot);

	// Returns if animations will play only once. 
	bool GetOneShot() const;

private:
	std::map<std::string, Animation*> _anims;
	float _counter;
	float _speed = 1.0f;
	bool _oneShot = false;
	Animation* _currentAnim = nullptr;

	/* TEMPLATE
	{
		"type": "TransformAnimator",
		"speed": 1.0,
		"one_shot": false,
		"curve": "linear",
		"animations":
		[
			{
				"name": "jump",
				"duration": 2.0,
				"positions": // OPTIONAL
				[
					{
						"time": 0.0,
						"value": [0.0, 0.0, 0.0]
					},
					{
						"time": 0.0,
						"value": [0.0, 2.0, 0.0]
					}
				],
				"rotations": // OPTIONAL
				[
					{
						"time": 0.0,
						"value": [0.0, 0.0, 0.0]
					}
				], 
				"scales": // OPTIONAL
				[
					{
						"time": 0.0,
						"value": [0.0, 0.0, 0.0]
					}
				]
			},
			{
				"name": "nothing",
				"duration": 10.0
				// no position key frames
				// no rotation key frames
				// no scale key frames
			}
		]
	}
	*/
	static Component* Create(json json);
	static PrefabRegistrar reg;
};

