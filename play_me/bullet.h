#pragma once

#include "object.h"
#include "state.h"
#include "glob.h"
#include "array_2d.h"
#include "frame_buffer.h"
#include "animation.h"
#include <list>

const int g_num_of_bullet_types = 1;

struct bullet : Object {
	rgba_array m_textures[g_num_of_bullet_types];
	frames m_blows[g_num_of_bullet_types];
	struct elem {
		v2f pos;
		v2f velocity;
		int type_num;
		float tm_inc;
		float tm_dec;
		bool active;
	};
	list <elem> m_all_bullets;
	animation m_blow_anim;
	bool m_type_is_restorable[g_num_of_bullet_types];

	void reload ();

	void render (State state);
	void update (State state, float dt);
	void clean ();
	void load ();
};