#pragma once
#include "object.h"
#include "state.h"
#include "frame_buffer.h"

struct cat : Object {
	frames m_frames;
	bool m_to_right;
	v2f m_pos;
	float m_time_to_next_movement;

	void render (State state);
	void update (State state, float dt);
	void clean ();
	void load ();
};