#pragma once
#include "glob.h"
#include "frame_buffer.h"
#include <list>

struct animation {
	struct elem {
		frames *fr;
		v2i pos;
		float time;
		float fps_time;
		int current_frame;
	};
	list <elem> m_elements;

	void update (float dt);
	void render (v2i cam_pos);
	void add (frames *fr, v2i pos, int fps);
};