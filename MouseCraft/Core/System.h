#pragma once

class System
{
public:
	System();
	~System();
	
	virtual void Update(float dt) = 0;
};
