#pragma once
#include "object.h"
#include "state.h"
#include "array_2d.h"

struct solid : Object {
	array_2d <int> m_map;
	v2i m_start_mouse_pos;
	rgba_array m_tmp;
	rgba_array m_map_visualisation;
	v2i m_tmp_pos;

	void render (State state);
	void update (State state, float dt);
	void clean ();
	void load ();
};