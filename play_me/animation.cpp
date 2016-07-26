#include "animation.h"
#include "Renderer.h"
#include "core.h"

void animation::update (float dt) {
	forstl_no_inc (p, m_elements) {
		p->time += dt;
		if (p->time > p->fps_time) {
			p->current_frame++;
			if (p->current_frame >= p->fr->m_frames.size ()) {
				p = m_elements.erase (p);
				continue;
			}
			p->time -= p->fps_time;
		}
		++p;
	}
}

void animation::render (v2i cam_pos) {
	forstl (p, m_elements) {
		D_ADD_SPRITE (p->fr->m_frames[p->current_frame], p->pos - cam_pos);
	}
}

void animation::add (frames *fr, v2i pos, int fps) {
	elem nw;
	nw.fps_time = 1.0f / float (fps);
	nw.fr = fr;
	nw.current_frame = 0;
	nw.pos = pos;
	nw.time = 0;
	m_elements.push_back (nw);
}