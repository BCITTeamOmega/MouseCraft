#pragma once

// Enumerated type used for indexing and referencing events
enum EventName {
    PLAY_SONG, //used to trigger a BGM track to play
    PLAY_SOUND, //used to trigger a Sound Effect
	COMPONENT_UPDATE,
	COMPONENT_REMOVED,
	COMPONENT_ADDED
};
