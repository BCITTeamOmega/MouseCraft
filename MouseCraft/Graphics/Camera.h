#pragma once
#include "../Core/Component.h"
#include "../Core/Transform.h"
class Camera : public Component {
public:
	Camera();
	~Camera();
	Transform getTransform();
	float getFOV();
	void setFOV(float fov);
	float getCloseClip();
	void setCloseClip(float clip);
	float getFarClip();
	void setFarClip(float clip);
private:
	float _fov;
	float _closeClip;
	float _farClip;
};