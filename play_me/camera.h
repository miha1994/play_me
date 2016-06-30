#pragma once
#include "object.h"
#include "state.h"
#include "math_3d.h"
#include "array_2d.h"

struct camera : Object {
	v2f m_pos;
	array_2d <int> m_map;
	rgba_array m_map_visualisation;

	v2i m_start_mouse_pos;
	rgba_array m_tmp;
	v2i m_tmp_pos;
	int m_thick;
	bool m_draw_in_game_mode;

	void render (State state);
	void update (State state, float dt);
	void clean ();
	void load ();
};