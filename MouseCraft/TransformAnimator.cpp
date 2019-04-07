#include "TransformAnimator.h"

#include "Core/Entity.h"

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
			_counter -= _currentAnim->duration;

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
