#pragma once
#include "object.h"
#include "state.h"
#include "array_2d.h"
#include "input.h"
#include "particle_system.h"
#include <list>

struct ship : Object {
	struct info {
		v2f velocity;
		int lifes_left;
	};

	struct generator {
		v2i pos;
		float tm;
		float time_step;
		info cpy_inf;
	};

	struct elem {
		v2f pos;
		button_state in_solid;
		info inf;
		float time_to_nxt_cloud;
	};
	rgba_array m_tex[2];
	rgba_array m_tex_clouds[2];

	list <generator> m_generators;
	list <elem> m_elements;

	particle_sys_1 m_clouds;

	void render (State state);
	void update (State state, float dt);
	void clean ();
	void load ();
};