#pragma once

#include "object.h"
#include "state.h"
#include "glob.h"
#include "array_2d.h"
#include <list>

struct item : Object {
	rgba_array m_textures[5];
	rgba_array m_light[5];
	rgba_array m_light_smpl;
	struct one_item {
		v2i pos;
		int type_num;
		bool active;
	};
	struct sp_effect_square {
		v2i pos;
		float time;
	};
	list <one_item> m_all_items;
	list <sp_effect_square> m_sp_effect_square;
	struct redactor {
		one_item tmp;
	};
	redactor m_redact;
	float m_time;

	void render (State state);
	void update (State state, float dt);
	void clean ();
	void load ();
};