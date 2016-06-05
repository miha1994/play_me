#pragma once
#include "object.h"
#include "state.h"
#include "frame_buffer.h"

enum Cat_state {
	Cat_on_floor,
	Cat_in_jump
};

struct cat : Object {
	frames m_frames;
	bool m_to_right;
	v2f m_pos;
	float m_time_to_next_movement; // время до след движения: моргнуть или вильнуть хвостом
	v2f m_velocity;  // скорость кота в дан момент
	Cat_state m_state;
	float m_time_velocity;  // время, которое показывает, сколько мы еще можем держать спейс //чем дольше держу спейс, тем выше прыжок

	void render (State state);
	void update (State state, float dt);
	void clean ();
	void load ();
};