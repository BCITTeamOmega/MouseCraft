#pragma once

// Enumerated type used for indexing and referencing events
enum EventName {
	// ENUM					| DATA TYPE			| INCLUDE FILE			| NOTES
	PLAY_SONG,			//	| TrackParams		| Sound/TrackParams.h	| Used to trigger a BGM track to play
	PLAY_SOUND,			//	| SoundParams		| Sound/SoundParams.h	| Used to trigger a Sound Effect
	COMPONENT_UPDATE,	//	| float				|						| Delta time 
	COMPONENT_REMOVED,	//	| Component*		| Core/Component.h		| DO NOT USE
	COMPONENT_ADDED,	//	| Component*		| Core/Component.h		| DO NOT USE
	ENTITY_CREATED,		//	| Entity*			| Core/Entity.h			| 
	ENTITY_DESTROYED,	//	| Entity*			| Core/Entity.h			| 
	INPUT_RAW,			//	|					|						| DO NOT USE
	INPUT_AXIS,			//	| AxisEvent			| Input/InputSystem.h	| Controller stick (1D)
	INPUT_AXIS_2D,		//	| Axis2DEvent		| Input/InputSystem.h	| Controller stick (2D)
	INPUT_BUTTON,		//	| ButtonEvent		| Input/InputSystem.h	| Controller button
	INPUT_MOUSE_CLICK,	//	| MouseButtonEvent	| Input/InputSystem.h	| Left and right click only
	INPUT_MOUSE_MOVE,	//	| glm::ivec2		| <glm/glm.hpp>			| 
	GAMEOVER,			//	| GameOverParams	| GameManager.h			| 
};
