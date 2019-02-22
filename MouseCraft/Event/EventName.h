#pragma once

// Enumerated type used for indexing and referencing events
enum EventName {
    PLAY_SONG, //used to trigger a BGM track to play
    PLAY_SOUND, //used to trigger a Sound Effect
	COMPONENT_UPDATE,
	COMPONENT_REMOVED,
	COMPONENT_ADDED,
	ENTITY_CREATED,
	ENTITY_DESTROYED,
	// input is only raised when it changes!
	INPUT_RAW, 
	INPUT_AXIS, 
	INPUT_BUTTON,
	/*
	INPUT_P1_MOVE,	
	INPUT_P1_AIM,
	INPUT_P1_BUTTON,
	*/
};
