#include "TransformAnimator.h"

#include "Core/Entity.h"

#include "ResourceCache.h"

TransformAnimator::TransformAnimator()
{
}

TransformAnimator::~TransformAnimator()
{
}

void TransformAnimator::Update(float deltaTime)
{
	if (_currentAnim != nullptr)
	{
		_counter += deltaTime * _speed;
		if (_counter > _currentAnim->duration)
		{
			if (_oneShot)
			{
				_counter = 0.0f;
				_currentAnim = nullptr;
				return;
			}
			else
			{
				_counter -= _currentAnim->duration;
			}
		}

		if (_currentAnim->GetPositionsCount() > 0)
			GetEntity()->transform.setLocalPosition(_currentAnim->GetPosition(_counter));

		if (_currentAnim->GetRotationCount() > 0)
			GetEntity()->transform.setLocalRotation(_currentAnim->GetRotation(_counter));

		if (_currentAnim->GetScalesCount() > 0)
			GetEntity()->transform.setLocalScale(_currentAnim->GetScale(_counter));
	}
}

void TransformAnimator::AddAnimation(Animation * animation)
{
	AddAnimation(animation->name, animation);
}

void TransformAnimator::AddAnimation(std::string name, Animation * animation)
{
	_anims[name] = animation;
	if (_anims.size() == 1) 
		_currentAnim = _anims[name];
}

void TransformAnimator::SetCurrentAnimation(std::string name)
{
	_counter = 0;
	_currentAnim = (_anims.count(name) == 0) ? nullptr : _anims[name];
}

void TransformAnimator::SetProgress(float percent)
{
	if (_currentAnim == nullptr)
		return;

	if (percent < 0.0f)
		percent = 0.0f;
	else if (percent > 1.0f)
		percent = 1.0f;

	_counter = _currentAnim->duration * percent;
}

void TransformAnimator::SetSpeed(float multiplier)
{
	_speed = multiplier;
}

void TransformAnimator::SetOneShot(bool oneShot)
{
	_oneShot = oneShot;
}

bool TransformAnimator::GetOneShot() const
{
	return _oneShot;
}

Component * TransformAnimator::Create(json json)
{
	auto c = ComponentManager<UpdatableComponent>::Instance().Create<TransformAnimator>();
	c->_speed = json["speed"].get<float>();
	c->_oneShot = json["one_shot"].get<bool>();

	auto animations = json["animations"];
	for (auto& j : animations)
	{
		auto key = j["name"].get<std::string>();
		auto a = ResourceCache<Animation>::Instance().Get(key);
		if (a == nullptr)
		{
			a = Animation::CreateFromJson(j);
			ResourceCache<Animation>::Instance().Add(key, a);
		}
		c->AddAnimation(a);
	}
	
	return c;
}

PrefabRegistrar TransformAnimator::reg("TransformAnimator", &TransformAnimator::Create);