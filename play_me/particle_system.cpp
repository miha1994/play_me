#include "particle_system.h"
#include "core.h"
#include "Renderer.h"

void particle_sys_1::add_elem (rgba_array *tex, v2f start_position, v2f velocity_direction, float len1, float len2, float len3, float time_of_all_moving) {
	elem el;
	el.tex = tex;
	el.pos = start_position;
	float whole_len = len1 + len2 + len3;
	el.vel = velocity_direction * (whole_len / time_of_all_moving / length (velocity_direction));
	el.tm[0] = (len1 / whole_len) * time_of_all_moving;
	el.tm[1] = (len2 / whole_len) * time_of_all_moving;
	el.tm[2] = (len3 / whole_len) * time_of_all_moving;
	el.phase = 0;
	el.alpha1 = 0;
	m_particles.push_back (el);
}

void particle_sys_1::upd (float dt) {
	forstl_no_inc (p, m_particles) {
		p->time += dt;
		if (p->time > p->tm[p->phase]) {
			if (p->phase == 2) {
				p = m_particles.erase (p);
				continue;
			}
			p->time -= p->tm[p->phase];
			++ p->phase;
		}
		switch (p->phase) {
		case 0:
			p->alpha1 = p->time / p->tm[p->phase];
			break;
		case 1:
			p->alpha1 = 1;
			break;
		case 2:
			p->alpha1 = 1 - p->time / p->tm[p->phase];
			break;
		}
		p->pos += p->vel * dt;
		++p;
	}
}

void particle_sys_1::render (v2f cam_pos) {
	forstl (p, m_particles) {
		D_ADD_SPRITE_ALPHA (*p->tex, p->pos - cam_pos, p->alpha1);
	}
}