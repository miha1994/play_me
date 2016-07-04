#pragma once
#include "object.h"
#include "state.h"
#include "frame_buffer.h"
#include "particle_system.h"

enum Cat_state {
	Cat_on_floor,
	Cat_in_jump,
	Cat_horizontal_fly,
	Cat_slide
};

struct cat : Object {
	frames m_frames;
	particle_sys_1 m_fly_effect;
	rgba_array m_fly_effect_tex[2];
	rgba_array m_black_screen;
	bool m_to_right;
	v2f m_pos;
	float m_time_to_next_movement; // время до след движения: моргнуть или вильнуть хвостом
	v2f m_velocity;  // скорость кота в дан момент
	Cat_state m_state;
	float m_time_velocity;  // время, которое показывает, сколько мы еще можем держать спейс //чем дольше держу спейс, тем выше прыжок
	float m_last_jump_btn_pressed;
	float m_horizontal_fly_time;
	bool m_horizontal_fly_able;
	float m_fly_next_effect_time;
	float m_slide_time;
	struct check_point {
		bool to_right;
		v2i pos;
		v2i cam_pos;
	};
	check_point m_check_point;
	bool m_game_over;
	float m_time_to_reload;

	void render (State state);
	void update (State state, float dt);
	void clean ();
	void load ();
};