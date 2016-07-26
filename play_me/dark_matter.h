#pragma once
#include "object.h"
#include "state.h"
#include "array_2d.h"
#include "particle_system.h"

struct dark_matter : Object {
	struct redactor_info {
		v2i strt_pos;
	};
	redactor_info m_redactor;
	struct elem {
		v2i pos;
		int len;

		float time_to_next_particle;
	};
	rgba_array m_textures[2];
	list <elem> m_elements;
	particle_sys_1 m_ps1;
	
	void render (State state);
	void update (State state, float dt);
	void clean ();
	void load ();
};