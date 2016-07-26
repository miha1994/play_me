#include "ship.h"
#include "Renderer.h"
#include "core.h"
#include "camera.h"
#include "solid.h"
#include "png.h"
#include "cat.h"

void ship::render (State state) {
	camera *cam = D_PLATFORM (camera);

	forstl (p, m_elements) {
		D_ADD_SPRITE (m_tex[p->inf.velocity.x > 0 ? 0 : 1], p->pos - v2i (0, 23) - cam->m_pos);
	}
	m_clouds.render (cam->m_pos);
}

void ship::update (State state, float dt) {
	camera *cam = D_PLATFORM (camera);
	solid *sld = D_PLATFORM (solid);
	cat *ct = D_PLATFORM (cat);

	m_clouds.upd (dt);

	if (state == State_PLATFORM_GAME) {
		forstl (p, m_generators) {
			p->tm -= dt;
			if (p->tm < 0) {
				p->tm += p->time_step;
				elem nw;
				nw.pos = p->pos;
				nw.inf = p->cpy_inf;
				nw.time_to_nxt_cloud = 0;
				m_elements.push_back (nw);
			}
		}
		forstl_no_inc (p, m_elements) {
			bool in_solid = false;
			v2i strt = p->pos;
			v2i cr;
			FOR (i, m_tex[0].m_W) {
				cr = strt + v2i(i, 0);
				if (sld->m_map << cr) {
					sld->m_map[cr] = sld->m_original_map[cr];
				}
			}
			p->pos += p->inf.velocity * dt;
			strt = p->pos;
			FOR (i, m_tex[0].m_W) {
				cr = strt + v2i(i, 0);
				if (sld->m_map << cr) {
					if (sld->m_map[cr] == 1) {
						in_solid = true;
					}
					sld->m_map[cr] = p->inf.velocity.x;
				}
			}

			p->in_solid._upd (in_solid);
			if (p->in_solid.just_pressed) {
				p->inf.lifes_left--;
				if (p->inf.lifes_left <= 0) {
					FOR (i, 10) {
						float len = 20 + rand1 * 30;
						m_clouds.add_elem (&m_tex_clouds[rand ()%2], p->pos + v2f (7 + rand () % (m_tex[0].m_W - 20), -15 + rand () % 20),
							p->inf.velocity, 0.1, 0.1, len, len / (length (p->inf.velocity) * (0.3 + 0.4 * rand1)));
					}
					FOR (i, m_tex[0].m_W) {
						cr = strt + v2i(i, 0);
						if (sld->m_map << cr) {
							sld->m_map[cr] = sld->m_original_map[cr];
						}
					}
					p = m_elements.erase (p);
					continue;
				}
			}

			if ((p->time_to_nxt_cloud -= dt) < 0) {
				p->time_to_nxt_cloud += ( (length (ct->m_pos - p->pos) > 400) ? 1 : 0.05);
				float len = 20 + rand1 * 30;
				m_clouds.add_elem (&m_tex_clouds[rand ()%2], p->pos + v2f (rand () % (m_tex[0].m_W - 6), 3 + rand () % 8),
					p->inf.velocity, len * 0.1, 0.1, len, len / (length (p->inf.velocity) * (0.3 + 0.4 * rand1)));
			}

			++p;
		}
	} else if (state == State_PLATFORM_REDACTOR && core.m_active_type == Ship) {
		if (in.mouse.left.just_pressed) {
			generator nw;
			nw.time_step = 2.5;
			nw.tm = 0;
			nw.pos = in.mouse.pos + cam->m_pos;
			nw.cpy_inf.lifes_left = 3;
			nw.cpy_inf.velocity = v2i (100, 0);
			m_generators.push_back (nw);
		}
	}
}

void ship::clean () {
	//PUT YOUR CODE HERE
}

void ship::load () {
	if (!load_png (m_tex[0], Texture_name("ship"))) {
		exit (0);
	}
	m_tex[0].alpha_matters = true;
	m_tex[1] = m_tex[0];
	m_tex[1].flip_horizontal ();
	if (!load_png (m_tex_clouds[0], Texture_name ("ship_clouds"), IntRect (v2i (0,0), v2i (7, 6)))) {
		exit (0);
	}
	if (!load_png (m_tex_clouds[1], Texture_name ("ship_clouds"), IntRect (v2i (7,0), v2i (10, 8)))) {
		exit (0);
	}
}
