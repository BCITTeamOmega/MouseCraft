#pragma once

class System
{
public:
	System();
	~System();

	// set this to true if you only want the system to be updated once per frame.
	// bool onlyReceiveFrameUpdates = false;
	
	virtual void Update(float dt) = 0;
};
