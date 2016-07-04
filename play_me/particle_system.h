#pragma once
#include "glob.h"
#include "array_2d.h"
#include <list>

struct particle_sys_1 {
	struct elem {
		rgba_array *tex;
		v2f pos;
		v2f vel;
		float tm[3];
		float alpha1;
		int phase;
		float time;
	};
	list <elem> m_particles;

	void add_elem (rgba_array *tex, v2f _start_position, v2f _velocity_direction, float _len1, float _len2, float _len3, float time_of_all_moving);
	void upd (float dt);
	void render (v2f cam_pos);
};